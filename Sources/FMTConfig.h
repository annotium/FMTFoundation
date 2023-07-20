#ifndef __FMTCONFIG_H__
#define __FMTCONFIG_H__

#include "FMPlugin.h"

////////////////////////////////////////////////////////////////////////////////
//
//  Shared constants
//
/////////////////////////////////////////////////////////////////////////////
//
//  The following constants configure your plug-in to behave well under
//  FileMaker applications and to be well recognized on both Mac OS and
//  Windows operating systems. It is recommended to always set all these
//  contants up to contain accurate information about your plug-in.
//

#define PLUGIN_ID_STRING        "FMTF"

#define PLUGIN_NAME             "FMTFoundation"
#define PLUGIN_VERSION          "1.0"

#define PLUGIN_VERSION_MAJOR    1
#define PLUGIN_VERSION_MINOR    0
#define PLUGIN_VERSION_BUILD    0
#define PLUGIN_VERSION_DEV      0

#define PLUGIN_INFO             "fmtechs foundation plug-in"

#define COMPANY_NAME            "fmtechs"
#define COPY_STATEMENT          "Copyright ©2014 by " COMPANY_NAME

#define WANT_FUNC_IDLE               0           // set to 1 if you want idle-processing
#define WANT_PREFERENCES        0           // set to 1 if you want to handle preferences


////////////////////////////////////////////////////////////////////////////////
//
//  Mac OS X constants
//
/////////////////////////////////////////////////////////////////////////////
//
//  Define Mac OS X specific information for you plug-in below.
//

#define PLUGIN_BUNDLE_FUNC_ID    "com.fmtechs.foundationplugin"


////////////////////////////////////////////////////////////////////////////////
//
//  Windows constants
//
/////////////////////////////////////////////////////////////////////////////
//
//  Define Windows specific information for you plug-in below.
//

#define FILE_NAME_PC        "FMTFoundation.fmx"  // A hint for user who accidentally renames your plug-in

#define PLUGIN_INFO_PC      "This file is a FileMaker applications plug-in. To make this file effective, drop it into FileMaker's Extensions folder and launch the FileMaker application."

#define COMPANY_URL         "http://www.fmtechs.com"


////////////////////////////////////////////////////////////////////////////////
//
//  Function definitions
//
/////////////////////////////////////////////////////////////////////////////
//
//  Do not modify the following enum, use the defined constants to configure
//  minimum and maximum parameters required for each function
//  (the FUNCTION_X_PARAMS macros)
//

#ifndef __PLIST__

#endif // __PLIST__

/////////////////////////////////////////////////////////////////////////////
//
//  Define your external functions here. For each of your functions,
//  uncomment the first commented block of defines, and set up the
//  function's name, flags, parameters and prototype.
//
#define FUNCTION_FLAG_DISPLAY_IN_ALL_DIALOGS fmx::ExprEnv::kDisplayInAllDialogs
#define FUNCTION_FLAG_ENABLE_ALL fmx::ExprEnv::kMayEvaluateOnServer | fmx::ExprEnv::kDisplayInAllDialogs

#define GET_VERSION_FUNC	"FMT_GetVersion()"
#define GET_VERSION_FUNC_ID		100
#define GET_OS_NAME_FUNC	"FMT_GetOSName"
#define GET_OS_NAME_FUNC_ID		101
#define GET_OS_VERSION_FUNC	"FMT_GetOSVersion"
#define GET_OS_VERSION_FUNC_ID	102
#define GET_COMPUTER_NAME_FUNC	"FMT_GetComputerName"
#define GET_COMPUTER_NAME_FUNC_ID	103
#define GET_PROCESSORS_NUM_FUNC	"FMT_GetProcessorNumber"
#define GET_PROCESSORS_NUM_FUNC_ID	104
//#define GET_USERNAME_FUNC	"FMT_GetUsername()"
//#define GET_USERNAME_FUNC_ID	105

#define GET_ENVIRONMENT_VAR_FUNC	"FMT_GetEnvironmentVariable({name})"
#define GET_ENVIRONMENT_VAR_FUNC_ID	110

#define HAS_ENVIRONMENT_VAR_FUNC	"FMT_HasEnvironmentVariable({key})"
#define HAS_ENVIRONMENT_VAR_FUNC_ID	111

#define SET_ENVIRONMENT_VAR_FUNC	"FMT_SetEnvironmentVariable({key}, {value})"
#define SET_ENVIRONMENT_VAR_FUNC_ID	112

#define CREATE_FOLDER_FUNC	"FMT_CreateFolder({folderpath})"
#define CREATE_FOLDER_FUNC_ID	121

#define DELETE_FILE_FUNC	"FMT_DeleteFileOrFolder({path})"
#define DELETE_FILE_FUNC_ID		122

#define RENAME_FILE_FUNC	"FMT_RenameFileOrFolder({path}, {newname})"
#define RENAME_FILE_FUNC_ID		123

