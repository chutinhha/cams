#include <sys/stat.h>
#include <vector>
#include <fstream>

#include "IdGlobals.h"
#include "IdXmlWkr.h"
#include "IdXmlUtility.h"
#include "Log.h"

CIdXmlWkr::CIdXmlWkr()
{
}

CIdXmlWkr::~CIdXmlWkr()
{
}

bool CIdXmlWkr::InsertChild(string strPath,VectorNameValue tVectorNameValue1,VectorNameValue tVectorNameValue2,
		string strSection, string strSecAttr, string strSecAttrValue, string strOPType)	
{
	CIdXmlUtility tXml(strPath);
	if(!tXml.Open())
	{
		g_tLog.WriteLogWithoutTimeStamp("XML open error <%s> !!!",strPath.c_str());
		return false;
	}

	if(strSecAttr != "" )
	{
		if(tXml.MoveToAttrPath(strSection.c_str(), strSecAttr.c_str(), strSecAttrValue.c_str(), true) != ESUCCESS)
		{
			g_tLog.WriteLogWithoutTimeStamp("Unable to read from strName ",strSection.c_str());
		}
	}
	else
	{
		if(tXml.MoveToPath(strSection.c_str(), true) != ESUCCESS)
		{
			g_tLog.WriteLogWithoutTimeStamp("Unable to read from <%s> ",strSection.c_str());
		}
	}
	for(int i = 0; i < tVectorNameValue1.size(); i++ )
	{
		if(tVectorNameValue2[i].first == "")
		{
			if(strOPType == "delete")
			{
				if(tXml.FindChildElem(tVectorNameValue1[i].first))
				{
					tXml.OutOfElem();
					tXml.RemoveChildElem();
					tXml.SaveFile();
				}
			}
			else
			{
				tXml.SetCurrTagChild(tVectorNameValue1[i].first.c_str(), tVectorNameValue1[i].second);
			}
		}
		else
		{
			if(tXml.MoveToAttrPath(tVectorNameValue1[i].first.c_str(), tVectorNameValue2[i].first.c_str(), tVectorNameValue2[i].second.c_str(), false) != ESUCCESS)
			{
				g_tLog.WriteLogWithoutTimeStamp("Unable to read from <%s>",tVectorNameValue1[i].first.c_str());
			}
			if(strOPType == "delete")
			{
				if(tXml.FindChildElem(tVectorNameValue1[i].first))
				{
					tXml.OutOfElem();
					tXml.RemoveChildElem();
					tXml.SaveFile();
				}
			}
			else
			{
				tXml.SetData(tVectorNameValue1[i].second);
				int nPos = 0;
				int nLevel =0;
				while(1)
				{
					nPos = tVectorNameValue1[i].first.find("/",nPos);
					if(nPos == string::npos)
						break;
					nLevel++;	
					nPos++;
					
				}
				nLevel++;
				tXml.MoveToParent(nLevel);
			}
		}	
	}
}

bool CIdXmlWkr::InsertAttribute(string strPath, string strSection, string strSecAttr,
						string strSecAttrValue, VectorNameValue tVectorNameValue1,
						VectorNameValue tVectorNameValue2, string strOPType)
{
	CIdXmlUtility tXml(strPath);
	if(!tXml.Open())
	{
		g_tLog.WriteLogWithoutTimeStamp("XML open error <%s> !!!",strPath.c_str());
		return false;
	}
	if(strSecAttr != "")
	{
		if(tXml.MoveToAttrPath(strSection.c_str(), strSecAttr.c_str(), strSecAttrValue.c_str(), true) != ESUCCESS)
		{
			g_tLog.WriteLogWithoutTimeStamp("Unable to read from <%s>",strSection.c_str());
		}
		
	}
	else
	{
		if(tXml.MoveToPath(strSection.c_str(), true) != ESUCCESS)
		{
			g_tLog.WriteLogWithoutTimeStamp("Unable to read from strName ",strSection.c_str());
		}
	}

	string strSpace = "";
	for(int i = 0; i < tVectorNameValue2.size(); i++)
	{
		strSpace = tVectorNameValue2[i].first;
	}
	if(strSpace != "")
	{
		if(tXml.MoveToPath("/Properties", false) != ESUCCESS)
		{
			g_tLog.WriteLogWithoutTimeStamp("Unable to go to Properties");
		}
		for(int i = 0; i < tVectorNameValue1.size(); i++)
		{
			tXml.SetCurrTagChildWithAttr(tVectorNameValue1[i].first.c_str(),tVectorNameValue2[i].first.c_str(),tVectorNameValue2[i].second.c_str(),tVectorNameValue1[i].second.c_str());
			if(strOPType == "delete")
			{
				tXml.RemoveChildElem();
				tXml.SaveFile();
			}
		}
	}
	else
	{
		for(int i = 0; i < tVectorNameValue1.size(); i++ )
		{
			tXml.SetAttrValue(strSection.c_str(),tVectorNameValue1[i].first.c_str(),tVectorNameValue1[i].second.c_str(),true);
		}
	}
}

bool CIdXmlWkr::AddSubDoc(string strPath, string strSection, string strValue, string strOPType, string strSecAttr, string strSecAttrValue,bool bClear)
{
	CIdXmlUtility tXml(strPath);
	
	if(!tXml.Open())
	{
		g_tLog.WriteLogWithoutTimeStamp("XML open error <%s> !!!",strPath.c_str());
		return false;
	}

	if(strOPType == "delete")
	{
		if(tXml.MoveToPath(strSection.c_str(), true) != ESUCCESS)
		{
			g_tLog.WriteLogWithoutTimeStamp("Unable to read from <%s> ",strSection.c_str());
		}
		if(tXml.MoveToAttrPath(strSection.c_str(), strSecAttr.c_str(), strSecAttrValue.c_str(), true) != ESUCCESS)
		{
			g_tLog.WriteLogWithoutTimeStamp("Unable to read from <%s> ",strSection.c_str());
		}
			
		tXml.RemoveElem();
		tXml.SaveFile();
	}
	else
	{
		tXml.AddSubDoc(strSection,strValue,bClear);
	}

	g_tLog.WriteLogWithoutTimeStamp("Leaving from addsubdoc::::");
}

#ifdef IDEAS_XML_TEST
int main(int argc, char *argv[])
{
	string str_SELF_GUID;
	CIdXmlUtility xmlu;
	CIdXmlWkr tdXmlWkr;

	g_tLog.Open("../../log/", false);

	xmlu.OpenXMLFile ("settings.xml");
	xmlu.GetValue (SELF_GUID, str_SELF_GUID);

    return 0;
}
#endif