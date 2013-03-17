/*strElementPath=<settings><configuration><self_listen_port>
strValue=value within the self_address
strattrvalue=attribute of whether the dumb is true or false
bcreate=if it is true then it will create the strelementpath if there is no path
        if it is false then it will not create the strelementpath
---------------------------------------------------------------------------------------*/

#include "IdCommons.h"
#include "Log.h"
#include "IdXmlUtility.h"
#include <stdio.h>

CLog g_tLog;

/**
 Opens the default path for settings.xml file
*/
CIdXmlUtility::CIdXmlUtility() : m_status(false), m_bDomExists(false)
{
}

/**
 \param string strXmlFilePath - Opens the given path for settings.xml file
*/
CIdXmlUtility::CIdXmlUtility(string strXmlFilePath) :  m_status(false), m_bDomExists(false)
{
    m_strXmlFilePath = strXmlFilePath;
}

CIdXmlUtility::~CIdXmlUtility()
{
	if(m_bDomExists)
	{
		CHECK_DELETE(m_domParser);
	}
}

bool CIdXmlUtility::Open()
{
	if( m_strXmlFilePath == "" )
	{
		g_tLog.WriteLog("XML file not specified!!");
		m_status = false;
		return false;
	}
	CreateDOM();
	if( !m_domParser->Load(m_strXmlFilePath.c_str()) )
	{
		g_tLog.WriteLog("XML Load Error:<%s> !!", m_strXmlFilePath.c_str());
		m_status = false;
		return false;	//GURU: returning error
	}
	g_tLog.WriteLog("XML Load Success:<%s>", m_strXmlFilePath.c_str());
	m_status = true;
	return true;	//GURU: returning status
}

void CIdXmlUtility::Close()
{
	CHECK_DELETE(m_domParser);
	m_bDomExists = false;
}

bool CIdXmlUtility::OpenXMLFile(const char* szXMLFilePath)
{
	CreateDOM();
	if( !m_domParser->Load(szXMLFilePath) )
	{
		g_tLog.WriteLog("XML Load Error:<%s> !!", szXMLFilePath);
		m_status = false;
		return false;
	}
	g_tLog.WriteLog("XML Load Success:<%s>", szXMLFilePath);
	m_status = true;
	return true;
}

bool CIdXmlUtility::OpenXMLString(const char* szXMLString)
{
	CreateDOM();
	if(!m_domParser->SetDoc(szXMLString))
	{
		g_tLog.WriteLog("XML Load String Error !!\n%s\n", szXMLString);
		return false;
	}

	return true;
}

void CIdXmlUtility::CreateDOM()
{
	if(m_bDomExists)
	{
		CHECK_DELETE(m_domParser); // Drop the existing DOM tree
	}
	else
	{
		m_bDomExists = true;
	}

	#if defined(WIN32) && !defined(__CYGWIN__)
		m_domParser = new CDOMParser();
	#else
		m_domParser = new CDOMParserSTL();
	#endif
}

//static string settingspath = g_tEngine->m_strInstRoot + FPATH_SETTINGS;
void CIdXmlUtility::dumpDoc()
{
	cout<<"Dumping the Settings.xml file:"<<endl<<m_domParser->GetDoc()<<endl;
}

stringCollection_t CIdXmlUtility::getNodes(string path)
{
	istrstream sstream(path.c_str());
	//const int max_node_depth = 4; //by default we expect xml node depth of max_node_depth
	//stringCollection_t & nodes = * new stringCollection_t(max_node_depth);
	stringCollection_t nodes;

	const size_t buflen = path.size()+1;
	char * buffer = new char[buflen];

    while (1)
    {
	sstream.getline(buffer, buflen, '/');
	//cerr<<"Got NodeName:"<<buffer<<endl;
	if( strlen(buffer) != 0 )	//skip the first '/' for root node
      	nodes.push_back(buffer);
    if (sstream.eof())
        break;
    }
	delete [] buffer;
    return nodes;
}

