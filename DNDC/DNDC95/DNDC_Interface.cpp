#include "DNDC_Interface.h"
#include "RunPaths.h"

///////////////////////////////////////////////////////////////////////////////
Site_BaseInformation* GetNewSitePtr()
{
    return new Site_BaseInformation();
}
///////////////////////////////////////////////////////////////////////////////
void DeleteSitePtr( Site_BaseInformation* siteData )
{
    delete siteData;
}
///////////////////////////////////////////////////////////////////////////////
void SetLibPath( const char* libPath )
{
    LIBRARY = libPath;
}
///////////////////////////////////////////////////////////////////////////////
void SetRootDirPaths(
    const char* rootPath,
    const char* outputPath,
    const char* interPath,
    const char* inputPath,
    const char* intermanPath )
{
    ROOTDIR = rootPath;
    OUTPUT = outputPath;
    INTER = interPath;
    INPUTS = inputPath;
    INTERMANAGE = intermanPath;
}
///////////////////////////////////////////////////////////////////////////////

