/******************************************************************************
* File Name		:	IdEntityMap.h
*
* Author			:
*
* Project			:	IDEAS
*
* Module			:	IDEAS AGENT
*
* Purpose			:	Defines the <name><value> container for storing named
*						inputs to worker, named identifiers for analyzing output
*						or named outputs to be extraced by a worker
*
* Copy Right		:	
*
* Notes			:
*
* History			:
*		Created		:   3/14/05
*		Version		:	1.0.0.0
*
*
******************************************************************************/


#ifndef __IDENTITYMAP_H__
#define __IDENTITYMAP_H___

//#pragma warning(disable:4786)

#include <iostream>
#include <string>
#include <list>
#include <map>

// Map deserialization modes
#define NEW				0
#define OVERWRITE		1
#define KEEP_EXISTING	2

#define CHAR_DELIM		','

using namespace std;


class CIdParamList
{
	public:
		CIdParamList();
		virtual ~CIdParamList();
		void AddParamValue(string& strValue);
		bool GetFirstParam(string& strValue);
		bool GetNextParam(string& strValue);
		bool Deserialize(const char* szParamChars);
		void Serialize(string& strList);
		int Size();

	private:
		typedef list<string> PARAMLIST;
		PARAMLIST			m_tParams;
		PARAMLIST::iterator	m_tIter;
};

class CIdEntityMap
{
public:
	CIdEntityMap();
	CIdEntityMap(char cDelim);
	CIdEntityMap(const CIdEntityMap& tMap);
	virtual ~CIdEntityMap();
	
	CIdEntityMap& operator= (const CIdEntityMap& tMap);
	void FlushOut(CIdEntityMap& tTargetMap);
	bool AddNewEntity(string strKey, string strValue, bool bSetLock);
	bool SetEntityValue(string strKey, string strValue);
	bool GetEntityValue(string strKey, string& strValue);
	bool GetFirstEntry(string& strKey, string& strValue);
	bool GetNextEntry(string& strKey, string& strValue);
	string GetEntityKeys(bool bOnlyEmptyKeys);
	void RemoveEntity(string strKey);
	void RemoveAll();
	void EmptyAll();
	int Size();
	void SetAddLock();
	bool Deserialize(string& strMap, int nMode = NEW);
	void Serialize(string& strMap);

	char m_cDelim;

private:

	bool m_bAddLock;
	typedef map<string, string> IDENTITYMAP;
	typedef pair<string,string> IDENTITYPAIR;
	IDENTITYMAP				m_tEntries;
	IDENTITYMAP::iterator	m_tIter;
};

#endif //__IDENTITYMAP_H__