/*void CIdXmlUtility::dispNodes(stringCollection_t &nodes)
{
	  for(strCollIter_t i=nodes.begin() ; i != nodes.end(); i++ )
	  {
		string nodeName = *i;
		cerr<<"\t" << nodeName<<endl;
	  }
 return;
}*/

/*if m_strvalue is not found in the strelementpath then it returns 0,
if m_strvalue is found in the strelementpath then it returns 1,
else if it shows error then it returns -1

case 1: PATH is wrong
		 return EPATH;	//-1
case 2: Value is missing
		return EMISSING;	//0
case 3: Value is present
			strValue = m_domParserGetData();
			return ESUCCESS;	//1
*/
int CIdXmlUtility::GetValue(string strElementPath,string &strValue)
{
	  m_strElementPath = strElementPath;
	  stringCollection_t nodes = getNodes(m_strElementPath);

	  m_domParser->ResetPos();//set current element of parser to root
	  strCollIter_t i=nodes.begin();	//skip the root node (ie Settings node)
	  i++;
	  for(; i != nodes.end(); i++ )
	  {
		string nodeName = *i;
        g_tLog.WriteLog("Looking for child node:%s", nodeName.c_str());

		if( ! m_domParser->FindChildElem(nodeName.c_str()))
		{
			g_tLog.WriteLog("Node %s not found", nodeName.c_str());
			#if defined(WIN32) && !defined(__CYGWIN__)
				g_tLog.WriteLog("DOM Error Status:%s", m_domParser->GetError());
			#else
				g_tLog.WriteLog("DOM Error Status:%s", m_domParser->GetError().c_str());
			#endif
			return EPATH;
		}
		else
			m_domParser->IntoElem();
	  }
	  strValue = m_domParser->GetData();
	  if(strValue == "" )
	  	  return EMISSING;
	  else
		   return ESUCCESS;

}

int CIdXmlUtility::GetAttrValue(string strElementPath,string strAttrName,string &strValue)
{

	m_strElementPath = strElementPath;
    stringCollection_t nodes = getNodes(m_strElementPath);

	m_domParser->ResetPos();//set current element of parser to root
	strCollIter_t i=nodes.begin();	//skip the root node (ie Settings node)
	i++;
	for(; i != nodes.end(); i++)
    {
		string nodeName = *i;
		#if defined(WIN32) && !defined(__CYGWIN__)
			//g_tLog.WriteLog("Current Node:%s", m_domParser->GetTagName());
		#else
			g_tLog.WriteLog("Current Node:%s", m_domParser->GetTagName().c_str());
		#endif

		g_tLog.WriteLog("Looking for child node:%s", nodeName.c_str());
		if( ! m_domParser->FindChildElem(i->c_str()))
        {
			g_tLog.WriteLog("Node %s not found", nodeName.c_str());
			return EPATH;
        }
		else
			m_domParser->IntoElem();
    }
    strValue = m_domParser->GetAttrib(strAttrName.c_str());
	if(strValue == "" )	// check the value and return accordingly
		   return EMISSING;
    else
	       return ESUCCESS;

}

/*
    if bCreate=true then if there is no strelementpath then it will create the new path for that strvalue;
	                and if the strelementpath exists then it will update the strvalue;
	if bcreate=false then only it returns 0;
*/

