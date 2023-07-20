#ifndef __FM_SETUP_H__
#define __FM_SETUP_H__
  
  //  Define macros we can later use regardless of what environment is being used
  //  It is strongly recommended that you use the macros defined here for any conditional
  //  compilation, instead of the environment-dependent equivalents
  
  #if defined( __GNUC__ )

    #undef __CONSTANT_CFSTRINGS__	
	
	//  Requires Prefix file that includes Carbon/Carbon.h
#ifndef FMX_MAC_TARGET
    #define FMX_MAC_TARGET  TARGET_OS_MAC
#endif
    #define FMX_WIN_TARGET  TARGET_OS_WIN32
    
    //  XCode projects have the FMPLUGIN_DEBUG macro defined in preprocessor
    //  options for the Development build style
    
    // #define FMPLUGIN_DEBUG  1

  #elif defined(__MWERKS__)

    //  Set the proper FMX_PLATFORM_TARGET macros
    #define FMX_MAC_TARGET  __MACOS__
    #define FMX_WIN_TARGET  __INTEL__
    
    //  Assume debug build if there is a marker in the project window debug column
    #if __option(sym)
        #define FMPLUGIN_DEBUG 1
    #else
        #define FMPLUGIN_DEBUG 0
    #endif

  #elif defined( _MSC_VER )
    #define FMX_MAC_TARGET  0
    #define FMX_WIN_TARGET  1
    #ifdef _DEBUG
        #define FMPLUGIN_DEBUG  1
    #else
        #define FMPLUGIN_DEBUG  0
    #endif // _DEBUG

  #endif // Compiler specific directives

  
//  Include FMAPI headers so that they don't have to be included manually
#ifndef _h_Extern_
#include "FMPlugInSDK/Headers/FMWrapper/FMXExtern.h"
#endif
#ifndef _h_Types_
#include "FMPlugInSDK/Headers/FMWrapper/FMXTypes.h"
#endif
#ifndef _h_FixPt_
#include "FMPlugInSDK/Headers/FMWrapper/FMXFixPt.h"
#endif
#ifndef _h_Text_
#include "FMPlugInSDK/Headers/FMWrapper/FMXText.h"
#endif
#ifndef _h_Data_
#include "FMPlugInSDK/Headers/FMWrapper/FMXData.h"
#endif
#ifndef _h_DBCalcEngine_
#include "FMPlugInSDK/Headers/FMWrapper/FMXCalcEngine.h"
#endif
#ifndef _h_TextStyle_
#include "FMPlugInSDK/Headers/FMWrapper/FMXTextStyle.h"
#endif
#ifndef _h_BinaryData_
#include "FMPlugInSDK/Headers/FMWrapper/FMXBinaryData.h"
#endif
#ifndef _h_DateTime_
#include "FMPlugInSDK/Headers/FMWrapper/FMXDateTime.h"
#endif

#include <vector>
#include "FMTConfig.h"
  //  Define a more convenient FMX_UniChar type which matches the UniChar type's case
 typedef FMX_Unichar FMX_UniChar;
  

  #if __cplusplus
    extern "C" {
  #endif
  
    #if FMX_WIN_TARGET
        HINSTANCE   GetPluginInstance();
    #endif

    #if FMX_MAC_TARGET
        CFBundleRef GetPluginBundle();
    #endif
    
  #if __cplusplus
    }
  #endif  
  
	extern std::vector<unsigned short> gRegisteredFunctions;

#endif // __FM_SETUP_H__