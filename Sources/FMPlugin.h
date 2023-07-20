#ifndef __FMPPLUGIN_H__
#define __FMPPLUGIN_H__

#define PLUGIN_NAME_ID          1
#define PLUGIN_PREFERENCES_ID   2
#define PLUGIN_VERSION_ID       3

#define FUNCTION_OFFSET       100

#if defined( __GNUC__ )

	#define FMX_MAC_TARGET	1
	/*
	 Unloadable modules can not use the new style constant strings that Apple introduced
	 in 10.4. So we have to turn if off in the headers and the project file with
	 */
	#undef __CONSTANT_CFSTRINGS__

	#define USER_PREFERENCES_DOMAIN L"com.fmtechs.fmtfoundation"

#elif defined( _MSC_VER )
	#define FMX_WIN_TARGET	1
	//#define _WIN32_WINNT _WIN32_WINNT_MAXVER
	#define USER_PREFERENCES_DOMAIN L"Software\\fmtechs\\FMTFoundation"

#endif 

/////////////////////////////////////////////////////////////////////////////
//
//  We are including this header in FMTConfig.h which is included by
//  FMTemplate.plc. The following definitions are for use in code only,
//  so we are excluding them from Plist definition from.
//

#ifndef __PLIST__

  #if __cplusplus
    extern "C" {
  #endif
  
	 enum FunctionErrorCodes {
		kErrorUnknown = -1,
		kNoError = 0,
		kErrorParameterMissing = 2
	}; 

	enum FileFunctionResultErrors
	{
		kFileSuccess = 0,
		kNoSuchFileOrDirectoryError = -1000,
		kOverrideExistFileError = -1001,
		kFileNotFoundError = -1002,
		kInvalidPathError = -1003,
		kInvalidInput = -1004,
		kInvalidRegistryKey = -1005,
        kFileOrFolderAlreadyExisted
	};

	enum {
		kBE_OptionsStringID = 1
	}; 

	enum {
		kBE_NumericConstantOffset = 1000,
		kBE_ButtonOffset = 1000,
		kBE_MessageDigestTypeOffset = 2000,
		kBE_FileTypeOffset = 3000,
		kBE_GetStringMaxBufferSize = 4096
	};
  
  #if __cplusplus
    }
  #endif

#endif //__PLIST__

/////////////////////////////////////////////////////////////////////////////


 
#endif // __FMPlugin_h__
