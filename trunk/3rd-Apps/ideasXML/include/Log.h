#ifndef __LOG_H__
#define __LOG_H__

#include <fstream>
#include <string>

using namespace std;

#if defined(WIN32) && !defined(__CYGWIN__)
	#include "afxwin.h"
	typedef CRITICAL_SECTION	MUTEX_OBJECT;
#else
	typedef pthread_mutex_t		MUTEX_OBJECT;
#endif

using namespace std;

class CKtiThread;

//Class contains the variable you want to protect and a CRITICAL_SECTION data structure
class CLog {
public:
	CLog();
	~CLog();
	bool Open(string strLogPath="", bool bAbsolutePath=false);
	void Close();
	void LogStackTrace(CKtiThread* pThread);
	bool Rollover(const char* szNewDate);
	bool WriteLog(const char* szFormat, ...);
	bool WriteLogWithoutTimeStamp(const char* szFormat, ...);
	bool WriteLogWithBlog(string& strBlogContent, const char* szFormat, ...);

	bool m_bOpen;
	bool m_bConsoleLog;
	bool m_bRolloverLog;
	std::ofstream m_hFile;

	string m_strLogPath;
	string m_strDate;
	string m_strDeletableLog;
	MUTEX_OBJECT m_stSync;
};

extern CLog g_tLog;

#endif //__LOG_H__
