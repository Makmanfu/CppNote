
#ifndef OPENTHREADS_VERSION
#define OPENTHREADS_VERSION 1

#include "Config.h"

extern "C" {

#define OPENTHREADS_MAJOR_VERSION 3
#define OPENTHREADS_MINOR_VERSION 3
#define OPENTHREADS_PATCH_VERSION 0
#define OPENTHREADS_SOVERSION 20

/** OpenThreadsGetVersion() returns the library version number.
 * Numbering convention : OpenThreads-1.0 will return 1.0 from OpenThreadsGetVersion. */
extern OPENTHREAD_EXPORT_DIRECTIVE const char* OpenThreadsGetVersion();

/** The OpenThreadsGetSOVersion() method returns the OpenSceneGraph soversion number. */
extern OPENTHREAD_EXPORT_DIRECTIVE const char* OpenThreadsGetSOVersion();

/** The OpenThreadsGetLibraryName() method returns the library name in human-friendly form. */
extern OPENTHREAD_EXPORT_DIRECTIVE const char* OpenThreadsGetLibraryName();

}

#endif
