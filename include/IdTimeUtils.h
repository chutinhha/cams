#ifndef __IDTIMEUTILS_H__
#define __IDTIMEUTILS_H__

//#include <afxwin.h>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <time.h>

#if defined(WIN32) && !defined(__CYGWIN__)
	#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
	  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
	#else
	  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
	#endif

	struct Idtimeval
	{
		long    tv_sec;         // seconds
		long    tv_usec;        // and microseconds
	};
#else
	#include <sys/time.h>
#endif
using namespace std;

class CIdTimeUtils
{
public:

	static void GetTimeAsString(char* szBuffer, const char* szFormat="%H-%M-%S")
	{
		::strcpy(szBuffer, "");
		time_t tim;
		struct tm *now;

		time(&tim);
		now = localtime(&tim);
		strftime(szBuffer, 128, szFormat, now );
	}

	static void GetDateAsString(char* szBuffer, const char* szFormat="%m-%d-%Y")
	{
		::strcpy(szBuffer, "");
		time_t tim;
		struct tm *now;

		time(&tim);
		now = localtime(&tim);
		strftime(szBuffer, 128, szFormat, now );
	}

	static void GetDateTimeAsString(char* szBuffer, const char* szFormat="%m-%d-%Y %H:%M:%S")
	{
		::strcpy(szBuffer, "");
		time_t tim;
		struct tm *now;

		time(&tim);
		now = localtime(&tim);
		strftime(szBuffer, 128, szFormat, now );
	}

	static void GetTimeFields(int& nHour, int& nMinute, int& nSecond)
	{
		time_t tim;
		struct tm *now;
		time(&tim);
		now = localtime(&tim);
		nHour	= now->tm_hour;
		nMinute = now->tm_min;
		nSecond	= now->tm_sec;
	}

	static long GetTimestamp()
	{
		#if defined(WIN32) && !defined(__CYGWIN__)
		struct Idtimeval now;
		#else
		struct timeval now;
		#endif
		gettimeofday(&now, NULL);

		return (now.tv_sec);
	}

	string getNumber(string strDate, int pos )
	{
		string strGetNum = "";
		strGetNum = strDate.substr(pos,2);
		return strGetNum;
	}

	string getYear(string strDate, int pos)
	{
		string strGetNum = "";
		strGetNum = strDate.substr(pos,4);
		return strGetNum;
	}

	int chk2Digits(string strDate1, string strDate2)
	{
		int nDate1 = atoi(strDate1.c_str());
		int nDate2 = atoi(strDate2.c_str());
		if(nDate1 > nDate2)
			return 1;
		else if(nDate1 == nDate2)
			return 0;
		return -1;
	}

	//Compares two Dates(Format :: MM-DD-YYYY)
	int CompareDate(string strDate1, string strDate2)
	{
		if( chk2Digits( getYear(strDate1,6), getYear(strDate2,6)) == 1 )//Extracts Year
			return 1;
		else if(chk2Digits( getYear(strDate1,6), getYear(strDate2,6)) == 0)
		{
			if(chk2Digits( getNumber(strDate1,0), getNumber(strDate2,0)) == 1 )//Extracts Month
				return 1;
			else if(chk2Digits( getNumber(strDate1,0), getNumber(strDate2,0)) == 0 )
			{
				if( chk2Digits( getNumber(strDate1,3), getNumber(strDate2,3)) == 1 )//Extracts Date
					return 1;
				else if(chk2Digits( getNumber(strDate1,3), getNumber(strDate2,3)) == 0 )
					return 0;
				else
					return -1;
			}
			else
				return -1;
		}
		else
			return -1;
	}