#define MOVE_FILE_FUNC     "FMT_MoveFileOrFolderTo({path}, {topath})"
#define MOVE_FILE_FUNC_ID		124

#define COPY_FILE_FUNC      "FMT_CopyFileOrFolderTo({path} , {topath} )"
#define COPY_FILE_FUNC_ID		125

#define FILE_EXISTS_FUNC    "FMT_FileExists({path})"
#define FILE_EXISTS_FUNC_ID		126

#define LIST_FILES_FUNC		"FMT_ListFilesInDirectory({folderpath}, {mode})"
#define LIST_FILES_FUNC_ID	127

#define ZIP_FILE_FUNC		"FMT_ZipFileOrFolder({file/folder}, {targetFile})"
#define ZIP_FILE_FUNC_ID	128

#define UNZIP_FILE_FUNC     "FMT_UnzipFile({file/folder}, {targetFolder})"
#define UNZIP_FILE_FUNC_ID		129

#define READ_TEXT_FILE_FUNC "FMT_ReadTextFile({file})"
#define READ_TEXT_FILE_FUNC_ID	130

#define WRITE_TEXT_FILE_FUNC "FMT_WriteTextFile({file}, {value})"
#define WRITE_TEXT_FILE_FUNC_ID	131

#define READ_BINARY_FILE_FUNC	"FMT_ReadBinaryFile({file})"
#define READ_BINARY_FILE_FUNC_ID	132

#define GET_TEMP_FOLDER_FUNC	"FMT_GetTempFolder"
#define GET_TEMP_FOLDER_FUNC_ID	133

#define GET_CURRENT_DIRECTOTY_FUNC	"FMT_GetCurrentDirectory"
#define GET_CURRENT_DIRECTOTY_FUNC_ID	134

#define GET_USER_HOME_DIRECTORY_FUNC	"FMT_GetUserHomeDirectory"
#define GET_USER_HOME_DIRECTORY_FUNC_ID	135

#define IS_DIRECTORY_FUNC	"FMT_IsDirectory( {path} )"
#define IS_DIRECTORY_FUNC_ID	136

#define GET_FILE_SIZE_FUNC	"FMT_GetFileSize( {file} )"
#define GET_FILE_SIZE_FUNC_ID	137
// ===========================================================

#define SET_INI_FUNC	"FMT_SetIniValue({inifile}, {key})"
#define SET_INI_FUNC_ID		150
#define	GET_INI_FUNC	"FMT_GetIniValue({inifile}, {key}, {value})"
#define	GET_INI_FUNC_ID		151
#define	DELETE_INI_FUNC	"FMT_DeleteIniKey({inifile}, {key})"
#define	DELETE_INI_FUNC_ID	152

#define SET_XML_CONFIG_FUNC	"FMT_SetPropertyValueFromXmlConfigFile({xmlfile}, {key}, {value})"
#define SET_XML_CONFIG_FUNC_ID	153
#define GET_XML_CONFIG_FUNC	"FMT_GetPropertyValueFromXmlConfigFile({xmlfile}, {key})"
#define GET_XML_CONFIG_FUNC_ID	154
#define DELETE_XML_CONFIG_FUNC	"FMT_DeletePropertyXmlConfigFile({inifile}, {key})"
#define DELETE_XML_CONFIG_FUNC_ID	155

#define GET_REGISTRY_VALUE_FUNC      "FMT_ReadRegistryKey({uri}, {file})"
#define GET_REGISTRY_VALUE_FUNC_ID	156
#define SET_REGISTRY_VALUE_FUNC      "FMT_WriteRegistryKey({uri}, {file})"
#define SET_REGISTRY_VALUE_FUNC_ID	157
#define DELETE_REGISTRY_VALUE_FUNC   "FMT_DeleteRegistryKey({uri}, {file})"
#define DELETE_REGISTRY_VALUE_FUNC_ID	158

#define HTTP_DOWNLOAD_FUNC	"FMT_HttpDownloadFile({uri}, {file})"
#define HTTP_DOWNLOAD_FUNC_ID	170

#define HTTP_UPLOAD_FUNC	"FMT_HttpUploadFile({uri}, {file})"
#define HTTP_UPLOAD_FUNC_ID		171

#define ENCRYPT_BASE64_FUNC "FMT_EncryptBase64({uri}, {file})"
#define ENCRYPT_BASE64_FUNC_ID	180

#define DECRYPT_BASE64_FUNC "FMT_DecryptBase64({uri}, {file})"
#define DECRYPT_BASE64_FUNC_ID	181

#define OPEN_FILE_IN_DEFAULT_APP_FUNC      "FMT_OpenFileOrFolderInDefaultApp( {path} )"
#define OPEN_FILE_IN_DEFAULT_APP_FUNC_ID	221

#define EXECUTE_COMMAND_LINE_FUNC      "FMT_ExecuteCommandLine({command}, {argument})"
#define EXECUTE_COMMAND_LINE_FUNC_ID		222

#endif // __FMTConfig_h__
