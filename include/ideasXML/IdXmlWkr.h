#ifndef __IDXMLWKR_H__
#define __IDXMLWKR_H__
#include <iostream>
#include <fstream>
//#include "IdWorker.h"

typedef pair<string,string> NameValue;
typedef vector<NameValue> VectorNameValue;

class CIdXmlWkr 
{
public:
	CIdXmlWkr();
	~CIdXmlWkr();

public:
	
	bool InsertChild(string strPath,VectorNameValue tVectorNameValue1, VectorNameValue tVectorNameValue2,
		string strSection, string strSecAttr, string strSecAttrValue, string strOPType);
	bool InsertAttribute(string strPath, string strSection, string strSecAttr, string strSecAttrValue,
		VectorNameValue tVectorNameValue1, VectorNameValue tVectorNameValue2, string strOPType);
	bool AddSubDoc(string strPath, string strSection, string strValue, string strOPType,string strSecAttr, string strSecAttrValue, bool bClear = false);

};

#endif // __IDFILEWKR_H__
