#ifndef __CAMSGLOBAL__
#define __CAMSGLOBAL__

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
	#define _WIN32_WINNT 0x0502
#endif

#include <string>
// CAMS specific return status that has to be returned by all the functions

const int CAMS_SUCCESS		=	1;
const int CAMS_OK			=	0;
const int CAMS_FAILURE		=	-1;

const string CAMS_NOT_PRESENT_DB	=	"CAMS_NOT_PRESENT";
const string CAMS_SUCCESS_DB		=	"CAMS_SUCCESS";
const string CAMS_FAIL_DB			=	"CAMS_FAILURE";

#ifdef WIN32
	#define	CLEAR		system ("cls");
	//#define	SLEEP(X)	Sleep (X);			// X in milliseconds
	#define GETCHE		_getche();
	#define GETCH		_getch();
#else
	#define	CLEAR	system ("clear");
	#define GETCHE		getche();
	#define GETCH		getch();
#endif

#define ENABLE_CONSOLE		g_tLog.m_bConsoleLog = true;
#define DISABLE_CONSOLE		g_tLog.m_bConsoleLog = false;

// db specific constant values
// tbluserdetails

// user type
const int const USER_TYPE_ADMIN     =   1;
const int const USER_TYPE_GENERAL   =   2;

// status
const int const STATUS_ACTIVE       =   1;
const int const STATUS_BLOCKED      =  -1;
const int const STATUS_SUSPENDED    =   0;

// Delete Mode
const int const DELETE_MODE         =   0;

// dept
const int const DEPT                =   1;

//configuration files
#define     CAMS_CONFIG_SETTINGS           "../../config/core/cams_settings.xml"
 
#endif //__CAMSGLOBAL_
