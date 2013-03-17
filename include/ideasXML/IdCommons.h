#ifndef __IDCOMMONS_H__
#define __IDCOMMONS_H__

#define CHECK_DELETE(X)		if(X){delete(X); X=NULL;}
#define BUFF_4K   	    4096
#define BUFF_1K   	    1024
#define BUFLEN_512   	512
#define BUFLEN_256		256
#define BUFLEN_128		128
#define BUFLEN_64		64

#ifdef WIN32
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

	typedef pthread_mutex_t		MUTEX_OBJECT;
	typedef pthread_t			THREAD_REF;
	typedef long				THREAD_ATTR;

	//GURU: added conditional variable support
	typedef pthread_cond_t		cond_var_t;
	typedef pthread_condattr_t	cond_attrib_t;
#endif

// Platform specific definitions
#ifdef WIN32
	

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
#else
	#include <pthread.h>
	#include <unistd.h>

	#define CMD_DELETEFILE		"rm -f %s"
	#define CMD_MOVEFILE		"mv %s %s"
	#define POPEN(x,y)			popen(x,y)
	#define PCLOSE(x)			pclose(x)
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

#endif //__IDCOMMONS_H__
