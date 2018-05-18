


#include "stdafx.h"
#include "Version.h"
#include <string>
#include <stdio.h>


/* These functions expect OPENTHREADS_MAJOR_VERSION,
 * OPENTHREADS_MINOR_VERSION, OPENTHREADS_PATCH_VERSION, and
 * OPENTHREADS_SOVERSION to be defined by the build system.
*/

extern "C" {

const char* OpenThreadsGetVersion()
{
    static char OpenThreads_version[256];
    static int OpenThreads_version_init = 1;
    if (OpenThreads_version_init)
    {
        sprintf(OpenThreads_version,"%d.%d.%d",OPENTHREADS_MAJOR_VERSION,OPENTHREADS_MINOR_VERSION,OPENTHREADS_PATCH_VERSION);
        OpenThreads_version_init = 0;
    }
    
    return OpenThreads_version;
}

const char* OpenThreadsGetSOVersion()
{
    static char OpenThreads_soversion[32];
    static int OpenThreads_soversion_init = 1;
    if (OpenThreads_soversion_init)
    {
        sprintf(OpenThreads_soversion,"%d",OPENTHREADS_SOVERSION);
        OpenThreads_soversion_init = 0;
    }
    
    return OpenThreads_soversion;
}

const char* OpenThreadsGetLibraryName()
{
    return "OpenThreads Library";
}

}
