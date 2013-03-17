/******************************************************************************
* File Name			:	IdStringUtils.h
*
* Author			:	Satheesh Kumar
*
* Project			:	Ideas Framework
*
* Module			:	Ideas Agent
*
* Purpose			:	Utility functions for string manipulation
*
* Copy Right		:	Copyright Kanrad(I)Pvt.Ltd.
*
* Notes				:
*
* History			:
*		Created		:   2/15/05
*		Version		:	1.0.0.0
*
*
******************************************************************************/

#ifndef __IDSTRINGUTILS_H__
#define __IDSTRINGUTILS_H__

#include <string>
#include <fstream>
#include <stdio.h>
#include <locale.h>

#if defined(WIN32) && !defined(__CYGWIN__)
	#include "IdRegExpr.h"
#else
	#include <regex.h>
#endif

using namespace std;

class CIdStringUtils
{
public:

	/******************************************************************************
	* Function name	  : CIdStringUtils::Trim
	* Description  	  : Removes starting and trailing white space characters.
	* Arguments		  : @strInput[IN]   - Input string to be trimmed
	*					@strResult[OUT] - Modified string without white spaces
	*
	* Return       	  : None
	******************************************************************************/

	static void Trim(const string& strInput, string& strResult)
	{
		char const* szDelims = " \t\r\n";
		strResult = strInput;

		// trim leading whitespace
		string::size_type  uNotWhitePos = strResult.find_first_not_of(szDelims);
		strResult.erase(0, uNotWhitePos);

		// trim trailing whitespace
		uNotWhitePos = strResult.find_last_not_of(szDelims);
		strResult.erase(uNotWhitePos + 1);
	}

	/******************************************************************************
	* Function name	  : CIdStringUtils::InsertEscapeChars
	* Description  	  : Inserts escape sequences for Engine-reserved characters.
	* Arguments		  : @strInput[IN]   - Input string to be escaped
	*					@strResult[OUT] - Modified string with escape sequences
	*
	* Return       	  : None
	******************************************************************************/

	static void InsertEscapeChars(const string& strInput, string& strResult)
	{
		strResult = "";
		char* szIn = (char* )strInput.data();
		int i=0;
		//while(szIn[i] != CHAR_NUL)
		for(;i<strInput.size();)
		{
			if(szIn[i] == CHAR_BSLASH)
				strResult += "\\\\";
			else if(szIn[i] == CHAR_COMMA)
				strResult += "\\,";
			else if(szIn[i] == CHAR_EQUAL)
				strResult += "\\=";
			else if(szIn[i] == CHAR_OPEN)
				strResult += "\\{";
			else if(szIn[i] == CHAR_CLOSE)
				strResult += "\\}";
			else if(szIn[i] == CHAR_PARAM)
				strResult += "\\$";
			else if(szIn[i] == CHAR_OUTPUT)
				strResult += "\\?";
			else if(szIn[i] == CHAR_FILEREF)
				strResult += "\\~";
			else if(szIn[i] == CHAR_PLUGINVAR)
				strResult += "\\@";
			else if(szIn[i] == CHAR_SYSVAR)
				strResult += "\\%";
			else
				strResult += szIn[i];
			i++;
		}
	}

	/********************************************************************************
	* Function name	  : CIdStringUtils::GetListValue
	* Description  	  : Get list field at the specified position.
	* Arguments		  : @strList[IN]   - Serialized list fields delimited by ',' char
	*					@nPos[IN] - Postion of the field tobe extracted
	*					@strValue[OUT] - Field value at the specified position.
	*
	* Return       	  : bool
	*********************************************************************************/

	static bool GetListField(const string& strList,
							int nPos, string& strValue)
	{
		strValue = "";

		if(strList.empty())
			return false;

		char* szIn = (char* )strList.data();
		int i=0;
		int nIndex = 0;
		string strTemp = "";
		while(szIn[i] != '\0')
		{
			if((szIn[i] == '\\') && (szIn[i+1] == '\\')) {
					strTemp += "\\"; i += 2;
			}
			else if((szIn[i] == '\\') && (szIn[i+1] == ',')) {
				strTemp += ","; i += 2;
			}
			else if(szIn[i] == ',')
			{
				if(nIndex == nPos) {
					strValue = strTemp; return true;
				}
				else {
					strTemp = ""; ++i; ++nIndex;
				}

			}
			else {
				strTemp += szIn[i]; ++i;
			}
		}
		return false;
	}

