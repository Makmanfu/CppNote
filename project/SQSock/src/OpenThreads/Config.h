

#ifndef _OPENTHREADS_CONFIG
#define _OPENTHREADS_CONFIG

/* #undef _OPENTHREADS_ATOMIC_USE_GCC_BUILTINS */
/* #undef _OPENTHREADS_ATOMIC_USE_MIPOSPRO_BUILTINS */
/* #undef _OPENTHREADS_ATOMIC_USE_SUN */
#define _OPENTHREADS_ATOMIC_USE_WIN32_INTERLOCKED
/* #undef _OPENTHREADS_ATOMIC_USE_BSD_ATOMIC */
/* #undef _OPENTHREADS_ATOMIC_USE_MUTEX */
/* #undef OT_LIBRARY_STATIC */

#ifndef WIN32
#define OPENTHREAD_EXPORT_DIRECTIVE
#else
#if defined( OT_LIBRARY_STATIC )
#define OPENTHREAD_EXPORT_DIRECTIVE
#elif defined( OPENTHREADS_EXPORTS )
#define OPENTHREAD_EXPORT_DIRECTIVE //__declspec(dllexport)
#else
#define OPENTHREAD_EXPORT_DIRECTIVE //__declspec(dllimport)
#endif
#endif




#endif
