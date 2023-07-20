#ifndef __FMPLUGINUTILITIES_H__
#define __FMPLUGINUTILITIES_H__

#include "FMPlugInSDK/Headers/FMWrapper/FMXTypes.h"
#include "FMPlugInSDK/Headers/FMWrapper/FMXText.h"
#include "FMPlugInSDK/Headers/FMWrapper/FMXCalcEngine.h"
#include <vector>
#include <Poco/Logger.h>

using namespace fmx;

typedef std::auto_ptr<std::string> StringAutoPtr;
typedef std::auto_ptr<std::wstring> WStringAutoPtr;

namespace FMTUtilities
{
	errcode TextConstantFunction(const std::wstring& text, Data& results);
	errcode TextConstantFunction(const WStringAutoPtr& text, Data& results);

	void SetResult(const long number, Data& results);
	void SetResult(const Text& text, Data& results);
	void SetResult(const std::string& text, Data& results);
	void SetResult(const std::wstring& text, Data& results);
	void SetResult(const StringAutoPtr text, Data& results);
	void SetResult(const WStringAutoPtr text, Data& results);
	void SetResult(const std::string& filename, const std::vector<char>& data, Data& results);

	std::string getParameterAsUTF8String(const DataVect& parameters, unsigned int which);
	WStringAutoPtr getParameterAsWideString(const DataVect& parameters, unsigned int which);
	std::string toBase64(const std::string &source);
	std::string fromBase64 (const std::string &source);
	void toBase64(const std::string &sourceFile, const std::string& targetFile);
	void fromBase64 (const std::string &sourceFile, const std::string& targetFile);
	// read/write string
	void ReadString(FMX_Unichar* buffer, unsigned int bufferSize, unsigned int stringID);
	fmx::errcode registerFunction(unsigned short funcId, const std::string& prototype, unsigned short minArgs, unsigned short maxArgs, 
		unsigned int flags, fmx::ExtPluginType iFunctionPtr);
	fmx::errcode unregisterFunction(unsigned short funcId);

	void initLogging();
	void info(const std::string& message);
	void debug(const std::string& message);
	void error(const std::string& message);
	void error(const Poco::Exception& ex); 
	void info(const std::string& loggerName, const std::string& message);
	void debug(const std::string& loggerName, const std::string& message);
	void error(const std::string& loggerName, const std::string& message);
	void error(const std::string& loggerName, const std::exception& ex); 
}

#endif // __FMPLUGINUTILITIES_H__