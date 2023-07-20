#include "FMTUtilities.h"
#include "FMPlugin.h"

#if FMX_WIN_TARGET

#endif

#if FMX_MAC_TARGET
    #include <CoreServices/CoreServices.h>
    #include <Carbon/Carbon.h>
#endif 

#include "FMWrapper/FMXBinaryData.h"
#include "FMWrapper/FMXData.h"
#include "FMWrapper/FMXFixPt.h"
#include "FMTSetup.h"
#include <Poco/FileChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/Path.h>
#include <Poco/Base64Encoder.h>
#include <Poco/Base64Decoder.h>
#include <Poco/Format.h>
#include <sstream>
#include <fstream>

static const std::string LOGGER_HEADER = "FMTFoundation";

enum { kXMpl_GetStringMaxBufferSize = 1024 };

namespace FMTUtilities
{
	errcode TextConstantFunction(const std::wstring& text, fmx::Data& results)
	{
		WStringAutoPtr text_constant(new std::wstring(text));	
		return TextConstantFunction(text_constant, results);	
	} // TextConstantFunction


	errcode TextConstantFunction(const WStringAutoPtr& text, Data& results)
	{
		errcode error_result = 0;

		try {
			TextAutoPtr result_text;
			result_text->AssignWide(text->c_str());

			LocaleAutoPtr default_locale;
			results.SetAsText(*result_text, *default_locale);

		} catch(std::exception&) {
			error_result = kErrorUnknown;
		}

		return error_result;

	} // TextConstantFunction


	// get parameters and set function results


#pragma mark -
#pragma mark SetResult
#pragma mark -

	void SetResult(const long number, Data& results)
	{
		FixPtAutoPtr numeric_result;
		numeric_result->AssignInt((int)number);
		results.SetAsNumber(*numeric_result);
	}


	void SetResult(const Text& text, Data& results)
	{
		LocaleAutoPtr default_locale;
		results.SetAsText(text, *default_locale);
	}


	void SetResult(const StringAutoPtr text, Data& results)
	{
		TextAutoPtr result_text;
		result_text->Assign(text->c_str(), Text::kEncoding_UTF8);			
		SetResult(*result_text, results);
	}


	void SetResult(const WStringAutoPtr text, Data& results)
	{
		TextAutoPtr result_text;
		result_text->AssignWide(text->c_str());			
		SetResult(*result_text, results);
	}

	void SetResult(const std::string& text, Data& results)
	{
		TextAutoPtr result_text;
		result_text->Assign(text.c_str(), Text::kEncoding_UTF8);			
		SetResult(*result_text, results);
	}

	void SetResult(const std::wstring& text, Data& results)
	{
		TextAutoPtr result_text;
		result_text->AssignWide(text.c_str());			
		SetResult(*result_text, results);
	}

	void SetResult(const std::string& filename, const std::vector<char>& data, Data& results)
	{
		bool as_binary = !filename.empty();

		if(as_binary) {	// if a file name is supplied send back a file

			BinaryDataAutoPtr resultBinary;
			TextAutoPtr file;
			file->Assign(filename.c_str(), Text::kEncoding_UTF8);
			resultBinary->AddFNAMData(*file); 
			QuadCharAutoPtr data_type('F', 'I', 'L', 'E'); 
			resultBinary->Add(*data_type, (int)data.size(), (void *)&data[0]);
			results.SetBinaryData(*resultBinary, true); 

		} else { // otherwise try sending back text

			// filemaker will go into an infinite loop if non-utf8 data is set as utf8
			// so try to convert it first

			const std::string data_string(data.begin(), data.end());
			StringAutoPtr utf8;// = ConvertTextToUTF8((char *)data_string.c_str(), data_string.size());
			SetResult(utf8, results);

		}

	} // SetBinaryDataResult


#pragma mark -
#pragma mark ParameterAs
#pragma mark -
	std::string getParameterAsUTF8String(const DataVect& parameters, unsigned int which)
	{
		std::string result("");

		try {
			TextAutoPtr raw_data;
			raw_data->SetText(parameters.AtAsText(which));

			unsigned int text_size = (2*(raw_data->GetSize())) + 1;
			char * text = new char [ text_size ]();
			raw_data->GetBytes(text, text_size, 0, (unsigned long)Text::kSize_End, Text::kEncoding_UTF8);
			result = std::string(text);
			delete [] text;

		} catch(std::exception&) {
			;	// return an empty string
		}

		return result;

	} // ParameterAsUTF8String

