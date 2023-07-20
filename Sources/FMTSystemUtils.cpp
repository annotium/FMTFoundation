#include "FMPlugin.h"
#include "FMTSetup.h"
#include "FMTUtilities.h"
#include "FMTSystemUtils.h"

#include <Poco/Exception.h>
#include <Poco/UnicodeConverter.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Process.h>
#include <Poco/Format.h>
#include <Poco/Environment.h>

#if FMX_WIN_TARGET
#include <Windows.h>
#else
#endif

#pragma mark SystemUtils

FMX_PROC(fmx::errcode) FMT_ExecuteCommandLine(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_ExecuteCommandLine");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_ExecuteCommandLine failed: Missing parameters");
		err = kErrorParameterMissing;
	}

	std::string commandLine = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (commandLine == "") {
		FMTUtilities::error("FMT_ExecuteCommandLine failed: Input string is empty");
		FMTUtilities::SetResult(kInvalidInput, result);
		return kErrorUnknown;
	}

	std::vector<std::string> args;
	std::string arguments = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	args.push_back(arguments);

	try {
		Poco::ProcessHandle ph = Poco::Process::launch(commandLine, args);
	}
	catch (Poco::Exception ex) {
		std::string message;
		Poco::format(message, 
			"FMT_ExecuteCommandLine failed: Exception occurred while executing command %s %s. Error: %s (code = %d)",
			commandLine, arguments, ex.message(), ex.code()); 
		FMTUtilities::error(message);
		FMTUtilities::SetResult(ex.code(), result);
		err = kErrorUnknown;	
	}

	return err;
}
// base 64
FMX_PROC(fmx::errcode) FMT_EncryptBase64(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_EncryptBase64");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_EncryptBase64 failed: Missing parameters");
		err = kErrorParameterMissing;
	}
	
	std::string source = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (source == "") {
		FMTUtilities::error("FMT_EncryptBase64 failed: Input string is empty");
		FMTUtilities::SetResult("", result);
		return kErrorUnknown;
	}

	std::string value = FMTUtilities::toBase64(source);
	FMTUtilities::SetResult(value, result);
	return err;
}

FMX_PROC(fmx::errcode) FMT_DecryptBase64(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_DecryptBase64");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_DecryptBase64 failed: Missing parameters");
		err = kErrorUnknown;
	}

	std::string source = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (source == "") {
		FMTUtilities::error("FMT_DecryptBase64 failed: Input string is empty");
		FMTUtilities::SetResult("", result);
		return kErrorUnknown;
	}

	std::string value = FMTUtilities::fromBase64(source);
	FMTUtilities::SetResult(value, result);
	return err;
}

FMX_PROC(fmx::errcode) FMT_GetOSName(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetOSName");
	FMTUtilities::SetResult(Poco::Environment::osName(), result);
	return kNoError;
}

FMX_PROC(fmx::errcode) FMT_GetOSVersion(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetOSVersion");
	FMTUtilities::SetResult(Poco::Environment::osVersion(), result);
	return kNoError;
}

FMX_PROC(fmx::errcode) FMT_GetComputerName(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetComputerName");
	FMTUtilities::SetResult(Poco::Environment::nodeName(), result);
	return kNoError;
}

FMX_PROC(fmx::errcode) FMT_GetProcessorNumber(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetProcessorNumber");
	FMTUtilities::SetResult(Poco::Environment::processorCount(), result);
	return kNoError;
}

FMX_PROC(fmx::errcode) FMT_GetEnvironmentVariable(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetEnvironmentVariable");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_GetEnvironmentVariable failed: Missing parameters");
		err = kErrorUnknown;
	}
	
	std::string variable = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (variable == "") {
		FMTUtilities::error("FMT_GetEnvironmentVariable failed: Input string is empty");
		FMTUtilities::SetResult("", result);
		return kErrorUnknown;
	}

	FMTUtilities::SetResult(Poco::Environment::get(variable), result);
    return err;
}

FMX_PROC(fmx::errcode) FMT_HasEnvironmentVariable(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_HasEnvironmentVariable");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_GetEnvironmentVariable failed: Missing parameters");
		err = kErrorUnknown;
	}
	
	std::string variable = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (variable == "") {
		FMTUtilities::error("FMT_GetEnvironmentVariable failed: Input string is empty");
		FMTUtilities::SetResult(0, result);
		return kErrorUnknown;
	}

	FMTUtilities::SetResult(Poco::Environment::has(variable), result);
    return err;
}

FMX_PROC(fmx::errcode) FMT_SetEnvironmentVariable(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
    FMTUtilities::info("Invoke function: FMT_SetEnvironmentVariable");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 2) {
		FMTUtilities::info("FMT_SetEnvironmentVariable failed: Missing parameters");
		err = kErrorUnknown;
	}
	
	std::string variable = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (variable == "") {
		FMTUtilities::error("FMT_SetEnvironmentVariable failed: Input string is empty");
		FMTUtilities::SetResult(kInvalidInput, result);
		return kErrorUnknown;
	}

	std::string value = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	Poco::Environment::set(variable, value);
	FMTUtilities::SetResult(kNoError, result);
    return err;
}

FMX_PROC(fmx::errcode) FMT_GetUserName(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetUserName");
	fmx::errcode err = kNoError;
	std::wstring userName;
	userName.resize(128);
	/*if (::GetUserName(&userName[0], 128)) {
		FMTUtilities::SetResult(userName, result);
	}
	else{
		FMTUtilities::SetResult("", result);
	}*/
	return err;
}