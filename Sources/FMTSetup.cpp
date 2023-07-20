//  FMTConfig.h is required somewhere as it configures the FM Template
#include "FMTConfig.h"
#include "FMTSetup.h"
#include "FMTUtilities.h"
#include "FMTFileUtils.h"
#include "FMTNetworkUtils.h"
#include "FMTSystemUtils.h"
#include "FMTConfigUtils.h"
#include "FMTZipUtils.h"

#include <Poco/DateTime.h>

#if FMX_WIN_TARGET
    #include <Windows.h>
#endif

#define if_error(cond, label)  if(cond) goto label

// ====== Forward Declaration ==============================================================================

static FMX_Int32 Do_PluginInit(FMX_Int16 version);
static void      Do_PluginShutdown(void);

#if WANT_FUNC_IDLE
static void         Do_PluginIdle(FMX_IdleLevel idleLevel);
#endif //WANT_FUNC_IDLE

#if WANT_PREFERENCES
static void         Do_PluginPrefs();
#endif //WANT_PREFERENCES

static void         Do_PluginGetString();

//static void       ProvideConfigString(FMX_UniChar* ioBuffer, FMX_UInt32 iBufferSize);
static void			ProvideConfigString(FMX_UniChar* ioBuffer, FMX_UInt32 iBufferSize );
static void         ProvideNameString(FMX_UniChar* ioBuffer, FMX_UInt32 iBufferSize);
static void         ProvidePreferencesString(FMX_UniChar* ioBuffer, FMX_UInt32 iBufferSize);

FMX_Boolean  RegisterExternalFunctions();
void         UnRegisterExternalFunctions();

// ====== Global Variables =================================================================================

    FMX_ExternCallPtr   gFMX_ExternCallPtr = NULL;
    FMX_Boolean         gPlugInInitialized = 0;
    FMX_Boolean         gPrefsOpen = 0;
	std::vector<unsigned short> gRegisteredFunctions;

void FMX_ENTRYPT FMExternCallProc(FMX_ExternCallPtr pb)
{
    // Setup global defined in FMxExtern.h
    gFMX_ExternCallPtr = pb;

    // Message dispatcher
    switch (gFMX_ExternCallPtr->whichCall) 
    {
        case kFMXT_Init:
            gFMX_ExternCallPtr->result = Do_PluginInit(gFMX_ExternCallPtr->extnVersion);
            break;
            
        case kFMXT_Shutdown:
            Do_PluginShutdown();
            break;
            
        case kFMXT_GetString:
            Do_PluginGetString();
            break;

        #if WANT_FUNC_IDLE
            case kFMXT_Idle:
                Do_PluginIdle(gFMX_ExternCallPtr->parm1);
                break;
        #endif //WANT_FUNC_IDLE
        
        #if WANT_PREFERENCES
            case kFMXT_DoAppPreferences:
                Do_PluginPrefs();
                break;
        #endif //WANT_PREFERENCES
        
    };// switch whichCall

} // FMExternCallProc

// ====== Handlers =========================================================================================

static FMX_Int32 Do_PluginInit(FMX_Int16 version) 
{
    Poco::DateTime now;
    if (now.year() >= 2014 && now.month() >= 8 && now.day() >= 13) {
        return kBadExtnVersion;
    }
    
	FMTUtilities::initLogging();
    // Check the app API version
    if ((version < k70ExtnVersion) || (version > kMaxExtnVersion)) {
        // This version of FileMaker is not supported; let FileMaker disable this 
        // plug-in and report the problem to the user.
        return kBadExtnVersion;
    }

    if(RegisterExternalFunctions()) {
        gPlugInInitialized = 1;
        return kCurrentExtnVersion;
    }
    
    return kDoNotEnable;
    
} // Do_PluginInit


static void Do_PluginShutdown(void) 
{
    UnRegisterExternalFunctions();
	gPlugInInitialized = 0;
} // Do_PluginShutdown