	//Compares two Times(HH-MM-SS)
	bool CompareTime(string strTime1, string strTime2)
	{
		if( chk2Digits( getNumber(strTime1,0), getNumber(strTime2,0)) == 1 )//Extracts Hour
			return true;
		else if(chk2Digits( getNumber(strTime1,0), getNumber(strTime2,0)) == 0)
		{
			if(chk2Digits( getNumber(strTime1,3), getNumber(strTime2,3)) == 1 )//Extracts Minute
				return true;
			else if(chk2Digits( getNumber(strTime1,3), getNumber(strTime2,3)) == 0 )
			{
				if(chk2Digits( getNumber(strTime1,6), getNumber(strTime2,6)) == 1 ||
					chk2Digits( getNumber(strTime1,6), getNumber(strTime2,6)) == 0)//Extracts Seconds
					return true;
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}

	// Gets the Date part from string with format="MM-DD-YYYY_HH-MM-SS"
	string GetDate(string strDateTime)
	{
		string strGetDate;
		ostringstream GetDateStream(strGetDate);
		for(unsigned int uCount = 0; uCount < strDateTime.length(); uCount++)
		{
			if(strDateTime[uCount] == '_' )
			{
				break;
			}
			else
				GetDateStream<<strDateTime[uCount];
		}
		return GetDateStream.str();
	}

	// Gets the Time part from string with format="MM-DD-YYYY_HH-MM-SS"
	string GetTime(string strDateTime)
	{
		string strGetTime;
		ostringstream GetTimeStream(strGetTime);
		unsigned int uCount = 0;
		for(uCount = 0; uCount < strDateTime.length(); uCount++)
		{
			if(strDateTime[uCount] == '_')
				break;
		}
		for(unsigned int uCpCount = uCount+1; uCpCount < strDateTime.length(); uCpCount++)
		{
			GetTimeStream<<strDateTime[uCpCount];
		}
		return GetTimeStream.str();
	}

	//Compares the given DateTime
	bool CompareDateTime(string strDT1, string strDT2)
	{
		 int nDateVal = CompareDate( GetDate(strDT1), GetDate(strDT2) );//ist>=2nd
		 if( nDateVal == 0 ) //Date is greater than OR less than don't need to check time
		 {
			bool bReatTimeVal = CompareTime( GetTime(strDT1), GetTime(strDT2) );
			if( bReatTimeVal )
				return true;
			return false;
		 }
		 else if( nDateVal == 1 )
			return true;
		 return false;
	}

	//MM-DD-YYYY_HH-MM-SS
	time_t GetTimeinStruct(string strDate)
	{
		struct tm tmpTime;

		int nSec = atoi(getNumber(strDate, 17).c_str());
		int nMin = atoi(getNumber(strDate, 14).c_str());
		int nHrs = atoi(getNumber(strDate, 11).c_str());
		int nDay = atoi(getNumber(strDate, 3).c_str());
		int nMon = atoi(getNumber(strDate, 0).c_str());
		int nYear = atoi(getYear(strDate, 6).c_str());

		tmpTime.tm_sec = nSec;
		tmpTime.tm_min = nMin;
		tmpTime.tm_hour = nHrs;
		tmpTime.tm_mday = nDay;
		tmpTime.tm_mon = nMon - 1;
		tmpTime.tm_year = nYear - 1900;

		return mktime( &tmpTime );
	}

	int GetDiffDate(string strDate1, string strDate2)//1st>=2nd
	{
		if(strDate1 == strDate2)
			return 0;
		bool bRetVal = CompareDateTime( strDate1, strDate2 );
		if(bRetVal == true)
		{
			unsigned int uDay = 60*60*24;
			int nTotalDay = 0;
			time_t tDate1 = GetTimeinStruct(strDate1);
			time_t tDate2 = GetTimeinStruct(strDate2);
			double uTotalSecs = difftime(tDate1, tDate2);

			if( uTotalSecs < uDay )	//difference is less than seconds in a day
			{
				nTotalDay = 0;
				return nTotalDay;
			}
			else
			{
				#ifdef KTI_HPUX
				nTotalDay = (int) floor(uTotalSecs/uDay);
				#else
				nTotalDay = (int) floorf(uTotalSecs/uDay);
				#endif
				return nTotalDay;
			}
		}
		return -1;
	}

#if defined(WIN32) && !defined(__CYGWIN__)
	static int gettimeofday(struct Idtimeval *tv, void* tZone)
	{
		FILETIME ft;
		unsigned __int64 uTemp = 0;

		if (NULL != tv)
		{
			GetSystemTimeAsFileTime(&ft);

			uTemp |= ft.dwHighDateTime;
			uTemp <<= 32;
			uTemp |= ft.dwLowDateTime;

			/*converting file time to unix epoch*/
			uTemp -= DELTA_EPOCH_IN_MICROSECS;
			uTemp /= 10;  /*convert into microseconds*/
			tv->tv_sec = (long)(uTemp / 1000000UL);
			tv->tv_usec = (long)(uTemp % 1000000UL);
		}
		return 0;
	}
#endif

};

#endif //__IDTIMEUTILS_H__