	WStringAutoPtr getParameterAsWideString(const DataVect& parameters, unsigned int which)
	{
		WStringAutoPtr result(new std::wstring);
		try {

			TextAutoPtr raw_data;
			raw_data->SetText(parameters.AtAsText(which));

			int text_size = raw_data->GetSize();
			ushort * text = new ushort [ text_size + 1 ];
			raw_data->GetUnicode(text, 0, text_size);
			text[text_size] = 0x0000;

			// wchar_t is 4 bytes on OS X and 2 on Windows

#if FMX_MAC_TARGET
			wchar_t * parameter = new wchar_t [ text_size + 1 ];
			for(long i = 0 ; i <= text_size ; i++) {
				parameter[i] = (wchar_t)text[i];
			}
			delete [] text;
#endif 

#if FMX_WIN_TARGET
			wchar_t * parameter = (wchar_t*)text;
#endif
			result->append(parameter);
			delete [] parameter; // parameter == text on Windows

		} catch(std::exception&) {
			;	// return an empty string
		}

		return result;

	} // ParameterAsUnicodeString

	void ReadString( FMX_Unichar* ioBuffer, FMX_UInt32 iBufferSize, FMX_UInt32 iStringID)
	{
#if FMX_WIN_TARGET
		LoadStringW(GetPluginInstance(), iStringID, (LPWSTR)ioBuffer, iBufferSize);
#endif

#if FMX_MAC_TARGET

		ioBuffer[0] = 0;

		// Turn stringID to a textual identifier, then get the string from the .strings file as a null-term unichar array.
		CFStringRef     strIdStr = CFStringCreateWithFormat( kCFAllocatorDefault, NULL, CFSTR( "String%d"), iStringID);

		// Note: The plug-in must be explicit about the bundle and file it wants to pull the string from.
		CFStringRef     osxStr = CFBundleCopyLocalizedString( GetPluginBundle(), strIdStr, strIdStr, CFSTR ("Localizable"));

		if((osxStr != NULL) && (osxStr != strIdStr))
		{
			long    osxStrLen = CFStringGetLength(osxStr);
			if( osxStrLen < (long)(iBufferSize-1))
			{
				CFStringGetCharacters(osxStr, CFRangeMake(0,osxStrLen), (UniChar*)(ioBuffer));
				ioBuffer[osxStrLen] = 0;

			};// osxStrLen

			CFRelease( osxStr);

		};// osxStr

		CFRelease( strIdStr);        

#endif
	}

#if FMX_MAC_TARGET

	unsigned long Sub_OSXLoadString(unsigned long stringID, FMX_Unichar* intoHere, long intoHereMax)
	{
		unsigned long       returnResult = 0;

		if( (intoHere != NULL) && (intoHereMax > 1))
		{
			// Turn stringID to a textual identifier, then get the string from the .strings file as a null-term unichar array.
			CFStringRef     strIdStr = CFStringCreateWithFormat( kCFAllocatorDefault, NULL, CFSTR( "String%d"), (int)stringID);

			// Note: The plug-in must be explicit about the bundle and file it wants to pull the string from.
			CFStringRef     osxStr = CFBundleCopyLocalizedString( reinterpret_cast<CFBundleRef>(gFMX_ExternCallPtr->instanceID), strIdStr, strIdStr, CFSTR("Localizable"));
			if((osxStr != NULL) && (osxStr != strIdStr))
			{
				long    osxStrLen = CFStringGetLength(osxStr);
				if( osxStrLen < (intoHereMax-1))
				{
					CFRange     allChars;
					allChars.location = 0;
					allChars.length = osxStrLen;

					CFStringGetCharacters(osxStr, allChars, (UniChar*)(intoHere));
					intoHere[osxStrLen] = 0x0000;
					returnResult = (unsigned long)osxStrLen;

				};// osxStrLen

				CFRelease( osxStr);

			};// osxStr

			CFRelease( strIdStr);

		};// intoHere

		return(returnResult);

	} // Sub_OSXLoadString

#endif

