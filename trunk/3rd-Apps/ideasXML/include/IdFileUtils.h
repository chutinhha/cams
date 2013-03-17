#include "IdGlobals.h"
#include "IdTimeUtils.h"

//#include <afxwin.h>
#include <iostream>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>


#define FALSE 0
#define TRUE  1

class CIdFileUtils
{
public:
	CIdFileUtils()
	{
	}

	~CIdFileUtils()
	{
	}

	static void CIdFileUtils::IsFileExists(const char *FilePath, bool *a_exists)
	{

	   #ifdef WIN32
		struct __stat64 FileStat;
		int result;
		*a_exists = FALSE;
		result = _stat64( FilePath, &FileStat );
		if( result != 0 )
			perror( "Problem getting information" );
		else
		{
			*a_exists = TRUE;
/*
			g_tLog.WriteLogWithoutTimeStamp("#################################################");
			g_tLog.WriteLogWithoutTimeStamp("File size     : %ld\n", FileStat.st_size);
			g_tLog.WriteLogWithoutTimeStamp("Drive         : %c:\n", FileStat.st_dev);
			g_tLog.WriteLogWithoutTimeStamp("Time modified : %s", _ctime64( &FileStat.st_mtime ) );
			g_tLog.WriteLogWithoutTimeStamp("#################################################");
*/
		
		}
	   #else
		struct stat FileStat;
		*a_exists = FALSE;
		if ( stat ( FilePath, &FileStat ) == 0 )
		{
			*a_exists = TRUE;
		}
	   #endif 
	}

	static bool ideas_debug_enabled(void)
	{
		static bool g_debug_enabled = FALSE;
		static bool g_debug_enabled_is_cached = FALSE;

		if (!g_debug_enabled_is_cached)
    		{
      			IsFileExists("../logs/.debug", &g_debug_enabled);
      			g_debug_enabled_is_cached = TRUE;
		}
  		return g_debug_enabled;
    	}
};
