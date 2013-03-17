#ifndef __IdPlatformSpDfns_H__
#define __IdPlatformSpDfns_H__

#if defined(WIN32) && !defined(__CYGWIN__)
	#include <afxwin.h>
	#include <atlbase.h>
	#include <process.h>
	#include <direct.h>

	#define DELETE_MUTEX(x) 	if(x) ::DeleteCriticalSection(x)
	#define INIT_MUTEX(x)		::InitializeCriticalSection(x)
	#define LOCK_MUTEX(x)		::EnterCriticalSection(x)
	#define UNLOCK_MUTEX(x)		::LeaveCriticalSection(x)
	#define SLEEP(x)			Sleep(x)
	#define THREAD_FUNCTION		DWORD WINAPI
	#define DETACH_THREAD(x)	if(x != NULL) CloseHandle(x)
	#define INIT_COND(x)		::CreateEvent(x, false, false, NULL)
	#define DELETE_COND(x)		if(x) ::CloseHandle(x)
	#define WAIT_COND(x)		::WaitForSingleObject(x, INFINITE)
	#define WAIT_COND_TO(x,y)	::WaitForSingleObject(x, y)
	#define SET_COND(x)			::SetEvent(x)
	#define RESET_COND(x)		::ResetEvent(x)
	#define EXIT_THREAD			::ExitThread(0)
	#define KILL_THREAD(x)		::TerminateThread(x,0)
	#define TIMEOUT				WAIT_TIMEOUT
    #define KBHIT               _kbhit()

	typedef CRITICAL_SECTION	MUTEX_OBJECT;
	typedef HANDLE				THREAD_REF;
	typedef DWORD				THREAD_ATTR;

	//GURU: added conditional variable support
	typedef HANDLE				cond_var_t;
#else
	#include <pthread.h>
	#include <unistd.h>
	#include <sys/time.h>
	#include <sys/errno.h>
	#include <time.h>

	#define CMD_DELETEFILE		"rm -f %s"
	#define DELETE_MUTEX(x) 	if(x) pthread_mutex_destroy(x)
	#define INIT_MUTEX(x)		pthread_mutex_init(x, NULL)
	#define LOCK_MUTEX(x)		pthread_mutex_lock(x)
	#define UNLOCK_MUTEX(x)		pthread_mutex_unlock(x)

	#define SLEEP(x)			if(x<1000){usleep(x*1000);}else{sleep(x/1000);}
	#define THREAD_FUNCTION		void*
	#define DETACH_THREAD(x)	pthread_detach(x)

	#define INIT_COND(x)		pthread_cond_init(x, NULL)
	#define DELETE_COND(x)		if(x) pthread_cond_destroy(x)
	#define WAIT_COND(x,y)		pthread_cond_wait(x,y)
	#define WAIT_COND_TO(x,y,z)	pthread_cond_timedwait(x,y,z)
	#define SET_COND(x)			pthread_cond_signal(&x)

	#define EXIT_THREAD			pthread_exit((void**)0)
	#define KILL_THREAD(x)		pthread_cancel(x)
	#define TIMEOUT				ETIMEDOUT
    #define KBHIT               kbhit()

	typedef pthread_mutex_t		MUTEX_OBJECT;
	typedef pthread_t			THREAD_REF;
	typedef long				THREAD_ATTR;

	//GURU: added conditional variable support
	typedef pthread_cond_t		cond_var_t;
	typedef pthread_condattr_t	cond_attrib_t;
#endif

// Platform specific definitions
#if defined(WIN32) && !defined(__CYGWIN__)
	#define CMD_DELETEFILE		"del %s"
	#define CMD_MOVEFILE		"move %s %s"
	#define POPEN(x,y)			_popen(x,y)
	#define PCLOSE(x)			_pclose(x)
	#define SNPRINTF			_snprintf
	#define GETCWD				_getcwd
	#define CLOSE(x)			closesocket(x)
	#define PATH_SEP			"\\"
	#define ssize_t				size_t

	#define FPATH_SETTINGS		"\\config\\settings.xml"
	#define FPATH_JOBS			"\\config\\jobs.xml"
	#define FPATH_WORKERS		"\\config\\workers.xml"
	#define FPATH_PLUGINS		"\\config\\plugins.xml"
	#define FPATH_CRONTAB		"\\config\\crontab"
	#define DPATH_LOG			"\\logs\\"
	#define DPATH_CONFIG		"\\config\\"
	#define DPATH_WORK			"\\work\\"

	#define FPATH_TEMP          "\\temp"
	#define FPATH_PKT		    "..\\Transport\\Backup\\%u.pkt"
	#define FPATH_TRALOG		"\\Transport\\Active\\TransportLog"
	#define FPATH_FILELOG       "\\Transport\\Active\\MasterLog"
	#define FPATH_FIRSTBOOT     "\\firstboot"

    // CAMS def's
    // path's are relative from cams root directory
    #define ABOUT_CAMS_PATH     "\\config\\core\\About_CAMS.txt"
    #define CAMS_SETTINGS       "\\config\\core\\settings.xml"
#else
	#include <pthread.h>
	#include <unistd.h>
	
	#ifdef __CYGWIN__
		#define POPEN(x,y)			POpen2(x,y)
		#define PCLOSE(x)			PClose2(x)
	#else
		#define POPEN(x,y)			popen(x,y)
		#define PCLOSE(x)			pclose(x)
	#endif
	
	#define CMD_DELETEFILE		"rm -f %s"
	#define CMD_MOVEFILE		"mv %s %s"
	#define SNPRINTF			snprintf
	#define GETCWD				getcwd
	#define _strnicmp 			strncasecmp
	#define CLOSE(x)			close(x)
	#define PATH_SEP			"/"

	#define FPATH_SETTINGS		"/config/settings.xml"
	#define FPATH_JOBS			"/config/jobs.xml"
	#define FPATH_WORKERS		"/config/workers.xml"
	#define FPATH_PLUGINS		"/config/plugins.xml"
	#define FPATH_CRONTAB		"/config/crontab"
	#define DPATH_LOG			"/logs/"
	#define DPATH_CONFIG		"/config/"
	#define DPATH_WORK			"/work/"

	#define FPATH_TEMP          "/temp"
	#define FPATH_PKT		    "../Transport/Backup/%u.pkt"
	#define FPATH_TRALOG		"/Transport/Active/TransportLog"
	#define FPATH_FILELOG       "/Transport/Active/MasterLog"
	#define FPATH_FIRSTBOOT     "/firstboot"
#endif

#if defined (WIN32) || defined (__CYGWIN__)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <afxwin.h>
typedef unsigned __int64 longlong;
#else
typedef long long longlong;
typedef long long unsigned int ULONGLONG;
#endif

#endif