	fmx::errcode registerFunction(unsigned short funcId, const std::string& prototype_, unsigned short minArgs, unsigned short maxArgs, 
		unsigned int flags, fmx::ExtPluginType funcPtr)
	{
		std::string logMessage;
		//Poco::format(logMessage, "Registering function: id %hu - prototype %s, minArgs %hu, maxArgs %hu", 
			//funcId, prototype_, minArgs, maxArgs);
		//debug(logMessage);
		fmx::QuadCharAutoPtr    pluginID(PLUGIN_ID_STRING[0], PLUGIN_ID_STRING[1], PLUGIN_ID_STRING[2], PLUGIN_ID_STRING[3]);
		fmx::TextAutoPtr        name;
		fmx::TextAutoPtr        prototype;

		std::string funcName = prototype_;
		size_t index = prototype_.find('(');
		if (index != prototype_.npos) {
			funcName = prototype_.substr(0, index);
		}
		name->Assign(funcName.c_str(), Text::kEncoding_UTF8);
		prototype->Assign(prototype_.c_str(), Text::kEncoding_UTF8);
		fmx::errcode err = fmx::ExprEnv::RegisterExternalFunction(*pluginID, funcId, *name, *prototype, minArgs, maxArgs, flags, funcPtr);
		if (err) {
			Poco::format(logMessage, "Error occurred while registering function: id %hu prototype %s, minArgs %hu, maxArgs %hu. Error code (%hd)", 
			funcId, prototype_, minArgs, maxArgs, err);
			error(logMessage);
		}
		else {
			gRegisteredFunctions.push_back(funcId);
		}

		return err;
	}

	void initLogging()
	{
		Poco::AutoPtr<Poco::FileChannel> pChannel(new Poco::FileChannel);
		Poco::Path tmpFilePath = Poco::Path(Poco::Path::temp()).append("FMTFoundation.log");
		pChannel->setProperty("path", tmpFilePath.toString());
		Poco::AutoPtr<Poco::PatternFormatter> pPF(new Poco::PatternFormatter);
		pPF->setProperty("pattern", "%Y-%m-%d %H:%M:%S %s: %t");
		Poco::AutoPtr<Poco::FormattingChannel> pFC(new Poco::FormattingChannel(pPF, pChannel));
		Poco::Logger::root().setChannel(pFC);

	#ifdef _DEBUG
		Poco::Logger::root().setLevel(Poco::Message::PRIO_DEBUG);
	#endif
	}

	void info(const std::string& message)
	{
		Poco::Logger& logger = Poco::Logger::get(LOGGER_HEADER);
		logger.information(message);
	}

	void debug(const std::string& message)
	{
		Poco::Logger& logger = Poco::Logger::get(LOGGER_HEADER);
		logger.debug(message);
	}
	
	void error(const std::string& message)
	{
		Poco::Logger& logger = Poco::Logger::get(LOGGER_HEADER);
		logger.error(message);	
	}

	void info(const std::string& loggerName, const std::string& message)
	{
		Poco::Logger& logger = Poco::Logger::get(loggerName);
		logger.information(message);
	}

	void debug(const std::string& loggerName, const std::string& message)
	{
		Poco::Logger& logger = Poco::Logger::get(loggerName);
		logger.debug(message);
	}

	void error(const std::string& loggerName, const std::string& message)
	{
		Poco::Logger& logger = Poco::Logger::get(loggerName);
		logger.error(message);
	}

	void error(const std::string& loggerName, const Poco::Exception& ex)
	{
		Poco::Logger& logger = Poco::Logger::get(loggerName);
		logger.error(ex.message());
	}

	std::string toBase64 (const std::string &source)
	{
	  std::istringstream in(source);
	  std::ostringstream out;
	  Poco::Base64Encoder b64out(out);
 
	  std::copy(std::istreambuf_iterator<char>(in),
				std::istreambuf_iterator<char>(),
				std::ostreambuf_iterator<char>(b64out));
	  b64out.close(); // always call this at the end!
 
	  return out.str();
	}
	std::string fromBase64 (const std::string &source)
	{
	  std::istringstream in(source);
	  std::ostringstream out;
	  Poco::Base64Decoder b64in(in);
 
	  std::copy(std::istreambuf_iterator<char>(b64in),
				std::istreambuf_iterator<char>(),
				std::ostreambuf_iterator<char>(out));
 
	  return out.str();
	}

	void toBase64(const std::string &sourceFile, const std::string& targetFile)
	{
		std::ifstream ifs(sourceFile.c_str());
		std::ofstream ofs(targetFile.c_str());
		Poco::Base64Encoder b64out(ofs);
 
		std::copy(std::istreambuf_iterator<char>(ifs),
				std::istreambuf_iterator<char>(),
				std::ostreambuf_iterator<char>(b64out));
		b64out.close();
	}
	void fromBase64 (const std::string &sourceFile, const std::string& targetFile)
	{
		std::ifstream ifs(sourceFile.c_str());
		Poco::Base64Decoder b64in(ifs);
		std::ofstream ofs(targetFile.c_str());
 
		std::copy(std::istreambuf_iterator<char>(b64in),
			std::istreambuf_iterator<char>(),
            std::ostreambuf_iterator<char>(ofs));
		ofs.close();
	}
}
