
#ifndef FMTFoundation_FMTZipUtils_h
#define FMTFoundation_FMTZipUtils_h

#if __cplusplus
extern "C" {
#endif
    // zip unzip
    extern FMX_PROC(fmx::errcode) FMT_ZipFileOrFolder(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
    extern FMX_PROC(fmx::errcode) FMT_UnzipFile(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
#if __cplusplus
}
#endif

#endif