#if WANT_FUNC_IDLE
    void Do_PluginIdle(FMX_IdleLevel idleLevel) 
    {
        //  eliminate the race condition we would be called when Startup is not yet done!
        if(gPlugInInitialized) {
            if(idleLevel == kFMXT_UserIdle)
                SafeIdle();
            else if (idleLevel == kFMXT_Unsafe)
                UnsafeIdle();
            else 
                SemiSafeIdle(idleLevel);
        }
    } // Do_PluginIdle
#endif //WANT_FUNC_IDLE

#if WANT_PREFERENCES    
    static void Do_PluginPrefs()
    {
        //  Avoid opening multiple instances of the preferences dialog
        if (!gPrefsOpen) {
            gPrefsOpen = 1;
            Preferences();
            gPrefsOpen = 0;
        }
    }
#endif //WANT_PREFERENCES


static void Do_PluginGetString()
{
    FMX_Strings   stringType = gFMX_ExternCallPtr->parm1;
   // FMX_UInt32     windowsLanguageID   = gFMX_ExternCallPtr->parm2;
    FMX_UInt32   outStringBufferSize = (FMX_UInt32)gFMX_ExternCallPtr->parm3;
    FMX_Unichar*  outStringBuffer = (FMX_Unichar*)(gFMX_ExternCallPtr->result);
         
    switch(stringType)
    {
        case kFMXT_OptionsStr:
            ProvideConfigString(outStringBuffer, outStringBufferSize);
            break;
        
        case kFMXT_NameStr:
            ProvideNameString(outStringBuffer, outStringBufferSize);
            break;
            
        case kFMXT_AppConfigStr:
            ProvidePreferencesString(outStringBuffer, outStringBufferSize);
            break;
    }
}

#if FMX_WIN_TARGET
    HINSTANCE   GetPluginInstance()
    {
        return (HINSTANCE)gFMX_ExternCallPtr->instanceID;
    }
#endif


#if FMX_MAC_TARGET
    CFBundleRef GetPluginBundle()
    {
        return (CFBundleRef)gFMX_ExternCallPtr->instanceID;
    }
#endif


void ProvideConfigString(FMX_UniChar* ioBuffer, FMX_UInt32 iBufferSize)
{
    #pragma unused(iBufferSize)
    
    //  Provide plug-in config string here.
    
    ioBuffer[ 0] = ((unsigned char*)PLUGIN_ID_STRING)[0];
    ioBuffer[ 1] = ((unsigned char*)PLUGIN_ID_STRING)[1];
    ioBuffer[ 2] = ((unsigned char*)PLUGIN_ID_STRING)[2];
    ioBuffer[ 3] = ((unsigned char*)PLUGIN_ID_STRING)[3];
    ioBuffer[ 4] = '1'; 

    #if WANT_PREFERENCES
        ioBuffer[ 5] = 'Y';
    #else
        ioBuffer[ 5] = 'n';
    #endif

    ioBuffer[ 6] = 'n';
    ioBuffer[ 7] = 'Y';

    #if WANT_FUNC_IDLE
        ioBuffer[ 8] = 'Y';
    #else
        ioBuffer[ 8] = 'n';
    #endif

    ioBuffer[ 9] = 'n';
    ioBuffer[10] = 'n';
    ioBuffer[11] = 0;
}


void ProvideNameString(FMX_Unichar* ioBuffer, FMX_UInt32 iBufferSize)
{
    //  Provide plug-in name string here.
    FMTUtilities::ReadString(ioBuffer, iBufferSize, PLUGIN_NAME_ID);
}


void ProvidePreferencesString(FMX_Unichar* ioBuffer, FMX_UInt32 iBufferSize)
{
    //  Provide plug-in name string here.
    FMTUtilities::ReadString(ioBuffer, iBufferSize, PLUGIN_PREFERENCES_ID);
}

