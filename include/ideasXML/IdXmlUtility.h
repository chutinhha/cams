#ifndef __IDXMLUTILITY_H
#define __IDXMLUTILITY_H

#include <iostream>
#include <vector>
#include <string>	
#include <strstream>

#if defined(WIN32) && !defined(__CYGWIN__)
#include "DOMParser.h"
#else
#include "DOMParserSTL.h"
#endif

using namespace std;

typedef vector<string> stringCollection_t;
typedef stringCollection_t::iterator strCollIter_t;
typedef vector<string> VectorNode;

const int EPATH = -1, EMISSING = 0, ESUCCESS = 1;

class CIdXmlUtility
{
       private:
		   string m_strElementPath;
           string m_strXmlFilePath;
		   int m_nLevels;
		   bool m_status;	///< Use with ! operator(object) to test error status
		   bool m_bDomExists;

		   //stringCollection_t * getNodes(string path);

		   //helper functions
		   void dispNodes(stringCollection_t &nodes);
		   void CreateDOM();

       public:
#if defined(WIN32) && !defined(__CYGWIN__)
		   CDOMParser*	m_domParser;
#else
		   CDOMParserSTL* m_domParser;
#endif

           CIdXmlUtility();
           CIdXmlUtility(string strXmlFilePath);
           ~CIdXmlUtility();
		   bool Open();
		   bool OpenXMLFile(const char* szXMLFilePath);
		   bool OpenXMLString(const char* szXMLString);
		   void Close();
           int GetValue(string strElementPath,string &strValue);
           int GetAttrValue(string strElementPath,string strAttrName,string &strValue);
           int SetValue(string strElementPath,string strValue,bool bCreate);
           int SetAttrValue(string strElementPath,string strAttrName,string strValue,bool bCreate);
           bool IsNodeExist(string strElementPath);

		   // Move to specified element-path which has a matching attr-value for the attr-name
		   int MoveToPath(const char* szElementPath, bool bAbsolutePath=true);
		   int MoveToAttrPath(const char* szElementPath, const char* szAttrName,
							const char* szAttrValue, bool bAbsolutePath=true);
			int MoveToNextAttrPath(const char* szAttrName,
								const char* szAttrValue);

		   int MoveToAttrPath2(const char* szElementPath, const char* szAttrName1,
							const char* szAttrValue1, const char* szAttrName2,
							const char* szAttrValue2, bool bAbsolutePath=true);
		   void MoveToParent(int nLevels);
		   int GetCurrTagAttr(const char* szAttrName, string& strAttrValue);
		   int GetCurrTagChild(const char* szChildTagName, string& strChildTagValue);
		   int SetCurrTagChild( const char* szChildTagName, string strChildTagValue);
		   int GetCurrTagChildWithAttr(const char* szChildTagName, const char* szAttrName,
		   					const char* szAttrValue, string& strChildTagValue);
		   int GetCurrTagChildAttr(const char* szChildTagName, const char* szChildAttrName,
									string& strChildAttrValue);
		   int GetChildCount(const char* szChildTagName=NULL);

		   int GetFirstChild(const char* szChildTagName, const char* szChildAttr1Name,
							 string& strChildTagValue, string& strChildAttr1Value);

		   int GetNextChild(const char* szChildTagName, const char* szChildAttr1Name,
							 string& strChildTagValue, string& strChildAttr1Value);

		   bool isLoaded();
		   stringCollection_t getNodes(string path);
		   int SetCurrTagChildWithAttr( const char* szChildTagName,
        const char* szAttrName,	const char* szAttrValue, const char* strChildTagValue);

		   //DEBUG
		   void dumpDoc();
		   void RemoveChildElem();
		   bool AddSubDoc(string strElementPath, string strInputSubDoc, bool bClear);
		   bool FindElem(string strElem);
		   bool FindChildElem(string strElem = "");
		   void RemoveElem();
		   string GetTagName();
		   void IntoElem();
		   void OutOfElem();
		   void SaveFile();

			string GetSubDoc();
			string GetData();
			void SetData(string strData);
			string GetChildSubDoc();
			string  m_tStringNode;
};

#endif