/**
\todo Set functions of XmlUtility depend on DOMParserSTL::Set functions.
 Calling DomParserSTL::SetXXXX() followed by GetXXXXX() doesn't show the updated value.
*/
int CIdXmlUtility::SetValue(string strElementPath,string strValue,bool bCreate)
{
	m_strElementPath = strElementPath;

	if( !IsNodeExist(m_strElementPath)  ) //if the strelementpath doesn't exist
	{
		if( bCreate == false ) //path is REQUIRED, return error
			return 0;
		else
		{
			//create the strelementpath
			m_domParser->ResetPos();//set current element of parser to root
			stringCollection_t nodes = getNodes(strElementPath);//Get the element names in descending order from root
			cerr<<"Current Node:"<<m_domParser->GetTagName()<<endl;
			//case 1:
			//		Leaf element is missing
			//case 2:
			//		Internal element is missing

			m_domParser->ResetPos();//set current element of parser to root
			strCollIter_t i=nodes.begin();	//skip the root node (ie Settings node)
			i++;
			for(; i != nodes.end(); i++)
            {
				string nodeName = *i;
				#if defined(WIN32) && !defined(__CYGWIN__)
					g_tLog.WriteLog("Current Node:%s", m_domParser->GetTagName());
				#else
					g_tLog.WriteLog("Current Node:%s", m_domParser->GetTagName().c_str());
				#endif

				g_tLog.WriteLog("Looking for child node:%s", nodeName.c_str());
				if( !m_domParser->FindChildElem(i->c_str()) )
					m_domParser->AddChildElem( i->c_str() );  //if not found child, create

                m_domParser->IntoElem();//goes inside that element
            }

		}
	}

	//Set the value for the newly created path and if the strelementpath exists then also it sets the value
	m_domParser->SetData(strValue.c_str(),0);
	m_domParser->Save(m_strXmlFilePath.c_str());
	return 1;
}

/**
\todo Set functions of XmlUtility depend on DOMParserSTL::Set functions.
 Calling DomParserSTL::SetXXXX() followed by GetXXXXX() doesn't show the updated value.
*/
int CIdXmlUtility::SetAttrValue(string strElementPath,string strAttrName,string strValue,bool bCreate)
{
	m_strElementPath = strElementPath;
	if(!IsNodeExist(m_strElementPath))  //if the strelementpath doesn't exist
    {
		if(bCreate == false)
		{
		   return 0;
		}
		else  //create the strelementpath
		{
			//Step 1) create the element path
		    stringCollection_t nodes = getNodes(strElementPath);

			//Step 2) create the attribute
			//Step 3) set the attribute value
			m_domParser->ResetPos();//set current element of parser to root
			strCollIter_t i=nodes.begin();	//skip the root node (ie Settings node)
			i++;
			for( ; i != nodes.end(); i++ )
            {
                string nodeName = *i;
				g_tLog.WriteLog("Looking for child node:%s", nodeName.c_str());

				if( !m_domParser->FindChildElem(i->c_str()) )	//if not found child element, create
					m_domParser->AddChildElem( i->c_str() );

                m_domParser->IntoElem();//goes inside that element
            }

		}
	}

   // case 1: Attribute exists then set the value
	// case 1.1:
	//		attrib="abcd"
	// case 1.2:
	//		attrib=""
	//SOLUTION1: SetAttrib() will set the value if any OR create any missing attribute
	if( bCreate == true )
	{
		m_domParser->SetAttrib(strAttrName.c_str(),strValue.c_str());
		m_domParser->Save(m_strXmlFilePath.c_str());
		return 1;
	}
	else
	{
    //DONE: As SetAttrib default behaviour creates any missing attributes]
   // case 2: Attribute missing
   //		   case 2.1: bCreate is true then
   //		   case 2.2: bCreate is false then return error
   // SOLUTION2:
	//		if (bCreate == false && !found)
	//			return error
	//		else if( bCreate == true && found)
	//			use SetAttrib to create the attribute and set the value

		// Details: Check if Attrib exists
		int i=0;
		bool found = false;
		string tmpAttrbName;
		while(1)
		{
			tmpAttrbName = m_domParser->GetAttribName(i++);
			if( tmpAttrbName == "" )
				break;
			if( strAttrName == tmpAttrbName )
				found = true;
		}
		 if( found )
		{
			m_domParser->SetAttrib(strAttrName.c_str(),strValue.c_str());
			return 1;
		}
		else
		{
			//attrib not found return error
			return 0;
		}
	}
}

bool CIdXmlUtility::IsNodeExist(string strElementPath)
{
      m_strElementPath = strElementPath;
      if( m_domParser->FindElem(m_strElementPath.c_str()))//if the strelementpath doesn't exist
		   return false;
	  else
		   return true;
}

