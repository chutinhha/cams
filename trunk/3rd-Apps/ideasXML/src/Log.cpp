#include "IdGlobals.h"
#include "IdTimeUtils.h"

#include <iostream>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "KtiThread.h"
#include "Log.h"

#if defined(WIN32) && !defined(__CYGWIN__)
	#define vsnprintf _vsnprintf 
#endif

///\param string fname defaults to LOG_PATH
CLog::CLog()
{
	m_bOpen			= false;
	m_bConsoleLog	= false;
	m_strLogPath	= "";
	m_strDate		= "";
	m_strDeletableLog = "";
	m_bRolloverLog	= true;
	INIT_MUTEX(&m_stSync);
}


CLog::~CLog()
{
	if(m_bOpen)
	{
		if(m_hFile)
		{
			m_hFile.close();
			//m_hFile = NULL;
		}
		m_bOpen = false;
	}
	DELETE_MUTEX(&m_stSync);
}


bool CLog::Open(string strLogPath, bool bAbsolutePath)
{
	if(m_bOpen)
		return false;

	m_strLogPath = strLogPath;
	if(!bAbsolutePath)
	{
		char szBuff[BUFLEN_128] = {0};
		CIdTimeUtils::GetDateAsString(szBuff);
		strLogPath += szBuff;
		m_strDate = szBuff;
		strLogPath += ".log";
	}
	else
		m_bRolloverLog	= false;
	cout << "\nOpening Log:" << strLogPath.c_str();

	m_hFile.open(strLogPath.c_str(), std::ios::app);

	if(m_hFile.fail())
		return false;

	m_bOpen = true;
	return true;
}

void CLog::LogStackTrace(CKtiThread* pThread)
{
	char szDateTime[BUFLEN_64] = {0};
	LOCK_MUTEX(&m_stSync);
	CIdTimeUtils::GetDateTimeAsString(szDateTime);

	WriteLogWithoutTimeStamp("*************************************************************");
	WriteLogWithoutTimeStamp("***<%s>:Printing stack trace (Time:<%s>)***",
				pThread->m_szThreadName, szDateTime);
	WriteLogWithoutTimeStamp("*************************************************************/n");
	string strThreadLog = "";
	pThread->PrintStack(strThreadLog);
	WriteLogWithoutTimeStamp("%s\n", strThreadLog.c_str());
	WriteLogWithoutTimeStamp("=============================================================");
	UNLOCK_MUTEX(&m_stSync);
}

bool CLog::Rollover(const char* szNewDate)
{
	if(m_strDeletableLog != "")
	{
		#if defined(WIN32) && !defined(__CYGWIN__)
		_unlink(m_strDeletableLog.c_str());
		#else
		unlink(m_strDeletableLog.c_str());
		#endif
		m_strDeletableLog = "";
	}
	LOCK_MUTEX(&m_stSync);
	string strOldLog = m_strLogPath + m_strDate;
	strOldLog += ".log";
	Close();
	string strLogPath = m_strLogPath;
	strLogPath += szNewDate;
	strLogPath += ".log";
	m_strDate = szNewDate;

	m_hFile.open(strLogPath.c_str(), std::ios::app);

	if(m_hFile.fail())
	{
		m_hFile.open(strOldLog.c_str(), std::ios::app);
		m_bOpen = true;
		UNLOCK_MUTEX(&m_stSync);
		return false;
	}
	m_strDeletableLog = strOldLog;
	m_bOpen = true;
	UNLOCK_MUTEX(&m_stSync);
	return true;
}


void CLog::Close()
{
	if(m_bOpen)
	{
		if(m_hFile)
		{
			m_hFile.close();
			//m_hFile = NULL;
		}
		m_bOpen = false;
	}
}


bool CLog::WriteLog(const char* szFormat, ...)
{
	if(!m_bOpen)
		return false;

	va_list	alArgList;
	va_start(alArgList, szFormat);

	char szMessage[BUFF_4K] = {0};
	char szDateTime[BUFLEN_64] = {0};

	// This mutex should be taken here..strtok is not re-entrant !!
	LOCK_MUTEX(&m_stSync);
	CIdTimeUtils::GetDateTimeAsString(szDateTime);
	char* szDate = strtok(szDateTime, " ");

	if(m_bRolloverLog)
	{
		if(m_strDate.compare(szDate))
			Rollover(szDate);
	}

	SNPRINTF(szMessage,sizeof(szMessage),"[%s]\t", strtok(NULL, " "));

	vsnprintf(szMessage+strlen(szMessage),BUFF_4K - strlen(szMessage)-1,
			    szFormat,
			    alArgList);
	va_end(alArgList);
	strcat(szMessage, "\r\n");

	m_hFile << szMessage;

	m_hFile.flush();
	if(m_bConsoleLog)
		cout << szMessage;

	UNLOCK_MUTEX(&m_stSync);
	return true;

}

bool CLog::WriteLogWithoutTimeStamp(const char* szFormat, ...)
{
	if(!m_bOpen)
		return false;

	va_list	alArgList;
	va_start(alArgList, szFormat);

	if(m_bRolloverLog)
	{
		char szDate[BUFLEN_64] = {0};
		CIdTimeUtils::GetDateAsString(szDate);
		if(m_strDate.compare(szDate))
			Rollover(szDate);
	}

	char szMessage[BUFF_4K] = {0};
	vsnprintf(szMessage,BUFF_4K-1,
			    szFormat,
			    alArgList);
	va_end(alArgList);
	strcat(szMessage, "\r\n");

	LOCK_MUTEX(&m_stSync);
	m_hFile << szMessage;
	m_hFile.flush();

	if(m_bConsoleLog)
		cout << "\n" << szMessage;

	UNLOCK_MUTEX(&m_stSync);
	return true;
}

bool CLog::WriteLogWithBlog(string& strBlogContent, const char* szFormat, ...)
{
	if(!m_bOpen)
		return false;

	va_list	alArgList;
	va_start(alArgList, szFormat);

	if(m_bRolloverLog)
	{
		char szDate[BUFLEN_64*6] = {0};
		CIdTimeUtils::GetDateAsString(szDate);
		if(m_strDate.compare(szDate))
			Rollover(szDate);
	}

	char szMessage[BUFF_4K] = {0};
	vsnprintf(szMessage,BUFF_4K-1,
			    szFormat,
			    alArgList);
	va_end(alArgList);
	strcat(szMessage, "\r\n");

	LOCK_MUTEX(&m_stSync);
	m_hFile << szMessage;
	m_hFile << strBlogContent.c_str();
	m_hFile << "\r\n";
	m_hFile.flush();

	if(m_bConsoleLog)
	{
		cout << endl << szMessage << endl;
		cout << strBlogContent.c_str();
		cout << "\r\n";
	}
	UNLOCK_MUTEX(&m_stSync);
	return true;
}


