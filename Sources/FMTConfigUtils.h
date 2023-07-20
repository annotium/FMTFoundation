//
//  FMTConfigUtils.h
//  FMTFoundation
//
//  Created by HoangLe on 7/10/14.
//
//

#ifndef FMTFoundation_FMTConfigUtils_h
#define FMTFoundation_FMTConfigUtils_h

#if __cplusplus
extern "C" {
#endif
    // ini file
	extern FMX_PROC(fmx::errcode) FMT_SetIniValue(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_GetIniValue(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_DeleteIniKey(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
    
	// xml config file
	extern FMX_PROC(fmx::errcode) FMT_GetPropertyValueFromXmlConfigFile(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_SetPropertyValueFromXmlConfigFile(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_DeletePropertyXmlConfigFile(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
    
#if FMX_WIN_TARGET
	// registry
	extern FMX_PROC(fmx::errcode) FMT_DeleteRegistryKey(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_ReadRegistryKey(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_WriteRegistryKey(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
#endif
    
#if __cplusplus
}
#endif


#endif
