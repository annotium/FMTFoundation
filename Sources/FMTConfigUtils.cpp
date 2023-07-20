#include "FMPlugin.h"
#include "FMTSetup.h"
#include "FMTConfigUtils.h"

#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/Util/XMLConfiguration.h>
#include <Poco/Util/IniFileConfiguration.h>
#if FMX_WIN_TARGET
#include <Poco/Util/WinRegistryConfiguration.h>
#include <Poco/Util/WinRegistryKey.h>
#endif
#include <Poco/Exception.h>
#include <Poco/UnicodeConverter.h>
#include <Poco/FileStream.h>
#include <Poco/File.h>

#include "FMTUtilities.h"

#define BUFFER_SIZE 256

#pragma mark ConfigUtils
FMX_PROC(fmx::errcode) FMT_GetPropertyValueFromXmlConfigFile(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetPropertyValueFromXmlConfigFile");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 2) {
		FMTUtilities::error("FMT_GetPropertyValueFromXmlConfigFile failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		return kErrorParameterMissing;
	}

	std::string xmlFile = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (xmlFile == "") {
		FMTUtilities::error("FMT_GetPropertyValueFromXmlConfigFile failed: Input file must not be empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}
	Poco::File file(xmlFile);
	if (!file.exists()) {
		FMTUtilities::info("FMT_GetPropertyValueFromXmlConfigFile failed: Input file '" + xmlFile + "' does not exist!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		return err;
	}

	std::string propName = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (propName == "") {
		FMTUtilities::error("FMT_GetPropertyValueFromXmlConfigFile failed: Property key name must not be empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	try {
		Poco::Util::AbstractConfiguration *cfg = new Poco::Util::XMLConfiguration(xmlFile);
		std::string value = cfg->getString(propName);
		FMTUtilities::SetResult(value, result);
	}
	catch (Poco::Exception ex) {
		FMTUtilities::error("FMT_GetPropertyValueFromXmlConfigFile failed: Exception occurred while reading xml configuration property '" + 
			propName + "'. " + ex.message());
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_SetPropertyValueFromXmlConfigFile(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_SetPropertyValueFromXmlConfigFile");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 3) { 
		FMTUtilities::error("FMT_SetPropertyValueFromXmlConfigFile failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		return kErrorParameterMissing;	
	}

	std::string xmlFile = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (xmlFile == "") {
		FMTUtilities::error("FMT_SetPropertyValueFromXmlConfigFile failed: Input file must not be empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}
	Poco::File file(xmlFile);
	if (!file.exists()) {
		FMTUtilities::info("FMT_SetPropertyValueFromXmlConfigFile failed: Input file '" + xmlFile + "' does not exist!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		return err;
	}

	std::string propName = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (propName == "") {
		FMTUtilities::error("FMT_SetPropertyValueFromXmlConfigFile failed: Property key name must not be empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	std::string sValue = FMTUtilities::getParameterAsUTF8String(dataVect, 2);

	try {
		Poco::Util::AbstractConfiguration *cfg = new Poco::Util::XMLConfiguration(xmlFile);
		cfg->setString(propName, sValue);
		FMTUtilities::SetResult(kNoError, result);
	}
	catch (Poco::Exception ex) {
		FMTUtilities::SetResult(kErrorUnknown, result);
		FMTUtilities::error("FMT_SetPropertyValueFromXmlConfigFile failed: Exception occurred while setting xml configuration property '" + 
			propName + "'. " + ex.message());
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_DeletePropertyXmlConfigFile(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_DeletePropertyXmlConfigFile");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 2) {
		FMTUtilities::error("FMT_DeletePropertyXmlConfigFile failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		return kErrorParameterMissing;
	}

	std::string xmlFile = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (xmlFile == "") {
		FMTUtilities::error("FMT_DeletePropertyXmlConfigFile failed: Input file must not be empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}
	Poco::File file(xmlFile);
	if (!file.exists()) {
		FMTUtilities::info("FMT_DeletePropertyXmlConfigFile failed: Input file '" + xmlFile + "' does not exist!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		return err;
	}

	std::string propName = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (propName == "") {
		FMTUtilities::error("FMT_DeletePropertyXmlConfigFile failed: Property key name must not be empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	try {
		Poco::Util::AbstractConfiguration *cfg = new Poco::Util::XMLConfiguration(xmlFile);
		cfg->remove(propName);
		FMTUtilities::SetResult(kNoError, result);
	}
	catch (Poco::Exception ex) {
		FMTUtilities::SetResult(kErrorUnknown, result);
		FMTUtilities::error("FMT_DeletePropertyXmlConfigFile failed: Exception occurred while remove property '" + 
			propName + "' in xml configuration '" + xmlFile + "'. " + ex.message());
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_SetIniValue(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_SetIniValue");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 3) {
		FMTUtilities::error("FMT_SetIniValue failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		return kErrorParameterMissing;
	}

	std::string iniFile = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (iniFile == "") {
		FMTUtilities::error("FMT_SetIniValue failed: Input file must not be empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}
	Poco::File file(iniFile);
	if (!file.exists()) {
		FMTUtilities::info("FMT_SetIniValue failed: Input file '" + iniFile + "' does not exist!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		return err;
	}

	std::string propName = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (propName == "") {
		FMTUtilities::error("FMT_SetIniValue failed: Property key name must not be empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	std::string sValue = FMTUtilities::getParameterAsUTF8String(dataVect, 2);

	try {
		FMTUtilities::SetResult(kNoError, result);
		Poco::Util::AbstractConfiguration* config = new Poco::Util::IniFileConfiguration(iniFile);
		config->setString(propName, sValue);
		FMTUtilities::SetResult(kNoError, result);
	}
	catch (Poco::Exception ex) {
		FMTUtilities::SetResult(kErrorUnknown, result);
		FMTUtilities::error("FMT_SetIniValue failed: Exception occurred while setting ini value for key '" + propName + "'. " + ex.message());
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_GetIniValue(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetIniValue");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 2) {
		FMTUtilities::error("FMT_GetIniValue failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		return kErrorParameterMissing;
	}

	std::string iniFile = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (iniFile == "") {
		FMTUtilities::error("FMT_GetIniValue failed: Input file must not be empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}
	Poco::File file(iniFile);
	if (!file.exists()) {
		FMTUtilities::info("FMT_GetIniValue failed: Input file '" + iniFile + "' does not exist!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		return err;
	}

	std::string propName = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (propName == "") {
		FMTUtilities::error("FMT_GetIniValue failed: Property key name must not be empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	try {
		FMTUtilities::SetResult(kNoError, result);
		Poco::Util::AbstractConfiguration* config = new Poco::Util::IniFileConfiguration(iniFile);
		std::string value = config->getString(propName);
		FMTUtilities::SetResult(value, result);
	}
	catch (Poco::Exception ex) {
		FMTUtilities::SetResult("", result);
		FMTUtilities::error("Exception occurred while setting ini value for key '" + propName + "'. " + ex.message());
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_DeleteIniKey(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_DeleteIniKey");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 2) {
		FMTUtilities::error("FMT_DeleteIniKey failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		return kErrorParameterMissing;
	}

	std::string iniFile = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (iniFile == "") {
		FMTUtilities::error("FMT_DeleteIniKey failed: Input file must not be empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}
	Poco::File file(iniFile);
	if (!file.exists()) {
		FMTUtilities::info("FMT_DeleteIniKey failed: Input file '" + iniFile + "' does not exist!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		return err;
	}

	std::string propName = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (propName == "") {
		FMTUtilities::error("FMT_DeleteIniKey failed: Property key name must not be empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	try {
		FMTUtilities::SetResult(kNoError, result);
		Poco::Util::AbstractConfiguration* config = new Poco::Util::IniFileConfiguration(iniFile);
		config->remove(propName);
		FMTUtilities::SetResult(kNoError, result);
	}
	catch (Poco::Exception ex) {
		FMTUtilities::SetResult("", result);
		FMTUtilities::error("FMT_DeleteIniKey failed: Exception occurred while setting ini value for key '" +
			propName + "'. " + ex.message());
		err = kErrorUnknown;
	}

	return err;
}