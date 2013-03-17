// IdEntityMap.cpp: implementation of the CIdEntityMap class.
//
//////////////////////////////////////////////////////////////////////
#include "IdGlobals.h"
#include "IdEntityMap.h"
#include "IdStringUtils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIdEntityMap::CIdEntityMap() : m_bAddLock(false)
{
	m_cDelim = CHAR_DELIM;
}

CIdEntityMap::CIdEntityMap(char cDelim) : m_bAddLock(false)
{
	m_cDelim = cDelim;
}

CIdEntityMap::CIdEntityMap(const CIdEntityMap& tMap) : m_bAddLock(false)
{
	m_cDelim = tMap.m_cDelim;
}

CIdEntityMap::~CIdEntityMap()
{
}

CIdEntityMap& CIdEntityMap::operator= (const CIdEntityMap& tMap)
{
	return *this;
}

void CIdEntityMap::FlushOut(CIdEntityMap& tTargetMap)
{
	m_tIter  = m_tEntries.begin();
	for (; m_tIter != m_tEntries.end(); m_tIter++)
	{
		tTargetMap.SetEntityValue(m_tIter->first, m_tIter->second);
		//m_tEntries[m_tIter->first] = "";
	}
}


bool CIdEntityMap::AddNewEntity(string strKey, string strValue, bool bSetLock)
{
	if(m_bAddLock)
		return false;

	IDENTITYPAIR tKeyValuePair(strKey, strValue);
	m_tEntries.insert(tKeyValuePair);
	if(bSetLock)
		m_bAddLock = true;

	return true;
}

bool CIdEntityMap::SetEntityValue(string strKey, string strValue)
{
	if(m_tEntries.find(strKey.c_str()) == m_tEntries.end())
		return false;

	m_tEntries[strKey] = strValue;
	return true;
}

bool CIdEntityMap::GetEntityValue(string strKey, string& strValue)
{
	IDENTITYMAP::iterator tEntityIter;
	tEntityIter = m_tEntries.find(strKey);
	if(tEntityIter == m_tEntries.end())
	{
		return false;
	}
	strValue = (*tEntityIter).second;
	return true;
}

bool CIdEntityMap::GetFirstEntry(string& strKey, string& strValue)
{
	if(m_tEntries.size() == 0)
		return false;

	m_tIter  = m_tEntries.begin();
	strKey	 = m_tIter->first;
	strValue = m_tIter->second;
	m_tIter++;
	return true;
}

bool CIdEntityMap::GetNextEntry(string& strKey, string& strValue)
{
	if(m_tIter == m_tEntries.end())
		return false;

	strKey	 = m_tIter->first;
	strValue = m_tIter->second;
	m_tIter++;
	return true;
}

string CIdEntityMap::GetEntityKeys(bool bOnlyEmptyKeys)
{
	string strKeys = "";
	for (IDENTITYMAP::iterator i = m_tEntries.begin();
			i != m_tEntries.end(); i++)
	{
		bool bReqKey = true;
		if(bOnlyEmptyKeys)
		{
			if(!i->second.empty())
				bReqKey = false;
		}
		if(bReqKey)
		{
			if(!strKeys.empty())
				strKeys.append(",");
			strKeys.append(i->first);
		}
	}
	return strKeys;
}

void CIdEntityMap::RemoveEntity(string strKey)
{
	m_tEntries.erase(strKey);
}

void CIdEntityMap::RemoveAll()
{
	m_tEntries.erase(m_tEntries.begin(), m_tEntries.end());
	m_bAddLock = false;
}

void CIdEntityMap::EmptyAll()
{
//	IDENTITYMAP::iterator	m_tIter;
	for (IDENTITYMAP::iterator it = m_tEntries.begin(); it != m_tEntries.end(); ++it)
	{
		m_tEntries[it->first] = "";
	}
}

int CIdEntityMap::Size()
{
	return m_tEntries.size();
}

void CIdEntityMap::SetAddLock()
{
	m_bAddLock = true;
}

