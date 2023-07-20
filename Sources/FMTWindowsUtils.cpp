#include "FMPlugin.h"
#include "FMTSetup.h"
#include <Poco/Exception.h>
#include <Poco/UnicodeConverter.h>
#include "FMTUtilities.h"
#include <Poco/Util/WinRegistryKey.h>
#include <Poco/Util/WinRegistryConfiguration.h>
#include <Poco/Util/AbstractConfiguration.h>

#pragma mark WindowsUtils

// registry
FMX_PROC(fmx::errcode) FMT_ReadRegistryKey(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_ReadRegistryKey");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 2) {
		FMTUtilities::info("FMT_ReadRegistryKey failed: Missing parameters");
		err = kErrorUnknown;
	}

	std::string registryKey = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (registryKey == "") {
		FMTUtilities::error("FMT_ReadRegistryKey failed: Registry key is empty");
		FMTUtilities::SetResult("", result);
		return kErrorUnknown;
	}
	
	std::string key = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (key == "") {
		FMTUtilities::error("FMT_ReadRegistryKey failed: Input key is empty");
		FMTUtilities::SetResult("", result);
		return kErrorUnknown;
	}

	try {
		Poco::Util::AbstractConfiguration* config = new Poco::Util::WinRegistryConfiguration(registryKey);
		std::string value = config->getRawString(key);
		FMTUtilities::SetResult(value, result);
	}
	catch(Poco::Exception ex) {
		FMTUtilities::error("FMT_ReadRegistryKey failed: Exception occurred while reading registry key '" + 
			registryKey + "' and key '" + key + "'. " + ex.message());
		FMTUtilities::SetResult(ex.code(), result);
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_WriteRegistryKey(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_ReadRegistryKey");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 3) {
		FMTUtilities::info("FMT_WriteRegistryKey failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		err = kErrorUnknown;
	}

	std::string registryKey = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (registryKey == "") {
		FMTUtilities::error("FMT_WriteRegistryKey failed: Registry key is empty");
		FMTUtilities::SetResult(kInvalidRegistryKey, result);
		return kErrorUnknown;
	}
	
	std::string key = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (key == "") {
		FMTUtilities::error("FMT_WriteRegistryKey failed: Input key is empty");
		FMTUtilities::SetResult(kInvalidInput, result);
		return kErrorUnknown;
	}

	std::string value = FMTUtilities::getParameterAsUTF8String(dataVect, 1);

	try {
		Poco::Util::AbstractConfiguration* config = new Poco::Util::WinRegistryConfiguration(registryKey);
		config->setString(key, value);
		FMTUtilities::SetResult(kNoError, result);
	}
	catch(Poco::Exception ex) {
		FMTUtilities::error("FMT_WriteRegistryKey failed: Exception occurred while writing registry key '" + 
			registryKey + "' and key/value '" + key + "/" + value + "'. " + ex.message());
		FMTUtilities::SetResult(ex.code(), result);
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_DeleteRegistryKey(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_DeleteRegistryKey");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 2) {
		FMTUtilities::info("FMT_DeleteRegistryKey failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		err = kErrorUnknown;
	}

	std::string registryKey = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (registryKey == "") {
		FMTUtilities::error("FMT_DeleteRegistryKey failed: Registry key is empty");
		FMTUtilities::SetResult(kInvalidRegistryKey, result);
		return kErrorUnknown;
	}
	
	std::string key = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (key == "") {
		FMTUtilities::error("FMT_ReadRegistryKey failed: Input key is empty");
		FMTUtilities::SetResult(kInvalidInput, result);
		return kErrorUnknown;
	}

	try {
		Poco::Util::AbstractConfiguration* config = new Poco::Util::WinRegistryConfiguration(registryKey);
		config->remove(key);
		FMTUtilities::SetResult(kNoError, result);
	}
	catch(Poco::Exception ex) {
		FMTUtilities::error("FMT_ReadRegistryKey failed: Exception occurred while removing registry key '" + 
			registryKey + "' and key '" + key + "'. " + ex.message());
		FMTUtilities::SetResult(ex.code(), result);
		err = kErrorUnknown;
	}

	return err;
}