int CIdXmlUtility::MoveToPath(const char* szElementPath, bool bAbsolutePath)
{
	m_nLevels = 0;
	if(bAbsolutePath)
		m_domParser->ResetPos();//set current element of parser to root

	string strElementPath = szElementPath;
	stringCollection_t nodes = getNodes(strElementPath);
	strCollIter_t i=nodes.begin();
	if(bAbsolutePath)
		i++;
	while(i != nodes.end())
    {
		string nodeName = *i;
		if(!m_domParser->FindChildElem(i->c_str()))
        {
            g_tLog.WriteLog("Node %s not found", i->c_str());

			for(int nLevel=0; nLevel<m_nLevels; nLevel++)
				m_domParser->OutOfElem(); // Revert back to original
			return EPATH;
        }
		else
		{
			m_domParser->IntoElem();
			m_nLevels++;
			i++;
		}
    }
	if(bAbsolutePath)
		m_strElementPath = szElementPath;
	else
		m_strElementPath += szElementPath;

	return ESUCCESS;
}

int CIdXmlUtility::MoveToAttrPath(const char* szElementPath, const char* szAttrName,
								const char* szAttrValue, bool bAbsolutePath)
{
	m_nLevels = 0;
	string strOldPath = m_strElementPath;
	if((szElementPath == NULL) || (szAttrName == NULL))
	{
		g_tLog.WriteLog("Element Path and attr name required!! Cannot move to path.");
		return EPATH;
	}
	string strElementPath = szElementPath;
	int nLastElemIndex	  = strElementPath.find_last_of("/");
	string strLastElem = strElementPath.substr(nLastElemIndex+1);
	if(nLastElemIndex != 0)
	{
		string strPathExLast = strElementPath.substr(0, nLastElemIndex);
		if(MoveToPath(strPathExLast.c_str(), bAbsolutePath) != ESUCCESS)
			return EPATH;
	}
	else if(bAbsolutePath)
	{
		m_domParser->ResetPos();//set current element of parser to root
		if(!m_domParser->FindElem(strLastElem.c_str()))
		{
			g_tLog.WriteLog("Incorrect Root Element!! Cannot move to path.");
			return EPATH;
		}
		m_strElementPath = szElementPath;
		return ESUCCESS; // Already moved to root.
	}
	string strAttrValue = "";
	if(szAttrValue != NULL)
		strAttrValue = szAttrValue;

	// Move to last path-element with matching attribute value
	string strNodeID = "";
	m_domParser->ResetChildPos();
	while(strNodeID != strAttrValue)
	{
		if(m_domParser->FindChildElem(strLastElem.c_str()) == 0)
		{
			g_tLog.WriteLog("No <%s> element with matching attribute value <%s> for <%s>",
											szElementPath, szAttrValue, szAttrName);
			m_domParser->ResetChildPos();
			for(int nLevel=0; nLevel<m_nLevels; nLevel++)
				m_domParser->OutOfElem(); // Revert back to original
			m_strElementPath = strOldPath;
			return EMISSING;
		}
		strNodeID = m_domParser->GetChildAttrib(szAttrName);
	}
	m_domParser->IntoElem();

	if(bAbsolutePath)
		m_strElementPath = szElementPath;
	else
	{
		m_strElementPath += "/";
		m_strElementPath += strLastElem;
	}

	return ESUCCESS;
}

int CIdXmlUtility::MoveToNextAttrPath(const char* szAttrName,const char* szAttrValue)
{
	m_domParser->OutOfElem();
	string strNodeID = "";
	while(strNodeID != szAttrValue)
	{
		if(m_domParser->FindChildElem() == 0)
		{
			g_tLog.WriteLog("No element with matching attribute value <%s> for <%s>",
											szAttrValue, szAttrName);
			m_domParser->ResetChildPos();
			for(int nLevel=0; nLevel<m_nLevels; nLevel++)
				m_domParser->OutOfElem(); // Revert back to original
			return EMISSING;
		}
		strNodeID = m_domParser->GetChildAttrib(szAttrName);
	}
	m_domParser->IntoElem();
	return ESUCCESS;
}


