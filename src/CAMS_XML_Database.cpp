#include "CAMS_XML_Database.h"

#include "Log.h"
#include "IdGlobals.h"
#include "CAMSDBUtil.h"
#include "CAMSGlobal.h"
#include "CAMS_Initialize.h"

#include "IdXmlWkr.h"
#include "IdXmlUtility.h"
#include <iostream>

using namespace std;

extern class CLog g_tLog;

CCAMSXMLDatabase :: CCAMSXMLDatabase ()
{
}

CCAMSXMLDatabase :: ~CCAMSXMLDatabase ()
{
}

void CCAMSXMLDatabase :: m_test ()
{
    CIdXmlUtility xmlu;

    g_tLog.WriteLog ("Opening Configuration Settings file <%s>", XML_DATABASE);

	xmlu.OpenXMLFile (XML_DATABASE);

    string strItem = "/CAMS/CONFIGURATION/DATABASE/ASSET/ITEM_1";
    string strName;
    xmlu.GetAttrValue(strItem,"name",strName);

    //xmlu.GetValue (CONN_DATABASE, g_dbu.m_strDataBase);
}