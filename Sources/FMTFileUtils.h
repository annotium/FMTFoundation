
#ifndef __FMT_FILE_UTILS_H__
#define __FMT_FILE_UTILS_H__

#if __cplusplus
extern "C" {
#endif
    // file systems
	extern FMX_PROC(fmx::errcode) FMT_CreateFolder(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_DeleteFileOrFolder(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_RenameFileOrFolder(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_MoveFileOrFolderTo(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_CopyFileOrFolderTo(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_FileExists(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_ListFilesInDirectory(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_GetTempFolder(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_GetCurrentDirectory(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_GetUserHomeDirectory(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_IsDirectory(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_GetFileSize(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	//extern FMX_PROC(fmx::errcode) FMT_FindFile(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
    // text file
	extern FMX_PROC(fmx::errcode) FMT_ReadTextFile(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
	extern FMX_PROC(fmx::errcode) FMT_WriteTextFile(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
    
	// binary file
	extern FMX_PROC(fmx::errcode) FMT_ReadBinaryFile(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
    
    extern FMX_PROC(fmx::errcode) FMT_OpenFileOrFolderInDefaultApp(short, const fmx::ExprEnv&, const fmx::DataVect&, fmx::Data&);
#if __cplusplus
}
#endif


#endif