int CIdXmlUtility::MoveToAttrPath2(const char* szElementPath, const char* szAttrName1,
							const char* szAttrValue1, const char* szAttrName2,
							const char* szAttrValue2, bool bAbsolutePath)
{
	m_nLevels = 0;
	string strOldPath = m_strElementPath;
	if((szElementPath == NULL) || (szAttrName1 == NULL) || (szAttrName2 == NULL))
	{
		g_tLog.WriteLog("Element Path and attr names required!! Cannot move to path.");
		return EPATH;
	}
	string strElementPath = szElementPath;
	int nLastElemIndex	  = strElementPath.find_last_of("/");
	string strLastElem = strElementPath.substr(nLastElemIndex+1);
	if(nLastElemIndex != 0)
	{
		string strPathExLast = strElementPath.substr(0, nLastElemIndex);
		if(MoveToPath(strPathExLast.c_str(), bAbsolutePath) != ESUCCESS)
			return EPATH;
	}
	else if(bAbsolutePath)
	{
		m_domParser->ResetPos();//set current element of parser to root
		if(!m_domParser->FindElem(strLastElem.c_str()))
		{
			g_tLog.WriteLog("Incorrect Root Element!! Cannot move to path.");
			return EPATH;
		}
		m_strElementPath = szElementPath;
		return ESUCCESS; // Already moved to root.
	}
	string strAttrValue1 = "";
	string strAttrValue2 = "";
	if(szAttrValue1 != NULL)
		strAttrValue1 = szAttrValue1;
	if(szAttrValue2 != NULL)
		strAttrValue2 = szAttrValue2;

	// Move to last path-element with matching attribute value
	string strNodeAttrValue1 = "";
	string strNodeAttrValue2 = "";
	m_domParser->ResetChildPos();
	while((strNodeAttrValue1 != strAttrValue1) || (strNodeAttrValue2 != strAttrValue2))
	{
		if(m_domParser->FindChildElem(strLastElem.c_str()) == 0)
		{
			g_tLog.WriteLog("No <%s> element with matching values for <%s> and <%s>",
											szElementPath, szAttrName1, szAttrName2);

			m_domParser->ResetChildPos();
			for(int nLevel=0; nLevel<m_nLevels; nLevel++)
				m_domParser->OutOfElem(); // Revert back to original
			m_strElementPath = strOldPath;
			return EMISSING;
		}
		strNodeAttrValue1 = m_domParser->GetChildAttrib(szAttrName1);
		strNodeAttrValue2 = m_domParser->GetChildAttrib(szAttrName2);
	}
	m_domParser->IntoElem();

	if(bAbsolutePath)
		m_strElementPath = szElementPath;
	else
	{
		m_strElementPath += "/";
		m_strElementPath += strLastElem;
	}

	return ESUCCESS;
}

void CIdXmlUtility::MoveToParent(int nLevels)
{
	for(int i=0; i<nLevels; i++)
	{
		int nLastNodeIndex = m_strElementPath.find_last_of("/");
		if(nLastNodeIndex == 0)
			break;

		m_domParser->ResetChildPos();
		m_strElementPath = m_strElementPath.substr(0, nLastNodeIndex);
		if(!m_domParser->OutOfElem())
			break;
	}
}

int CIdXmlUtility::GetCurrTagAttr(const char* szAttrName, string& strAttrValue)
{
	strAttrValue = m_domParser->GetAttrib(szAttrName);
	if(strAttrValue.empty())
	{
//		g_tLog.WriteLog("Attribute <%s> not found for <%s>",
//									szAttrName, m_strElementPath.c_str());
		return EPATH;
	}
	return ESUCCESS;
}

int CIdXmlUtility::GetCurrTagChild(const char* szChildTagName, string& strChildTagValue)
{
	m_domParser->ResetChildPos();
	if(!m_domParser->FindChildElem(szChildTagName))
	{
		g_tLog.WriteLog("Child <%s> not found under <%s>",
									szChildTagName, m_strElementPath.c_str());
		return EPATH;
	}
	strChildTagValue = m_domParser->GetChildData();
	return ESUCCESS;
}

