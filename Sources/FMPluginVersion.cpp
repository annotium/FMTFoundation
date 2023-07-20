#include "FMPlugin.h"
#include "FMTUtilities.h"
#include "FMTSetup.h"
#include "FMTSystemUtils.h"

#define BUFFER_SIZE 256

#pragma mark GetVersion
FMX_PROC(fmx::errcode) FMT_GetVersion(short funcId, const fmx::ExprEnv& environment, const fmx::DataVect& dataVect, fmx::Data& result)
{
	FMTUtilities::info("Invoke function: FMT_GetVersion");
    fmx::errcode        err = 0;
    FMX_Unichar         pluginVersion[BUFFER_SIZE];
    fmx::TextAutoPtr    resultText;
	fmx::LocaleAutoPtr locale;

	FMTUtilities::ReadString(pluginVersion, sizeof(pluginVersion)/sizeof(FMX_Unichar), PLUGIN_VERSION_ID);
    resultText->AssignUnicode(pluginVersion);
    err = result.SetAsText( *resultText, *locale);

    return err;
}