/******************************************************************************
* Function name	  : CIdEntityMap::Deserialize
* Description  	  : Deserialize key value pairs from the supplied string.
* Arguments		  : @strMap[IN] - Serialized map string
*					@nMode[IN] - Mode of extraction (NEW, OVERWRITE, KEEP_EXISTING)
*
* Return       	  : bool - TRUE on Success, else FALSE
******************************************************************************/

bool CIdEntityMap::Deserialize(string& strMap, int nMode)
{
 bool bResult = true;
 const char* szMapChars = strMap.c_str();
 if(nMode == NEW)
  RemoveAll();

 int i=0;
 string strKey = "";
 string strTemp = "";
 while(szMapChars[i] != '\0')
 {
  if(szMapChars[i] == '\\')
  {
   if(szMapChars[i+1] == '\\')
    strTemp += "\\\\";
   else if(szMapChars[i+1] == m_cDelim)
    strTemp += m_cDelim;
   else if(szMapChars[i+1] == '=')
    strTemp += "\\=";
   else if(szMapChars[i+1] == m_cDelim)
    strTemp = strTemp + "\\" + m_cDelim;
   else
   {
		strTemp += '\\';
		strTemp += szMapChars[i+1];//Maybe special character (@/$/% etc)
   }
   i+=2;
   if(szMapChars[i] != '\0')
    continue;
  }
  if((szMapChars[i] != m_cDelim) && (szMapChars[i] != '=') && (szMapChars[i] != '\0'))
  {
   strTemp += szMapChars[i];
   i++;
  }
  if(szMapChars[i] == '=')
  {
   if(strTemp.size() > 0)
   {
    strKey = strTemp;i++;
    strTemp = "";
    if(szMapChars[i] == '\0')
    {
     if((nMode==OVERWRITE) || (m_tEntries.find(strKey)==m_tEntries.end()))
      m_tEntries[strKey] = strTemp;
     break;
    }
   }
  }
  else if((szMapChars[i] == m_cDelim) ||
    (szMapChars[i] == '\0'))
  {
   if(nMode == OVERWRITE)
    (strKey.size()>0)?(m_tEntries[strKey]=strTemp):(m_tEntries[strTemp]="");
   else
   {
    if((strKey.size()>0)&&(m_tEntries.find(strKey)==m_tEntries.end()))
     m_tEntries[strKey]=strTemp;
    else if((strKey.size()== 0)&&(m_tEntries.find(strTemp)==m_tEntries.end()))
     m_tEntries[strTemp]="";
   }
   strKey = "";
   strTemp = "";
   if(szMapChars[i] == '\0') break;
   i++;
  }
 }
 return bResult;
}
/*
bool CIdEntityMap::Deserialize(string& strMap, int nMode)
{
	bool bResult = true;
	const char* szMapChars = strMap.c_str();
	if(nMode == NEW)
		RemoveAll();

	int i=0;
	string strKey	= "";
	string strTemp	= "";
	while(szMapChars[i] != '\0')
	{
		if(szMapChars[i] == '\\')
		{
			if(szMapChars[i+1] == '\\')
				strTemp += '\\';
			else if(szMapChars[i+1] == m_cDelim)
				strTemp += m_cDelim;
			else if(szMapChars[i+1] == '=')
				strTemp += '=';
			else if(szMapChars[i+1] == '{') //Donot remove escapechar, since it maybe reqd to resolve variables
				strTemp += "\\{";
			else if(szMapChars[i+1] == '}') //Donot remove escapechar, since it's maybe reqd to resolve variables
				strTemp += "\\}";
			else
			{
				strTemp += '\\';
				strTemp += szMapChars[i+1];//Not properly escaped string
			}
			i+=2;
			if(szMapChars[i] != '\0')
				continue;
		}
		if((szMapChars[i] != m_cDelim) && (szMapChars[i] != '=') && (szMapChars[i] != '\0'))
		{
			strTemp += szMapChars[i];
			i++;
		}
		if(szMapChars[i] == '=')
		{
			if(strTemp.size() > 0)
			{
				strKey = strTemp;i++;
				strTemp = "";
				if(szMapChars[i] == '\0')
				{
					if((nMode==OVERWRITE) || (m_tEntries.find(strKey)==m_tEntries.end()))
						m_tEntries[strKey] = strTemp;
					break;
				}
			}
		}
		else if((szMapChars[i] == m_cDelim) ||
				(szMapChars[i] == '\0'))
		{
			if(nMode == OVERWRITE)
				(strKey.size()>0)?(m_tEntries[strKey]=strTemp):(m_tEntries[strTemp]="");
			else
			{
				if((strKey.size()>0)&&(m_tEntries.find(strKey)==m_tEntries.end()))
					m_tEntries[strKey]=strTemp;
				else if((strKey.size()== 0)&&(m_tEntries.find(strTemp)==m_tEntries.end()))
					m_tEntries[strTemp]="";
			}
			strKey = "";
			strTemp = "";
			if(szMapChars[i] == '\0') break;
			i++;
		}
	}
	return bResult;
}*/