FMX_Boolean RegisterExternalFunctions() 
{   
	FMTUtilities::debug("Registering functions");
    //FMX_Boolean enable = FALSE;
	gRegisteredFunctions.clear();
	FMTUtilities::registerFunction(GET_VERSION_FUNC_ID, GET_VERSION_FUNC, 0, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_GetVersion);
	
	FMTUtilities::registerFunction(GET_OS_NAME_FUNC_ID, GET_OS_NAME_FUNC, 0, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_GetOSName);
	FMTUtilities::registerFunction(GET_OS_VERSION_FUNC_ID, GET_OS_VERSION_FUNC, 0, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_GetOSVersion);
	FMTUtilities::registerFunction(GET_COMPUTER_NAME_FUNC_ID, GET_COMPUTER_NAME_FUNC, 0, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_GetComputerName);
	FMTUtilities::registerFunction(GET_PROCESSORS_NUM_FUNC_ID, GET_PROCESSORS_NUM_FUNC, 0, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_GetProcessorNumber);
	FMTUtilities::registerFunction(GET_ENVIRONMENT_VAR_FUNC_ID, GET_ENVIRONMENT_VAR_FUNC, 1, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_GetEnvironmentVariable);
	FMTUtilities::registerFunction(HAS_ENVIRONMENT_VAR_FUNC_ID, HAS_ENVIRONMENT_VAR_FUNC, 1, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_HasEnvironmentVariable);
	FMTUtilities::registerFunction(SET_ENVIRONMENT_VAR_FUNC_ID, SET_ENVIRONMENT_VAR_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_SetEnvironmentVariable);

	FMTUtilities::registerFunction(CREATE_FOLDER_FUNC_ID, CREATE_FOLDER_FUNC, 1, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_CreateFolder);
	FMTUtilities::registerFunction(DELETE_FILE_FUNC_ID, DELETE_FILE_FUNC, 1, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_DeleteFileOrFolder);
	FMTUtilities::registerFunction(RENAME_FILE_FUNC_ID, RENAME_FILE_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_RenameFileOrFolder);
	FMTUtilities::registerFunction(MOVE_FILE_FUNC_ID, MOVE_FILE_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_MoveFileOrFolderTo);
	FMTUtilities::registerFunction(COPY_FILE_FUNC_ID, COPY_FILE_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_CopyFileOrFolderTo);
	FMTUtilities::registerFunction(FILE_EXISTS_FUNC_ID, FILE_EXISTS_FUNC, 1, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_FileExists);
	FMTUtilities::registerFunction(LIST_FILES_FUNC_ID, LIST_FILES_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_ListFilesInDirectory);
	FMTUtilities::registerFunction(ZIP_FILE_FUNC_ID, ZIP_FILE_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_ZipFileOrFolder);
	FMTUtilities::registerFunction(UNZIP_FILE_FUNC_ID, UNZIP_FILE_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_UnzipFile);
	FMTUtilities::registerFunction(GET_TEMP_FOLDER_FUNC_ID, GET_TEMP_FOLDER_FUNC, 0, 0, FUNCTION_FLAG_ENABLE_ALL, FMT_GetTempFolder);
	FMTUtilities::registerFunction(GET_CURRENT_DIRECTOTY_FUNC_ID, GET_CURRENT_DIRECTOTY_FUNC, 0, 0, FUNCTION_FLAG_ENABLE_ALL, FMT_GetCurrentDirectory);
	FMTUtilities::registerFunction(GET_USER_HOME_DIRECTORY_FUNC_ID, GET_USER_HOME_DIRECTORY_FUNC, 0, 0, FUNCTION_FLAG_ENABLE_ALL, FMT_GetUserHomeDirectory);
	FMTUtilities::registerFunction(IS_DIRECTORY_FUNC_ID, IS_DIRECTORY_FUNC, 1, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_IsDirectory);
	FMTUtilities::registerFunction(GET_FILE_SIZE_FUNC_ID, GET_FILE_SIZE_FUNC, 1, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_GetFileSize);

	FMTUtilities::registerFunction(READ_TEXT_FILE_FUNC_ID, READ_TEXT_FILE_FUNC, 1, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_ReadTextFile);
	FMTUtilities::registerFunction(WRITE_TEXT_FILE_FUNC_ID, WRITE_TEXT_FILE_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_WriteTextFile);
	FMTUtilities::registerFunction(READ_BINARY_FILE_FUNC_ID, READ_BINARY_FILE_FUNC, 1, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_ReadBinaryFile);
	
	FMTUtilities::registerFunction(GET_INI_FUNC_ID, GET_INI_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_GetIniValue);
	FMTUtilities::registerFunction(SET_INI_FUNC_ID, SET_INI_FUNC, 3, 3, FUNCTION_FLAG_ENABLE_ALL, FMT_SetIniValue);
	FMTUtilities::registerFunction(DELETE_INI_FUNC_ID, DELETE_INI_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_DeleteIniKey);
	FMTUtilities::registerFunction(GET_XML_CONFIG_FUNC_ID, GET_XML_CONFIG_FUNC, 3, 3, FUNCTION_FLAG_ENABLE_ALL, FMT_GetPropertyValueFromXmlConfigFile);
	FMTUtilities::registerFunction(SET_XML_CONFIG_FUNC_ID, SET_XML_CONFIG_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_SetPropertyValueFromXmlConfigFile);
	FMTUtilities::registerFunction(DELETE_XML_CONFIG_FUNC_ID, DELETE_XML_CONFIG_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_DeletePropertyXmlConfigFile);
	//FMTUtilities::registerFunction(GET_REGISTRY_VALUE_FUNC_ID, GET_REGISTRY_VALUE_FUNC, 3, 3, FUNCTION_FLAG_ENABLE_ALL, FMT_GetPropertyValueFromXmlConfigFile);
	//FMTUtilities::registerFunction(SET_REGISTRY_VALUE_FUNC_ID, SET_REGISTRY_VALUE_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_SetPropertyValueFromXmlConfigFile);
	//FMTUtilities::registerFunction(DELETE_REGISTRY_VALUE_FUNC_ID, DELETE_REGISTRY_VALUE_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_DeletePropertyXmlConfigFile);

	FMTUtilities::registerFunction(HTTP_DOWNLOAD_FUNC_ID, HTTP_DOWNLOAD_FUNC, 2, 4, FUNCTION_FLAG_ENABLE_ALL, FMT_HttpDownloadFile);
	//FMTUtilities::registerFunction(HTTP_UPLOAD_FUNC_ID, HTTP_UPLOAD_FUNC, 2, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_HttpUploadFile);
	FMTUtilities::registerFunction(ENCRYPT_BASE64_FUNC_ID, ENCRYPT_BASE64_FUNC, 1, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_EncryptBase64);
	FMTUtilities::registerFunction(DECRYPT_BASE64_FUNC_ID, DECRYPT_BASE64_FUNC, 1, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_DecryptBase64);
	FMTUtilities::registerFunction(OPEN_FILE_IN_DEFAULT_APP_FUNC_ID, OPEN_FILE_IN_DEFAULT_APP_FUNC, 1, 1, FUNCTION_FLAG_ENABLE_ALL, FMT_OpenFileOrFolderInDefaultApp);
	FMTUtilities::registerFunction(EXECUTE_COMMAND_LINE_FUNC_ID, EXECUTE_COMMAND_LINE_FUNC, 1, 2, FUNCTION_FLAG_ENABLE_ALL, FMT_ExecuteCommandLine);
	
	return TRUE;
}

void UnRegisterExternalFunctions()
{
	std::string errMessage;
    // Unregister plug-in functions
    fmx::QuadCharAutoPtr pluginID(PLUGIN_ID_STRING[0], PLUGIN_ID_STRING[1], PLUGIN_ID_STRING[2], PLUGIN_ID_STRING[3]);
	for (std::size_t i = 0; i < gRegisteredFunctions.size(); ++i) {
		unsigned int id = gRegisteredFunctions[i];
		fmx::errcode err = fmx::ExprEnv::UnRegisterExternalFunction(*pluginID, id);
		if (err) {
			Poco::format(errMessage, "Error occurred while unregistering function id: %hu. Error code: %hd", id, err); 
			FMTUtilities::error(errMessage);
		}
	}

	gRegisteredFunctions.clear();
}