int CIdXmlUtility::SetCurrTagChild( const char* szChildTagName, string strChildTagValue)
{
	m_domParser->ResetChildPos();
	if(!m_domParser->FindChildElem(szChildTagName))
	{
		m_domParser->AddChildElem( szChildTagName );
		m_domParser->IntoElem();
		m_domParser->SetData(strChildTagValue.c_str(),0);
		m_domParser->OutOfElem();
		m_domParser->Save(m_strXmlFilePath.c_str());
		return ESUCCESS;
	}
	strChildTagValue = m_domParser->SetChildData(strChildTagValue.c_str(),0);
	m_domParser->Save(m_strXmlFilePath.c_str());
	return ESUCCESS;

}

int CIdXmlUtility::GetCurrTagChildWithAttr(const char* szChildTagName, const char* szAttrName,
		   					const char* szAttrValue, string& strChildTagValue)
{
	string strNodeAttrValue = "";
	string strAttrValue = szAttrValue;
	m_domParser->ResetChildPos();
	while(strNodeAttrValue != strAttrValue)
	{
		if(m_domParser->FindChildElem(szChildTagName) == 0)
		{
			g_tLog.WriteLog("No <%s> element with matching attribute <%s>",
											szChildTagName, szAttrName);

			m_domParser->ResetChildPos();
			return EMISSING;
		}
		strNodeAttrValue = m_domParser->GetChildAttrib(szAttrName);
	}
	strChildTagValue = m_domParser->GetChildData();
	return ESUCCESS;
}

int CIdXmlUtility::GetCurrTagChildAttr(const char* szChildTagName,
					const char* szChildAttrName, string& strChildAttrValue)
{
	m_domParser->ResetChildPos();
	if(m_domParser->FindChildElem(szChildTagName) == 0)
	{
		g_tLog.WriteLog("Child <%s> not found under <%s>",
									szChildTagName, m_strElementPath.c_str());
		return EPATH;
	}
	strChildAttrValue = m_domParser->GetChildAttrib(szChildAttrName);
	if(strChildAttrValue.empty())
		return EPATH;

	return ESUCCESS;
}

int CIdXmlUtility::GetChildCount(const char* szChildTagName)
{
	int nCount = 0;
	while(m_domParser->FindChildElem(szChildTagName) != 0)
	{
		nCount++;
	}
	return nCount;
}

int CIdXmlUtility::GetFirstChild(const char* szChildTagName, const char* szChildAttr1Name,
							 string& strChildTagValue, string& strChildAttr1Value)
{
	m_domParser->ResetChildPos();
	return GetNextChild(szChildTagName, szChildAttr1Name,
				strChildTagValue, strChildAttr1Value);
}

int CIdXmlUtility::GetNextChild(const char* szChildTagName, const char* szChildAttr1Name,
								 string& strChildTagValue, string& strChildAttr1Value)
{
	if(!m_domParser->FindChildElem(szChildTagName))
	{
		g_tLog.WriteLog("No more child <%s> under <%s>",
									szChildTagName, m_strElementPath.c_str());
		return EPATH;
	}
	strChildTagValue = m_domParser->GetChildData();
	strChildAttr1Value = m_domParser->GetChildAttrib(szChildAttr1Name);
	return ESUCCESS;
}


/** Check error status of self
 \return bool - return the error status
*/
bool CIdXmlUtility::isLoaded ()
{
	return m_status;
}

//GURU
void CIdXmlUtility::RemoveChildElem()
{
	m_domParser->RemoveChildElem();
}

