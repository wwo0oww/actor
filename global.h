#ifndef _GLOBAL_H
#define _GLOBAL_H
#if defined(__linux__)
# include <pthread.h>
// Linuxϵͳ
#elif defined(_WIN32)
// Windowsϵͳ
#include <windows.h>
#endif

#if defined(__linux__)
typedef pthread_mutex_t WMutex
#define CLOSE_MUTEX(Mutex) pthread_mutex_destroy(&Mutex)
#define INIT_MUTEX(Mutex) pthread_mutex_init(&Mutex,NULL)
// Linuxϵͳ
#elif defined(_WIN32)
// Windowsϵͳ
typedef HANDLE WMutex;
#define CLOSE_MUTEX(Mutex) CloseHandle(Mutex)
#define INIT_MUTEX(Mutex) (Mutex = CreateMutex(NULL, false, NULL))
#define LOCK(Mutex) (Mutex = CreateMutex(NULL, false, NULL))
#define UNLOCK(Mutex) ReleaseMutex(Mutex)

#endif

#define DEBUG ture

#ifndef HANDLE
typedef void* HANDLE;
#endif

#ifndef NULL
typedef 0 NULL;
#endif

#define GETLAST(Index,Total) (Index-1 + Total) % Total

#ifndef	SafeDelete
#define	SafeDelete(p)	if (p) { delete p; p = NULL; }
#endif

#ifndef	SafeDeleteArray
#define	SafeDeleteArray(p)	if (p) { delete [] p; p = NULL; }
#endif

#ifndef	SafeFree
#define	SafeFree(p)	if (p) { free(p); p = NULL; }
#endif

#ifndef SafeRelease
#define	SafeRelease(p)	if (p){ p->Release(); p = NULL; }
#endif

#define ArrayCount(a)	(sizeof(a) / sizeof((a)[0]))

#endif