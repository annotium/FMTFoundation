#include "FMPlugin.h"
#include "FMTSetup.h"
#include "FMTNetworkUtils.h"

#include <Poco/Net/HTTPBasicCredentials.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net//HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/UnicodeConverter.h>
#include <Poco/FileStream.h>
#include <Poco/File.h>

#include "FMTUtilities.h"

#define BUFFER_SIZE 256

#pragma mark NetworkUtils

	// http post/get
FMX_PROC(fmx::errcode) FMT_HttpDownloadFile(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_HttpDownloadFile");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 2) {
		FMTUtilities::error("Invoke function FMT_HttpDownloadFile failed because of missing parameters");
		FMTUtilities::SetResult(kErrorUnknown, result);
		return kErrorParameterMissing;
	}

	std::string uriPath = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	std::string localFilePath = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	Poco::File file(localFilePath);
	if (localFilePath.length() == 0) {
		FMTUtilities::info("FMT_HttpDownloadFile failed: Input file is not valid!");
		FMTUtilities::SetResult(kInvalidInput, result);
		return err;
	}

	FMTUtilities::debug("Download URI '" + uriPath + "' to file: '" + localFilePath + "'");
	std::string username = FMTUtilities::getParameterAsUTF8String(dataVect, 2);
	std::string password = FMTUtilities::getParameterAsUTF8String(dataVect, 3);
		
	try {
		Poco::URI uri(uriPath);
		Poco::Net::HTTPClientSession session(uri.getHost(), uri.getPort());

		// prepare path
		std::string path(uri.getPathAndQuery());
		if (path.empty()) 
			path = "/";

		Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
		if (username.length() > 0 && password.length() > 0) {
			Poco::Net::HTTPBasicCredentials cred(username, password);
			cred.authenticate(req);
		}

		session.sendRequest(req);

		// get response
		Poco::Net::HTTPResponse res;
		std::string message;
		Poco::format(message, "HTTP request status %d, reason %s", res.getStatus(), res.getReason()); 
		FMTUtilities::debug(message);

		// output response
		std::istream& is = session.receiveResponse(res);
		Poco::FileOutputStream fos(localFilePath);
		Poco::StreamCopier::copyStream(is, fos);
		fos.close();
		FMTUtilities::SetResult(kNoError, result);
	}
	catch (Poco::Exception ex) {
		std::string message;
		Poco::format(message, 
			"FMT_HttpDownloadFile failed: Exception occurred while downloading file %s to %s. Error: %s (code = %d)",
			uriPath, localFilePath, ex.message(), ex.code()); 
		FMTUtilities::error(message);
		FMTUtilities::SetResult(ex.code(), result);
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_HttpUploadFile(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_HttpUploadFile");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 2) {
		FMTUtilities::error("Invoke function FMT_HttpUploadFile failed because of missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		return kErrorParameterMissing;
	}
	else {
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		err = kErrorUnknown;
	}
	return err;
}