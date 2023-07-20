//
//  FMTZipUtils.cpp
//  FMTFoundation
//
//  Created by HoangLe on 7/10/14.
//
//

#include "FMPlugin.h"
#include "FMTSetup.h"
#include "FMTZipUtils.h"
#include <Poco/StringTokenizer.h>
#include <Poco/Zip/Compress.h>
#include <Poco/Zip/Decompress.h>
#include <Poco/Delegate.h>
#include <Poco/File.h>
#include <Poco/Path.h>
#include "FMTUtilities.h"
#include <fstream>

using namespace Poco::Zip;


FMX_PROC(fmx::errcode) FMT_ZipFileOrFolder(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
    FMTUtilities::info("Invoke function: FMT_ZipFileOrFolder");
    fmx::errcode err = kNoError;
    if (dataVect.Size() < 2) {
        FMTUtilities::info("FMT_ZipFileOrFolder failed: Missing parameters");
        FMTUtilities::SetResult(kErrorParameterMissing, result);
        err = kErrorParameterMissing;
    }
    
    std::string inputFiles= FMTUtilities::getParameterAsUTF8String(dataVect, 0);
    if (inputFiles == "") {
        FMTUtilities::error("FMT_ZipFileOrFolder failed: Input files list must not be empty and separate by comma");
        FMTUtilities::SetResult(kInvalidPathError, result);
        return kErrorUnknown;
    }
    
    std::string outputZipFile= FMTUtilities::getParameterAsUTF8String(dataVect, 1);
    if (outputZipFile == "") {
        FMTUtilities::error("FMT_ZipFileOrFolder failed: Output files must not be empty!");
        FMTUtilities::SetResult(kInvalidPathError, result);
        return kErrorUnknown;
    }
    
    try {
        std::ofstream ofs(outputZipFile.c_str(), std::ios::binary);
        Poco::Zip::Compress compress(ofs, true);
        Poco::StringTokenizer tokenizer(inputFiles, ";");
        std::vector<std::string>::const_iterator iter = tokenizer.begin();
        for (; iter != tokenizer.end(); ++iter) {
            Poco::File fsFile(*iter);
            Poco::Path fsPath(fsFile.path());
            
            if (!fsFile.exists()) {
                FMTUtilities::error("FMT_ZipFileOrFolder failed: Input file '" + *iter + "' does not exist!");
                continue;
            }
            
            if (fsFile.isDirectory()) {
                compress.addRecursive(fsPath, Poco::Zip::ZipCommon::CL_MAXIMUM);
            }
            else {
                compress.addFile(fsPath, fsPath.getFileName());
            }
        }
        compress.close();
    }
    catch (Poco::Exception ex) {
        std::string message;
        Poco::format(message,
                     "FMT_ZipFileOrFolder failed: Exception occurred while zipping file/folder %s to %s. Error: %s (code = %d)",
                     inputFiles, outputZipFile, ex.message(), ex.code());
        FMTUtilities::error(message);
        FMTUtilities::SetResult(ex.code(), result);
        err = kErrorUnknown;
    }
    
    return err;
}

void onDecompressError(const void* pSender, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string>& info)
{
    // inform user about error
}

FMX_PROC(fmx::errcode) FMT_UnzipFile(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
    FMTUtilities::info("Invoke function: FMT_UnzipFile");
    fmx::errcode err = kNoError;
    if (dataVect.Size() < 2) {
        FMTUtilities::info("FMT_UnzipFile failed: Missing parameters");
        FMTUtilities::SetResult(kErrorParameterMissing, result);
        err = kErrorParameterMissing;
    }
    
    std::string inputZipFile = FMTUtilities::getParameterAsUTF8String(dataVect, 0);
    if (inputZipFile == "") {
        FMTUtilities::error("FMT_UnzipFile failed: Input file must not be empty");
        FMTUtilities::SetResult(kInvalidPathError, result);
        return kErrorUnknown;
    }
    Poco::File file(inputZipFile);
    if (!file.exists()) {
        FMTUtilities::info("FMT_UnzipFile failed: Input file '" + inputZipFile + "' does not exist!");
        FMTUtilities::SetResult(kFileNotFoundError, result);
        return err;
    }
    
    std::string outputPathStr = FMTUtilities::getParameterAsUTF8String(dataVect, 1);
    Poco::Path outputPath = Poco::Path(outputPathStr);
    
    try {
        std::ifstream ifs(inputZipFile.c_str(), std::ios::binary);
        Poco::Zip::Decompress decompress(ifs, outputPath);
        //decompress.EError += Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &onDecompressError);
        decompress.decompressAllFiles();
        //decompress.EError -= Poco::Delegate<ZipTest, std::pair<const Poco::Zip::ZipLocalFileHeader, const std::string> >(this, &onDecompressError);
    }
    catch (Poco::Exception ex) {
        std::string message;
        Poco::format(message,
                     "FMT_UnzipFile failed: Exception occurred while unzipping file %s to %s. Error: %s (code = %d)",
                     inputZipFile, outputPathStr, ex.message(), ex.code());
        FMTUtilities::error(message);
        FMTUtilities::SetResult(ex.code(), result);
        err = kErrorUnknown;
    }
    
    return err;
}