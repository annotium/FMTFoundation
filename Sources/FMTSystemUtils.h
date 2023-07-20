
#ifndef FMTFoundation_FMTSystemUtils_h
#define FMTFoundation_FMTSystemUtils_h

#if __cplusplus
extern "C" {
#endif

    // version
	extern FMX_PROC(fmx::errcode) FMT_GetVersion(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_GetOSName(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_GetOSVersion(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_GetComputerName(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_GetProcessorNumber(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_GetUserName(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_GetEnvironmentVariable(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_HasEnvironmentVariable(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_SetEnvironmentVariable(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
    // command line
	extern FMX_PROC(fmx::errcode) FMT_ExecuteCommandLine(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
    
    // base 64
	extern FMX_PROC(fmx::errcode) FMT_EncryptBase64(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_DecryptBase64(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);

    
#if __cplusplus
}
#endif

#endif
