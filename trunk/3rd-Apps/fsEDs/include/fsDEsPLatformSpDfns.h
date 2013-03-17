/***************************************************************************
 ** File            :   fsDEsPLatformSpDfns.h
 ** 
 ** Description     :   common globals that can be used. Made compatible with 
 **                     Linux and windows
 ** History
 **     Created     :   Nov 7 2012
 **     Modified    :   Nov 14 2012
 **
 ** Notes           :
 **
 ** ToDo            :
 **
 ***************************************************************************/

#ifndef __IDPLATFORMSPDFNS_H__
#define __IDPLATFORMSPDFNS_H__

#if defined(WIN32) && !defined(__CYGWIN__)
	#include <process.h>
	#include <direct.h>

	#define	CLEAR		        system ("cls");
	#define GETCHE		        _getche();
	#define GETCH		        _getch();
	#define SLEEP(x)		    Sleep(x)
    #define KBHIT               _kbhit();
#else
	#include <pthread.h>
	#include <unistd.h>
	#include <sys/time.h>
	#include <sys/errno.h>
	#include <time.h>

	#define CMD_DELETEFILE		"rm -f %s"

    #define	CLEAR	            system ("clear");
	#define GETCHE		        getche();
	#define GETCH		        getch();

	#define SLEEP(x)		    if(x<1000){usleep(x*1000);}else{sleep(x/1000);}
	#define TIMEOUT			    ETIMEDOUT
    #define KBHIT               kbhit();
#endif

// Platform specific definitions
#if defined(WIN32) && !defined(__CYGWIN__)
	#define CMD_DELETEFILE		"del %s"
	#define CMD_MOVEFILE		"move %s %s"
	#define POPEN(x,y)			_popen(x,y)
	#define PCLOSE(x)			_pclose(x)
	#define SNPRINTF			_snprintf
	#define GETCWD				_getcwd
	#define ssize_t				size_t

    // fsDEs_s def's
    // path's are relative from fsDEs_s root directory
    #define ABOUT_CAMS_PATH     "\\config\\core\\About_fsDEs.txt"
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
	
    #define SNPRINTF			snprintf
	#define GETCWD				getcwd
	#define _strnicmp 			strncasecmp
	#define CLOSE(x)			close(x)
	#define PATH_SEP			"/"
#endif

#endif //__IDPLATFORMSPDFNS_H__