/******************************************************************************
* Function name	  : CIdEntityMap::Serialize
* Description  	  : Return serialized form of this map.
* Arguments		  : @strMap[OUT] - Serialized map string

* Return       	  : None
******************************************************************************/

void CIdEntityMap::Serialize(string& strMap)
{
	string strValue = "";
	strMap			= "";
	for (IDENTITYMAP::iterator i = m_tEntries.begin();
			i != m_tEntries.end(); i++)
	{
		if(!strMap.empty())
			strMap += m_cDelim;

		strMap.append(i->first); //Assuming Keys are not to be escaped
		strMap.append("=");

		strValue = i->second;
		if(strValue.empty())
			continue;
		if(strValue.find_first_of(",={}$?~@%\\") != string::npos)
		{
			string strTemp = "";
			CIdStringUtils::InsertEscapeChars(strValue, strTemp);
			strMap.append(strTemp.c_str());
		}
		else
			strMap.append(strValue.c_str());
	}
}


CIdParamList::CIdParamList()
{
}

CIdParamList::~CIdParamList()
{
}

void CIdParamList::AddParamValue(string& strValue)
{
}

bool CIdParamList::GetFirstParam(string& strValue)
{
	if(m_tParams.size() == 0)
		return false;

	m_tIter  = m_tParams.begin();
	strValue = *m_tIter;
	m_tIter++;
	return true;
}

bool CIdParamList::GetNextParam(string& strValue)
{
	if(m_tIter == m_tParams.end())
		return false;

	strValue = *m_tIter;
	m_tIter++;
	return true;
}

bool CIdParamList::Deserialize(const char* szParamChars)
{
	bool bResult = true;
	int i=0;
	string strTemp	= "";
	while(true)
	{
		if(szParamChars[i] == '\\')
		{
			if(szParamChars[i+1] == '\\')
				strTemp += '\\';
			else if(szParamChars[i+1] == ',')
				strTemp += ',';
			else
			{
				bResult = false;
				break;
			}
			i+=2;
			if(szParamChars[i] != '\0')
				continue;
		}
		if((szParamChars[i] != ',') && (szParamChars[i] != '\0'))
		{
			strTemp += szParamChars[i];
			i++; continue;
		}
		m_tParams.push_back(strTemp);
		if(szParamChars[i] == '\0')
			break;
		strTemp = "";
		i++;
	}
	return bResult;
}

void CIdParamList::Serialize(string& strList)
{
	strList			= "";
	for (PARAMLIST::iterator i = m_tParams.begin();
			i != m_tParams.end(); i++)
	{
		if(!strList.empty())
			strList.append(",");

		if((*i).find_first_of("\\,={}$?~@%") != string::npos)
		{
			string strTemp = "";
			CIdStringUtils::InsertEscapeChars(*i, strTemp);
			strList.append(strTemp.c_str());
		}
		else
			strList.append(*i);
	}
}

int CIdParamList::Size()
{
	return m_tParams.size();
}
