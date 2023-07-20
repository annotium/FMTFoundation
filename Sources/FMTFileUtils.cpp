#include "FMPlugin.h"
#include "FMTSetup.h"
#include <Poco/File.h>
#include <Poco/Path.h>
#include <Poco/Exception.h>
#include <Poco/UnicodeConverter.h>
#include <Poco/FileStream.h>
#include <Poco/StringTokenizer.h>
#include <Poco/DirectoryIterator.h>

#include "FMTUtilities.h"
#include "FMTFileUtils.h"
#include <iostream>
#include <fstream>
#include <locale>

#if FMX_WIN_TARGET
    #include <ShellApi.h>
    #include <codecvt>
#endif

#define BUFFER_SIZE 256

#pragma mark FileUtils

FMX_PROC(fmx::errcode) FMT_CreateFolder(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_CreateFolder");
	fmx::errcode err = kNoError;

	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_CreateFolder failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		err = kErrorUnknown;
	}

	const std::string& folderPathStr = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (folderPathStr == "") {
		FMTUtilities::error("FMT_CreateFolder failed: Invalid path. Parent folder path must be not empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	Poco::Path folderPath(folderPathStr);
	Poco::File file(folderPath);
	if (file.exists()) {
		std::string message;
		Poco::format(message, "FMT_CreateFolder failed: Folder %s already existed", folderPath);
		FMTUtilities::error(message);
		FMTUtilities::SetResult(kFileOrFolderAlreadyExisted, result);
		return kErrorUnknown;
	}
	
	try {
		file.createDirectories();
	}
	catch (Poco::IOException ex) {
		std::string message;
		Poco::format(message, "FMT_CreateFolder failed: Exception occurred while creating folder %s. Error: %s (code = %d)",
			folderPath.toString(), ex.message(), ex.code()); 
		FMTUtilities::error(message);
		FMTUtilities::SetResult(ex.code(), result);
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_DeleteFileOrFolder(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_DeleteFileOrFolder");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_DeleteFileOrFolder failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		err = kErrorParameterMissing;
	}

	std::string path = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (path == "") {
		FMTUtilities::error("FMT_DeleteFileOrFolder failed: Invalid path. Path must be not empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	Poco::File file(path);
	if (!file.exists()) {
		FMTUtilities::info("File/folder '" + path + "' does not exist!");
		FMTUtilities::SetResult(kNoError, result);
		return err;
	}

	try {
		file.remove(true);
		FMTUtilities::SetResult(kNoError, result);
	}
	catch (Poco::Exception ex) {
		std::string message;
		Poco::format(message, "FMT_DeleteFileOrFolder failed: Exception occurred while deleting folder %s. Error: %s (code = %d)",
			path, ex.message(), ex.code()); 
		FMTUtilities::error(message);
		FMTUtilities::SetResult(ex.code(), result);
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_RenameFileOrFolder(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_RenameFileOrFolder");
	fmx::errcode err = 0;
	if (dataVect.Size() < 2) {
		FMTUtilities::info("FMT_RenameFileOrFolder failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		err = kErrorParameterMissing;
	}

	std::string path = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	std::string newPath = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (path == "" || newPath == "") {
		FMTUtilities::error("FMT_RenameFileOrFolder failed: Invalid source/target path. Path must be not empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	Poco::File file(path);
	if (!file.exists()) {
		FMTUtilities::error("FMT_RenameFileOrFolder failed: Source file/folder '" + path + "' does not exist!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		err = kErrorUnknown;
	}

	try {
		file.renameTo(newPath);
		FMTUtilities::SetResult(0, result);
	}
	catch (Poco::Exception ex) {
		std::string message;
		Poco::format(message, 
			"FMT_RenameFileOrFolder failed: Exception occurred while rename file/folder %s. Error: %s (code = %d)",
				path, ex.message(), ex.code()); 
		FMTUtilities::error(message);
		FMTUtilities::SetResult(ex.code(), result);
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_MoveFileOrFolderTo(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_MoveFileOrFolderTo");
	fmx::errcode err = kNoError;

	if (dataVect.Size() < 2) {
		FMTUtilities::info("FMT_MoveFileOrFolderTo failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		err = kErrorParameterMissing;
	}

	std::string path = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	std::string newPath = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (path == "" || newPath == "") {
		FMTUtilities::error("FMT_MoveFileOrFolderTo failed: Invalid source/target path. Path must be not empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	Poco::File file(path);
	if (!file.exists()) {
		FMTUtilities::error("FMT_MoveFileOrFolderTo failed: Source file/folder '" + path + "' does not exist!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		err = kErrorUnknown;
	}

	try {
		file.moveTo(newPath);
		FMTUtilities::SetResult(kNoError, result);
	}
	catch (Poco::Exception ex) {
		std::string message;
		Poco::format(message, 
			"FMT_MoveFileOrFolderTo failed: Exception occurred while moving file/folder '%s' to '%s'. Error: %s (code = %d)",
				path, newPath, ex.message(), ex.code()); 
		FMTUtilities::error(message);
		FMTUtilities::SetResult(ex.code(), result);
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_CopyFileOrFolderTo(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_CopyFileOrFolderTo");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 2) {
		FMTUtilities::info("FMT_MoveFileOrFolderTo failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		err = kErrorParameterMissing;
	}

	std::string path = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	std::string newPath = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (path == "" || newPath == "") {
		FMTUtilities::error("FMT_MoveFileOrFolderTo failed: Invalid source/target path. Path must be not empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	Poco::File file(path);
	if (!file.exists()) {
		FMTUtilities::error("FMT_MoveFileOrFolderTo failed: Source file/folder '" + path + "' does not exist!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		err = kErrorUnknown;
	}
	
	Poco::File newFile(newPath);
	try {
		file.copyTo(newPath);
		FMTUtilities::SetResult(kNoError, result);
	}
	catch (Poco::Exception ex) {
		std::string message;
		Poco::format(message, 
			"FMT_MoveFileOrFolderTo failed: Exception occurred while copying file/folder %s to %s. Error: %s (code = %d)",
				path, newPath, ex.message(), ex.code()); 
		FMTUtilities::error(message);
		FMTUtilities::SetResult(ex.code(), result);
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_OpenFileOrFolderInDefaultApp(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_CopyFileOrFolderTo");
	fmx::errcode err = 0;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_OpenFileOrFolderInDefaultApp failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		err = kErrorUnknown;
	}

	std::string path = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (path == "") {
		FMTUtilities::error("FMT_OpenFileOrFolderInDefaultApp failed: Input path is empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	Poco::File file(path);
	if (!file.exists()) {
		FMTUtilities::info("FMT_OpenFileOrFolderInDefaultApp failed: Input path '" + path + "' does not exist!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		return err;
	}

#if FMX_WIN_TARGET
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring narrow = converter.from_bytes(path.c_str());
	ShellExecute(0, 0, narrow.c_str(), 0, 0 , SW_SHOW);
#else
    std::string command = "open " + path;
	system(command.c_str());
#endif

	return err;
}

FMX_PROC(fmx::errcode) FMT_FileExists(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_FileExists");
	fmx::errcode  err = kNoError;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_FileExists failed: Missing parameters");
		err = kErrorUnknown;
	}
	std::string path = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (path == "") {
		FMTUtilities::error("FMT_FileExists failed: Filepath is empty. Path must be not empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	Poco::File file(path);
	FMTUtilities::SetResult(file.exists(), result);

	return err;
}

FMX_PROC(fmx::errcode) FMT_ListFilesInDirectory(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_ListFilesInDirectory");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_ListFilesInDirectory failed: Missing parameters");
		FMTUtilities::SetResult("", result);
		err = kErrorParameterMissing;
	}

	std::string path = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (path == "") {
		FMTUtilities::error("FMT_ListFilesInDirectory failed: Folder path can not be empty!");
		FMTUtilities::SetResult("", result);
		return kInvalidPathError;
	}

	int itMode = dataVect.AtAsNumber(1).AsLong();

	Poco::File file(path);
	if (!file.isDirectory()) {
		FMTUtilities::info("FMT_ListFilesInDirectory failed: Input path '" + path + "' is not directory!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		return err;
	}

	std::vector<std::string> files;
	Poco::DirectoryIterator it(path);
	Poco::DirectoryIterator dend;
	while (it != dend) {
		if (it->isDirectory()) {
			if (itMode == 0 || itMode == 1) {
				files.push_back(it.path().toString());
			}
		}
		else {
			if (itMode == 0 || itMode == 2) {
				files.push_back(it.path().toString());
			}
		}
        ++it;
	}

	std::string filesStr;
	for (int i = 0; i < files.size(); ++i) {
		if (filesStr.length() > 0) {
			filesStr += ";";
		}
		filesStr += files[i];
	}
	FMTUtilities::SetResult(filesStr, result);

	return err;
}

FMX_PROC(fmx::errcode) FMT_ReadTextFile(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_ReadTextFile");

	fmx::errcode err = kNoError;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_ReadTextFile failed: Missing parameters");
		FMTUtilities::SetResult("", result);
		err = kErrorParameterMissing;
	}

	std::string path = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (path == "") {
		FMTUtilities::error("FMT_ReadTextFile failed: Input file is empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}
	Poco::File file(path);
	if (!file.exists()) {
		FMTUtilities::info("FMT_ReadTextFile failed: Input file '" + path + "' does not exist!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		return err;
	}

	try {
		Poco::FileInputStream fis(path);
		std::string fileText;
		std::size_t fileSize = file.getSize();
		fileText.resize(fileSize);
		fis.read(&fileText[0], fileSize);
		FMTUtilities::SetResult(fileText, result);
	}
	catch (Poco::Exception ex) {
		std::string message;
		Poco::format(message, 
			"FMT_ReadTextFile failed: Exception occurred while reading text file %s. Error: %s (code = %d)",
				path, ex.message(), ex.code()); 
		FMTUtilities::error(message);
		FMTUtilities::SetResult("", result);
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_WriteTextFile(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_WriteTextFile");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 2) {
		FMTUtilities::info("FMT_WriteTextFile failed: Missing parameters");
		FMTUtilities::SetResult(kErrorParameterMissing, result);
		err = kErrorUnknown;
	}

	std::string path = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (path == "") {
		FMTUtilities::error("FMT_WriteTextFile failed: Input file is empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	Poco::File file(path);
	std::string writeText = FMTUtilities::getParameterAsUTF8String(dataVect, 1);

	try {
		Poco::FileOutputStream fos(path);
		fos << writeText;
		fos.close();
		FMTUtilities::SetResult(kNoError, result);
	}
	catch (Poco::Exception ex) {
		std::string message;
		Poco::format(message, 
			"FMT_WriteTextFile failed: Exception occurred while writing text %s to file '%s'. Error: %s (code = %d)",
			writeText, path, ex.message(), ex.code()); 
		FMTUtilities::error(message);
		FMTUtilities::SetResult(ex.code(), result);
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_ReadBinaryFile(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_ReadBinaryFile");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_ReadBinaryFile failed: Missing parameters");
		err = kErrorUnknown;
	}

	std::string path = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (path == "") {
		FMTUtilities::error("FMT_ReadBinaryFile failed: Input file is empty");
		return kErrorUnknown;
	}
	Poco::File file(path);
	if (!file.exists()) {
		FMTUtilities::info("FMT_ReadBinaryFile failed: Input file '" + path + "' does not exist!");
		FMTUtilities::SetResult(kFileNotFoundError, result);
		return err;
	}

	try {
		Poco::File file(path);
		if (!file.exists()) {
			FMTUtilities::error("FMT_ReadBinaryFile failed: Input file does not exist");
			return kFileNotFoundError;
		}

		Poco::File::FileSize size = file.getSize();
		Poco::FileInputStream fis(path);
		fmx::BinaryDataAutoPtr binaryData;

		std::vector<char> data(size);
		fis.read(&data[0], size);
		fis.close();

		std::string fileName = path.substr(path.find_last_of("\\") + 1);
		FMTUtilities::SetResult(fileName, data, result);
	}
	catch (Poco::Exception ex) {
		std::string message;
		Poco::format(message, 
			"FMT_ReadBinaryFile failed: Exception occurred while reading binary file %s. Error: %s (code = %d)",
				path, ex.message(), ex.code()); 
		FMTUtilities::error(message);
		FMTUtilities::SetResult(ex.code(), result);
		err = kErrorUnknown;
	}

	return err;
}

FMX_PROC(fmx::errcode) FMT_GetTempFolder(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetTempFolder");
	FMTUtilities::SetResult(Poco::Path::temp(), result);
	return kNoError;
}

FMX_PROC(fmx::errcode) FMT_GetCurrentDirectory(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetCurrentDirectory");
	FMTUtilities::SetResult(Poco::Path::current(), result);
	return kNoError;
}

FMX_PROC(fmx::errcode) FMT_GetUserHomeDirectory(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetCurrentDirectory");
	FMTUtilities::SetResult(Poco::Path::home(), result);
	return kNoError;
}

/*FMX_PROC(fmx::errcode) FMT_FindFile(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_FindFile");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 3) {
		FMTUtilities::info("FMT_FindFile failed: Missing parameters");
		FMTUtilities::SetResult("", result);
		err = kErrorParameterMissing;
	}

	std::string pathList = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (pathList == "") {
		FMTUtilities::error("FMT_FindFile failed: Input paths is empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	std::string fileName = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
	if (fileName == "") {
		FMTUtilities::error("FMT_FindFile failed: Input file name is empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	std::vector<std::string> searchPath;
	Poco::StringTokenizer tokenizer(pathList, ";");
	std::vector<std::string>::const_iterator iter = tokenizer.begin();
	for (; iter != tokenizer.end(); ++iter) {
		Poco::File fsFile(*iter);

		if (!fsFile.exists()) {
			FMTUtilities::error("FMT_FindFile failed: Input path '" + *iter + "' does not exist!");
			continue;
		}
		
		searchPath.push_back(*iter);
	}

	try {
		//Poco::File file;
		//file.fin
		//Poco::FileInputStream fis(path);
		//std::string readText;
		//fis >> readText;
		//FMTUtilities::SetResult(readText, result);
	}
	catch (Poco::Exception ex) {
				std::string message;
		Poco::format(message, 
			"FMT_FindFile failed: Exception occurred while finding file '%s' in path list '%s'. Error: %s (code = %d)",
				fileName, pathList, ex.message(), ex.code()); 
		FMTUtilities::error(message);
		FMTUtilities::SetResult("", result);
		err = kErrorUnknown;
	}

	return kNoError;
}*/

FMX_PROC(fmx::errcode) FMT_IsDirectory(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_IsDirectory");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_IsDirectory failed: Missing parameters");
		FMTUtilities::SetResult("", result);
		err = kErrorParameterMissing;
	}

	std::string path = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (path == "") {
		FMTUtilities::error("FMT_IsDirectory failed: Input file is empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}

	Poco::File file(path);
	if (!file.exists()) {
		std::string message;
		Poco::format(message, "FMT_IsDirectory failed: Path '%s' does not exist", path);
		FMTUtilities::error(message);
		FMTUtilities::SetResult(kNoSuchFileOrDirectoryError, result);
		return kErrorUnknown;
	}

	FMTUtilities::SetResult(file.isDirectory(), result);

	return kNoError;
}

FMX_PROC(fmx::errcode) FMT_GetFileSize(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetFileSize");
	fmx::errcode err = kNoError;
	if (dataVect.Size() < 1) {
		FMTUtilities::info("FMT_GetFileSize failed: Missing parameters");
		FMTUtilities::SetResult("", result);
		err = kErrorParameterMissing;
	}

	std::string path = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
	if (path == "") {
		FMTUtilities::error("FMT_GetFileSize failed: Input file is empty");
		FMTUtilities::SetResult(kInvalidPathError, result);
		return kErrorUnknown;
	}
	Poco::File file(path);
	if (!file.exists()) {
		std::string message;
		Poco::format(message, "FMT_GetFileSize failed: File %s does not exist", path);
		FMTUtilities::error(message);
		FMTUtilities::SetResult(kNoSuchFileOrDirectoryError, result);
		return kErrorUnknown;
	}

	FMTUtilities::SetResult(file.getSize(), result);

	return kNoError;
}