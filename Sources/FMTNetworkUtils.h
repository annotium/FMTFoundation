
#ifndef FMTFoundation_FMTNetworkUtils_h
#define FMTFoundation_FMTNetworkUtils_h

#if __cplusplus
extern "C" {
#endif
    // http post/get
	extern FMX_PROC(fmx::errcode) FMT_HttpDownloadFile(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	//extern FMX_PROC(fmx::errcode) FMT_HttpUploadFile(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
#if __cplusplus
}
#endif


#endif