bool CIdXmlUtility::AddSubDoc(string strElementPath,string strInputSubDoc,bool bClear)
{
	stringCollection_t nodes = getNodes(strElementPath);
	m_domParser->ResetPos();
	strCollIter_t j=nodes.begin();
	int nCount = 0;
	for(; j != nodes.end(); j++ )
	{
		nCount++;
	}

	strCollIter_t i=nodes.begin();
	if(nCount != 1)
		i++;
	for(; i != nodes.end(); i++ )
	{
		string nodeName = *i;
		if( nCount == 1 )
		{
			if( ! m_domParser->FindElem(nodeName.c_str()))
			{
				cout<<"Elem Node "<<nodeName<< "not found"<<endl;
				return false;
			}
			break;
		}
		else
		{
			if( ! m_domParser->FindChildElem(nodeName.c_str()))
			{
				cout<<"Child Node "<<nodeName<< "not found"<<endl;
				return false;
			}
			else
			{
				m_domParser->IntoElem();
			}
		}
	}


	strInputSubDoc = "<entry>"+	strInputSubDoc + "</entry>";
	//g_tLog.WriteLog("strInputSubDoc<%s>",strInputSubDoc.c_str());
	CIdXmlUtility tXml;
	if(!tXml.OpenXMLString(strInputSubDoc.c_str()))
	{
		g_tLog.WriteLog("XML open error !!!<%s>",strInputSubDoc.c_str());
		return false;
	}

	string strTags;
	while(tXml.FindChildElem())
	{
		string strChildTagName = tXml.GetTagName();

		tXml.OutOfElem();
		string strChild = tXml.GetChildSubDoc();

		if(bClear && strTags.find(strChildTagName) == string::npos)
		{
			strTags = strTags + "," + strChildTagName ;
			g_tLog.WriteLog("***strChildTagName<%s>",strChildTagName.c_str());
			while(m_domParser->FindChildElem(strChildTagName.c_str()))
			{

				g_tLog.WriteLog("strChildTagName<%s>",strChildTagName.c_str());
				m_domParser->RemoveChildElem();
			}

		}

		if(!m_domParser->AddChildSubDoc(strChild.c_str()))
			return false;
		m_domParser->ResetChildPos();
	}

	m_domParser->Save( m_strXmlFilePath.c_str() );
	return true;
}

bool CIdXmlUtility::FindElem(string strElem)
{
	if (m_domParser->FindElem( strElem.c_str() ))
		return true;
	else
		return false;
}

bool CIdXmlUtility::FindChildElem(string strElem)
{
	if (m_domParser->FindChildElem( strElem.c_str() ))
	{
		m_domParser->IntoElem();
		return true;
	}
	else
		return false;
}

string CIdXmlUtility::GetSubDoc()
{
	return m_domParser->GetSubDoc();
}

string CIdXmlUtility::GetData()
{
	return m_domParser->GetData();
}

void CIdXmlUtility::SetData(string strData)
{
	m_domParser->SetData(strData.c_str(),0);
	m_domParser->Save( m_strXmlFilePath.c_str() );
}

string CIdXmlUtility::GetChildSubDoc()
{
	return m_domParser->GetChildSubDoc();
}

void CIdXmlUtility::IntoElem()
{
	m_domParser->IntoElem();
}

void CIdXmlUtility::OutOfElem()
{
	m_domParser->OutOfElem();
}

string CIdXmlUtility::GetTagName()
{
	return m_domParser->GetTagName();
}

int CIdXmlUtility::SetCurrTagChildWithAttr( const char* szChildTagName,
        const char* szAttrName,	const char* szAttrValue, const char* strChildTagValue)
{
	string strNodeAttrValue = "";
	string strAttrValue = szAttrValue;
	m_domParser->ResetChildPos();
	while(strNodeAttrValue != strAttrValue)
	{
		if(m_domParser->FindChildElem(szChildTagName) == 0)
		{
			m_domParser->AddChildElem( szChildTagName );  //if not found child, create
			m_domParser->SetChildAttrib(szAttrName,szAttrValue);
			m_domParser->IntoElem();
			m_domParser->SetData(strChildTagValue,0);
			m_domParser->OutOfElem();
			m_domParser->Save(m_strXmlFilePath.c_str());
			return ESUCCESS;
		}
		strNodeAttrValue = m_domParser->GetChildAttrib(szAttrName);
	}
	m_domParser->SetChildData(strChildTagValue,0);
	m_domParser->Save(m_strXmlFilePath.c_str());
	return ESUCCESS;
}

void CIdXmlUtility::SaveFile()
{
	m_domParser->Save( m_strXmlFilePath.c_str() );
}

void CIdXmlUtility::RemoveElem()
{
	m_domParser->RemoveElem();
}