	static int CountListFields(const string& strList)
	{
		int nCount = 0;

		if(strList.empty())
			return nCount;

		char* szIn = (char* )strList.data();
		int i=0;
		while(szIn[i] != '\0')
		{
			if((szIn[i] == '\\') && (szIn[i+1] == '\\'))
				i += 2;
			else if((szIn[i] == '\\') && (szIn[i+1] == ','))
				i += 2;
			else if(szIn[i] == ',') {
				++i; ++nCount;
			}
			else
				++i;
		}
		return nCount;
	}

	static void GetStringTokens(const string& strList,
                      vector<string>& vTokens,
                      const string& strDelim = " ")
	{
		string::size_type nLastPos = strList.find_first_not_of(strDelim, 0);
		string::size_type nPos     = strList.find_first_of(strDelim, nLastPos);

		while (string::npos != nPos || string::npos != nLastPos)
		{
			vTokens.push_back(strList.substr(nLastPos, nPos - nLastPos));
			nLastPos = strList.find_first_not_of(strDelim, nPos);
			nPos	 = strList.find_first_of(strDelim, nLastPos);
		}
	}

#if defined(WIN32) && !defined(__CYGWIN__)
	static int FindRegex(string szInput, string szRegex)
	{
		CIdRegExpr tRegex;

		tRegex.compile(szRegex.c_str());
		if(tRegex.search(szInput.c_str()) == REGEXPR_NOT_FOUND)
			return 0;

		return 1;
	}

	static int FindRegex(ifstream tFile, string szRegex)
	{
		CIdRegExpr tRegex;

		tRegex.compile(szRegex.c_str());

		char szBuf[1024] = {0};
		string strFile;
		while (! tFile.eof() )
		{
			tFile.getline (szBuf,1024);
			strFile.append(szBuf);
		}
		if(tRegex.search(strFile.c_str()) == REGEXPR_NOT_FOUND)
			return 0;

		return 1;
	}

#else // For *nix platforms only

	static int FindRegex(string szInput, string szRegex)
	{
		regex_t re;
		int nFound = 0;
		if (regcomp(&re, szRegex.c_str(), REG_EXTENDED|REG_NOSUB) != 0)
		{

			return -1;
		}
		if (regexec(&re, szInput.c_str(), 0, NULL, 0) == 0)
			nFound = 1;

		regfree(&re);
		return nFound;
	}

	static int FindRegex(ifstream& tFile, string szRegex)
	{
		regex_t re;
		int nFound = 0;
		if (regcomp(&re, szRegex.c_str(), REG_EXTENDED|REG_NOSUB) != 0)
		{

			return -1;
		}
		char szBuf[1024] = {0};
		string strFile;
		while (! tFile.eof() )
		{
			tFile.getline (szBuf,1024);
			strFile.append(szBuf);
		}
		if (regexec(&re, strFile.c_str(), 0, NULL, 0) == 0)
			nFound = 1;

		regfree(&re);
		return nFound;
	}

	static int GetPatternMatch(string szInput, string szRegex, string& strMatch)
	{
		regex_t re;
		int nFound = 0;
		setlocale(LC_ALL, "");
		if (regcomp(&re, szRegex.c_str(), REG_EXTENDED) != 0)
		{

			return -1;
		}
		regmatch_t regm[50];
		int i=1;
		int nOffset = 0;
		while (regexec(&re, szInput.c_str() + nOffset, i, regm, 0) == 0)
		{
			if(i>1)
				strMatch += "[SP]";
			else
				nFound = 1;

			int nLen = regm[0].rm_eo-regm[0].rm_so;
			strMatch.append(szInput.substr(nOffset+regm[0].rm_so, nLen));
			nOffset += (regm[0].rm_so + nLen);
			i++;
		}
		regfree(&re);
		return nFound;
	}
#endif

};

#endif //__IDSTRINGUTILS_H__

