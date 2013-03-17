/******************************************************************************/
// File Name		:	CAMS_DBUtil.cpp
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS AGENT
//
// Purpose			:	DB Frameork utility, contains all the Database utility
//
// Copy Right		:	
//
// Notes			:
//
// History			:
//		Created		:   02/07/2012
//		Version		:	1.0.0.0
//
//
//
/******************************************************************************/
#include "Log.h"
#include "IdGlobals.h"
#include "CAMSDBUtil.h"
#include "CAMSGlobal.h"
#include "CAMS_Initialize.h"

#include <iostream>

extern class CLog g_tLog;

using namespace std;

//
// initialize the static member here
//
int CCAMSDBUtil :: m_nsNumRowtblAssetDetails = 0;

/*****************************************************
 * Function		:
 * Description	:	
 * Arguments	:	
 * Returns		:	
 *****************************************************/
 CCAMSDBUtil :: CCAMSDBUtil ()
 {
	g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: CCAMSDBUtil");
    // entries now being read from cams_settings.xml
   /*
    m_strDataBase		            =   "Asset";
	m_strServer			            =   "win2k8r2x64";
	m_strPort			            =   "1433";
	m_strUID			            =   "ccplusplus";
	m_strPWD			            =   "password@12345";
    */
	m_strSQLQuery		            =   "select * from tblCatalog";
	m_bDriverConnectStatus          =   true;
	m_isQueryReceived		        =   false;
    m_bINSERTtblAssetDetails        =   false;

	m_strgen_tblMastertable			=	"gen_tblMastertable";
	m_strgen_tblMastertablevalues	=	"gen_tblMastertablevalues";
	m_strtblAssetDetails			=	"tblAssetDetails";
	m_strtblCatalog					=	"tblCatalog";
	m_strTblItemGroupMaster			=	"TblItemGroupMaster";
	m_strtblSoftwareProDetails		=	"tblSoftwareProDetails";
	m_strtblSupplierDetails			=	"tblSupplierDetails";
	m_strtbluserdetails				=	"tbluserdetails";
	m_strtblusergroupmaster			=	"tblusergroupmaster";

    m_strINSERT                     =   "insert";

    m_ntblAssetDetails_Id               =   0;
	m_strtblAssetDetails_AssetCode      =   "";
    m_strtblAssetDetails_AssetName      =   "";
    m_ntblAssetDetails_Group            =   0;
    m_ntblAssetDetails_Supplier         =   0;
    m_ntblAssetDetails_SupportProvider  =   0;
    m_strtblAssetDetails_CatalogNumber  =   "";
	m_strtblAssetDetails_Description    =   "";
    m_ntblAssetDetails_AssetType        =   0;
	m_strtblAssetDetails_Model          =   "";
    m_strtblAssetDetails_CompanySerial  =   "";
    m_strtblAssetDetails_MonitorSerial  =   "";
    //PurchaseDate
    //WarrantyExpiration
    //LastMaintenance
    m_dtblAssetDetails_PurchaseCost   =   0.0;
    m_ntblAssetDetails_Currency       =   0;
    m_ntblAssetDetails_DeleteMode     =   0;

	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: CCAMSDBUtil");
 }

//--------------------------------------------------------------
// Function		:	CCAMSDBUtil
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 CCAMSDBUtil :: CCAMSDBUtil (string strDataBase, string strServer, string strPort, string strUID, string strPWD)
 {
	g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: CCAMSDBUtil - argumented");
	m_strDataBase	= strDataBase;
	m_strServer		= strServer;
	m_strPort		= strPort;
	m_strUID		= strUID;
	m_strPWD		= strPWD;
	m_strSQLQuery	=	"select * from tblCatalog";
	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: CCAMSDBUtil");
 }

//--------------------------------------------------------------
// Function		:	~CCAMSDBUtil
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 CCAMSDBUtil :: ~CCAMSDBUtil()
 {
	 g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: ~CCAMSDBUtil");
	 g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: ~CCAMSDBUtil");
 }

//--------------------------------------------------------------
// Function		:	m_show_error
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 int m_show_error(unsigned int handletype, const SQLHANDLE& handle)
 {
	g_tLog.WriteLog ("ENTER : m_show_error");
    SQLCHAR sqlstate[1024];
    SQLCHAR message[1024];
    if(SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL))
	{
		g_tLog.WriteLog ("SQLSTATE: = %s %s", message, sqlstate);
	}
	g_tLog.WriteLog ("EXIT  : m_show_error");
	return 0;
 }

 //--------------------------------------------------------------
// Function		:	m_ConnectionString
// Description	:   DRIVER={SQL Server};SERVER=localhost,1433;DATABASE=MY DATABASE;UID=sgupta;PWD=password@123;
// Arguments	:
// Returns		:
// Note			:   Add arguments check and return accordingly, make to int return 
//--------------------------------------------------------------
 void CCAMSDBUtil :: m_ConnectionString ()
 {
	g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: m_ConnectionString");

	g_tLog.WriteLog ("CS    : Forming Connection String");

	m_strConnectionString = "DRIVER={SQL Server};SERVER="+m_strServer+","+m_strPort+";DATABASE="+m_strDataBase+";UID="+m_strUID+";PWD="+m_strPWD+";";

	g_tLog.WriteLog ("CS    : m_strConnectionString = %s", m_strConnectionString.c_str());

	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_ConnectionString");
 }

//--------------------------------------------------------------
// Function		:	m_CreateDataBase
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 int CCAMSDBUtil :: m_CreateDataBase (string strDataBase)
 {
	 g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: m_CreateDataBase");
	 g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_CreateDataBase\tCAMS_OK");
	 return CAMS_OK;
 }

//--------------------------------------------------------------
// Function		:	m_SQLDriverConnect
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 int CCAMSDBUtil :: m_SQLDriverConnect ()
 {
	g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: m_SQLDriverConnect");

	// create the connection string now
	// add a connection check too
	m_ConnectionString ();

	switch(SQLDriverConnect (
			sqlconnectionhandle, 
			NULL, 
			(SQLCHAR*)m_strConnectionString.c_str(), 
			SQL_NTS, 
			retconstring, 
			1024, 
			NULL,
			SQL_DRIVER_NOPROMPT))
	{
        case SQL_SUCCESS_WITH_INFO:
            m_show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
			g_tLog.WriteLog ("SQL_SUCCESS_WITH_INFO");
            break;
        case SQL_INVALID_HANDLE:
        case SQL_ERROR:
            m_show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
			g_tLog.WriteLog ("SQL_ERROR");
			m_bDriverConnectStatus = false;
			m_ReleaseConnection ();
		default:;
            break;
    }
    
    if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &sqlstatementhandle))
    {
		m_bDriverConnectStatus = false;
		m_ReleaseConnection ();
    	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_SQLDriverConnect - [%d] - %d", CAMS_FAILURE, __LINE__);
		return CAMS_FAILURE;
	}

	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_SQLDriverConnect - [%d] - %d", CAMS_OK, __LINE__);
	return CAMS_OK;
 }

//--------------------------------------------------------------
// Function		:	m_InvokeConnection
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 int CCAMSDBUtil :: m_InvokeConnection ()
 {
	g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: m_InvokeConnection");

    if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle))
    {
		g_tLog.WriteLog ("IC :: SQLAllocHandle Fails : Releasing Connection");
		m_ReleaseConnection ();
	}

    if(SQL_SUCCESS!=SQLSetEnvAttr(sqlenvhandle,SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) 
    {
		g_tLog.WriteLog ("IC :: SQLSetEnvAttr Fails : Releasing Connection");
        m_bDriverConnectStatus  = false;
		m_ReleaseConnection ();
	}
    
    if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle))
    {
		g_tLog.WriteLog ("IC :: SQLAllocHandle Fails : Releasing Connection");
        m_bDriverConnectStatus  = false;
		m_ReleaseConnection ();
	}
	
	if (m_SQLDriverConnect () == CAMS_FAILURE)
    {
        g_tLog.WriteLog ("IC :: m_SQLDriverConnect failed");
        m_bDriverConnectStatus  = false;
        //g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_InvokeConnection - [%d] - %d", CAMS_FAILURE, __LINE__);
    }
    else
    {
       	m_bDriverConnectStatus = true;
        g_tLog.WriteLog ("IC :: m_SQLDriverConnect success");
    }

	if (m_bDriverConnectStatus == false)
	{
        g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_InvokeConnection - [%d] - %d", CAMS_FAILURE, __LINE__);
		return CAMS_FAILURE;
	}
    else
    {
    	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_InvokeConnection - [%d] - %d", CAMS_SUCCESS, __LINE__);
		return CAMS_SUCCESS;
    }
	
	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_InvokeConnection - [%d] - %d", CAMS_OK, __LINE__);
	return CAMS_OK;
 }

//--------------------------------------------------------------
// Function		:	m_CreateSQLQuery
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 int CCAMSDBUtil :: m_CreateSQLQuery (string strSQLQuery)
 {
	g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: m_CreateSQLQuery");

	if (m_isQueryReceived)
	{
		m_strSQLQuery = strSQLQuery;
	}
	else
	{
		g_tLog.WriteLog ("User query not received, creating default query");
		m_strSQLQuery = "select * from tblCataLOG";
	}

	g_tLog.WriteLog ("Query Created : [%s]", m_strSQLQuery.c_str());

	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_CreateSQLQuery");

	return CAMS_SUCCESS;
 }
 
//--------------------------------------------------------------
// Function		:	m_ReceiveSQLQuery
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 int CCAMSDBUtil :: m_ReceiveSQLQuery (string strUserSQLQuery)
 {
	g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: m_ReceiveSQLQuery");
	g_tLog.WriteLog ("received user query :: <%s>", strUserSQLQuery.c_str());
	if (strUserSQLQuery != "")
	{
		m_isQueryReceived = true;	// make this status true if query received by the user
	}
	else
	{
		g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_ReceiveSQLQuery- [%d] - %d", CAMS_FAILURE, __LINE__);
		return CAMS_FAILURE;
	}

	if (m_isQueryReceived)
	{
		m_CreateSQLQuery (strUserSQLQuery);
		g_tLog.WriteLog ("Query received : <%s>", m_strSQLQuery.c_str());
	}

	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_ReceiveSQLQuery- [%d] - %d", CAMS_OK, __LINE__);

	return CAMS_OK;
 }

//--------------------------------------------------------------
// Function		:	m_ReleaseConnection
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 void CCAMSDBUtil :: m_ReleaseConnection ()
 {
	g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: m_ReleaseConnection ()");

	SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle );
	SQLDisconnect(sqlconnectionhandle);
	SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
	SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);

	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_ReleaseConnection ()");
 }

 ///////////////////////////////////////////
 //		DB table specific functionss	  //
 ///////////////////////////////////////////

//----------------------------------------------------------------------------
// Function		:	_Exec_gen_tblMastertable
// Description	:	Execute the table sepcific query 
// Arguments	:	@strgen_tblMastertable - table query
// Returns		:	@strColumnValue		   - colun value to be searched and return
// ToDo			:	check for multiple column return and handling
//----------------------------------------------------------------------------
 string CCAMSDBUtil :: _Exec_gen_tblMastertable (string strgen_tblMastertable, string strColumnValue)
 {
	 g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: _Exec_gen_tblMastertable ()");

	 g_tLog.WriteLog ("------> gen_tblMastertable <------");

	 g_tLog.WriteLog ("Query received : <%s>", strgen_tblMastertable.c_str());

	 m_InvokeConnection ();
	 m_ReceiveSQLQuery (strgen_tblMastertable);

	if (m_bDriverConnectStatus)
	{
		if (m_strSQLQuery != "")
		{
		   /*
		    *
		    SQLRETURN SQLExecDirect(
									SQLHSTMT     StatementHandle,
									SQLCHAR *    StatementText,
									SQLINTEGER   TextLength);
			*
			*/
			g_tLog.WriteLog ("Executing Query <%s>", m_strSQLQuery.c_str());
			if(SQL_SUCCESS!=SQLExecDirect(sqlstatementhandle, (UCHAR*)m_strSQLQuery.c_str(), SQL_NTS))
			{
				m_show_error(SQL_HANDLE_STMT, sqlstatementhandle);
				m_ReleaseConnection ();
			}
			else
			{
				//char name[64];
				//char address[64];

				while(SQLFetch(sqlstatementhandle)==SQL_SUCCESS)
				{
				   /*
					*
					SQLRETURN SQLGetData(
										SQLHSTMT       StatementHandle,
										SQLUSMALLINT   Col_or_Param_Num,
										SQLSMALLINT    TargetType,
										SQLPOINTER     TargetValuePtr,
										SQLLEN         BufferLength,
										SQLLEN *       StrLen_or_IndPtr);
					*
				    */
					int id;
					char CatalogName[64];
					char Model[64];
					int Supplier;
					char Manufacturer[64];
					char CatalogNumber[64];
					char Note[64];
					int DeleteMode;

					SQLGetData(sqlstatementhandle, 1, SQL_C_ULONG, &id, 0, NULL);
					SQLGetData(sqlstatementhandle, 2, SQL_C_CHAR, CatalogName, 64, NULL);
					SQLGetData(sqlstatementhandle, 3, SQL_C_CHAR, Model, 64, NULL);
					SQLGetData(sqlstatementhandle, 4, SQL_C_ULONG, &Supplier, 0, NULL);
					SQLGetData(sqlstatementhandle, 5, SQL_C_CHAR, Manufacturer, 64, NULL);
					SQLGetData(sqlstatementhandle, 6, SQL_C_CHAR, CatalogNumber, 64, NULL);
					SQLGetData(sqlstatementhandle, 7, SQL_C_CHAR, Note, 64, NULL);
					SQLGetData(sqlstatementhandle, 8, SQL_C_ULONG, &DeleteMode, 0, NULL);

				   /*
				    * form the return result according to the strColumnValue
				    */
					
					if ( strColumnValue.compare("CatalogName") == 0)
					{
						string strTmpCatalogName = CatalogName;
						m_strtblCatalog_CatalogName.append(strTmpCatalogName);
						m_strtblCatalog_CatalogName.append("\n");
						g_tLog.WriteLog ("m_strtblCatalog_CatalogName = %s", m_strtblCatalog_CatalogName.c_str());
						strTmpCatalogName = "";
					}
					else if ( strColumnValue.compare("Manufacturer") == 0)
					{
						string strTmpManufacturer = Manufacturer;
						m_strtblCatalog_Manufacturer.append(strTmpManufacturer);
						m_strtblCatalog_Manufacturer.append("\n");
						g_tLog.WriteLog ("m_strtblCatalog_Manufacturer = %s", m_strtblCatalog_Manufacturer.c_str());
						strTmpManufacturer = "";
					}
					else
					{
						g_tLog.WriteLog ("Unsupported Table Item Found");
					}

					g_tLog.WriteLog ("%d\t\t%s\t\t%s\t\t%d\t\t%s\t\t%s\t\t%s\t\t%d", id, CatalogName, Model, Supplier, Manufacturer, CatalogNumber, Note, DeleteMode);
				}
			}
		}
		else
		{
			// handle the default query
			g_tLog.WriteLog ("No Query Found");
		}
	}
	else
	{
		// check the return value
		return CAMS_FAIL_DB;
	}

	 g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_gen_tblMastertable ()");
	 return strgen_tblMastertable;
 }


 /****************************************************************************
 * Function		:	_Exec_tbluserdetails
 * Description	:	Execute the table sepcific query 
 * Arguments	:	@strtbluserdetails  - table query
 * Returns		:	@strColumnValue	    - colun value to be searched and return
 * ToDo			:	check for multiple column return and handling
 ****************************************************************************/
 string CCAMSDBUtil :: _Exec_tbluserdetails (string strtbluserdetails, string strColumnValue)
 {
	 g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: _Exec_tbluserdetails ()");

	 g_tLog.WriteLog ("------> tbluserdetails <------");

	 g_tLog.WriteLog ("Query received : <%s>", strtbluserdetails.c_str());

	 if (m_InvokeConnection () ==  CAMS_FAILURE)
     {
         g_tLog.WriteLog ("m_InvokeConnection failed");
         m_bDriverConnectStatus = false;
     }

	 m_ReceiveSQLQuery (strtbluserdetails);

	if (m_bDriverConnectStatus)
	{
		if (m_strSQLQuery != "")
		{
		   /*
		    *
		    SQLRETURN SQLExecDirect(
									SQLHSTMT     StatementHandle,
									SQLCHAR *    StatementText,
									SQLINTEGER   TextLength);
			*
			*/
			g_tLog.WriteLog ("Executing Query <%s>", m_strSQLQuery.c_str());
			if(SQL_SUCCESS!=SQLExecDirect(sqlstatementhandle, (UCHAR*)m_strSQLQuery.c_str(), SQL_NTS))
			{
				m_show_error(SQL_HANDLE_STMT, sqlstatementhandle);
				m_ReleaseConnection ();
			}
			else
			{
				while(SQLFetch(sqlstatementhandle)==SQL_SUCCESS)
				{
				   /*
					*
					SQLRETURN SQLGetData(
										SQLHSTMT       StatementHandle,
										SQLUSMALLINT   Col_or_Param_Num,
										SQLSMALLINT    TargetType,
										SQLPOINTER     TargetValuePtr,
										SQLLEN         BufferLength,
										SQLLEN *       StrLen_or_IndPtr);
					*
				    */
					int id;
					char UserName[64];
					char Contact[64];
					char Email[64];
					int UserType;
					char Password[64];
					int Status;
					int DeleteNode;
					int dept;

					SQLGetData(sqlstatementhandle, 1, SQL_C_ULONG, &id, 0, NULL);
					SQLGetData(sqlstatementhandle, 2, SQL_C_CHAR, UserName, 64, NULL);
					SQLGetData(sqlstatementhandle, 3, SQL_C_CHAR, Contact, 64, NULL);
					SQLGetData(sqlstatementhandle, 4, SQL_C_ULONG, Email, 64, NULL);
					SQLGetData(sqlstatementhandle, 5, SQL_C_ULONG, &UserType, 0, NULL);
					SQLGetData(sqlstatementhandle, 6, SQL_C_CHAR, Password, 64, NULL);
					SQLGetData(sqlstatementhandle, 5, SQL_C_ULONG, &Status, 0, NULL);
					SQLGetData(sqlstatementhandle, 7, SQL_C_ULONG, &DeleteNode, 0, NULL);
					SQLGetData(sqlstatementhandle, 8, SQL_C_ULONG, &dept, 0, NULL);

				   /*
				    * form the return result according to the strColumnValue
				    */

					if ( strColumnValue.compare("UserName") == 0)
					{
						string strTmpUserName = UserName;
						m_strtbluserdetails_UserName.append(strTmpUserName);
						m_strtbluserdetails_UserName.append("\n");
						g_tLog.WriteLog ("m_strtbluserdetails_UserName = %s", m_strtbluserdetails_UserName.c_str());
						strTmpUserName = "";
					}
					else if ( strColumnValue.compare("Password") == 0)
					{
						string strTmpPassword = Password;
						m_strtbluserdetails_Password.append(strTmpPassword);
						m_strtbluserdetails_Password.append("\n");
						g_tLog.WriteLog ("m_strtbluserdetails_Password = %s", m_strtbluserdetails_Password.c_str());
						strTmpPassword = "";
					}
					else
					{
						g_tLog.WriteLog ("Unsupported Table Item Found");
					}

					//g_tLog.WriteLog ("%d\t\t%s\t\t%s\t\t%d\t\t%s\t\t%s\t\t%s\t\t%d", id, CatalogName, Model, Supplier, Manufacturer, CatalogNumber, Note, DeleteMode);
				}
			}
		}
		else
		{
			// handle the default query
			g_tLog.WriteLog ("No Query Found");
		}
	}
	else
	{
		// check the return value
		g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_tbluserdetails ()");
		return CAMS_FAIL_DB;
	}

	if (strColumnValue == "UserName")
	{
		g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_tbluserdetails ()");
		return m_strtbluserdetails_UserName;
	}
	else if (strColumnValue == "Password")
	{
		g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_tbluserdetails ()");
		return m_strtbluserdetails_Password;
	}
	else
	{
		g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_tbluserdetails ()");
		return "Value not found";
	}

	 g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_tbluserdetails ()");
	
	 return "Value not found";
 }

 //---------------------------------------------------------------------------------
 // Function		:	_Exec_tblAssetDetails
 // Description		:	Execute the table sepcific query 
 // Arguments		:	@strtblAssetDetails - table query
 // Returns			:	@strColumnValue	    - colun value to be searched and return
 // ToDo			:	check for multiple column return and handling
 //---------------------------------------------------------------------------------
 string CCAMSDBUtil :: _Exec_tblAssetDetails (string strtblAssetDetails, string strColumnValue)
 {
	g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: _Exec_tblAssetDetails ()");

	int id, Group, Supplier, SupportProvider, AssetType, Currency, DeleteMode;
	
    char AssetCode[64];
    char AssetName[64];
    char CatalogNumber[64];
    char Description[64];
    char Model[64];
    char CompanySerial[64];
    char PurchaseDate[64];
    char MonitorSerial[64];
    char WarrantyExpiration[64];
    char LastMaintenance[64];

    double PurchaseCost;

	g_tLog.WriteLog ("------> tblAssetDetails <------");

	g_tLog.WriteLog ("Query received : <%s>", strtblAssetDetails.c_str());
	
    if (m_InvokeConnection () == CAMS_FAILURE)
    {
        g_tLog.WriteLog ("m_InvokeConnection failed");
        m_bDriverConnectStatus = false;
    }

	m_ReceiveSQLQuery (strtblAssetDetails);

	if (m_bDriverConnectStatus)
	{
		if (m_strSQLQuery != "")
		{
		   /*
		    *
		    SQLRETURN SQLExecDirect(
									SQLHSTMT     StatementHandle,
									SQLCHAR *    StatementText,
									SQLINTEGER   TextLength);
			*
			*/
			g_tLog.WriteLog ("Executing Query <%s>", m_strSQLQuery.c_str());
			if(SQL_SUCCESS!=SQLExecDirect(sqlstatementhandle, (UCHAR*)m_strSQLQuery.c_str(), SQL_NTS))
			{
				m_show_error(SQL_HANDLE_STMT, sqlstatementhandle);
				m_ReleaseConnection ();
			}
			else
			{
				while(SQLFetch(sqlstatementhandle)==SQL_SUCCESS)
				{
				   /*
					*
					SQLRETURN SQLGetData(
										SQLHSTMT       StatementHandle,
										SQLUSMALLINT   Col_or_Param_Num,
										SQLSMALLINT    TargetType,
										SQLPOINTER     TargetValuePtr,
										SQLLEN         BufferLength,
										SQLLEN *       StrLen_or_IndPtr);
					*
				    */

					SQLGetData(sqlstatementhandle, 1, SQL_C_ULONG, &id, 0, NULL);
					SQLGetData(sqlstatementhandle, 2, SQL_C_CHAR, AssetCode, 64, NULL);
					SQLGetData(sqlstatementhandle, 3, SQL_C_CHAR, AssetName, 64, NULL);
					SQLGetData(sqlstatementhandle, 4, SQL_C_ULONG, &Group, 0, NULL);
					SQLGetData(sqlstatementhandle, 5, SQL_C_ULONG, &Supplier, 0, NULL);
					SQLGetData(sqlstatementhandle, 6, SQL_C_ULONG, &SupportProvider, 0, NULL);
					SQLGetData(sqlstatementhandle, 7, SQL_C_CHAR, CatalogNumber, 64, NULL);
					SQLGetData(sqlstatementhandle, 8, SQL_C_CHAR, Description, 64, NULL);
					SQLGetData(sqlstatementhandle, 9, SQL_C_ULONG, &AssetType, 0, NULL);
					SQLGetData(sqlstatementhandle, 10, SQL_C_CHAR, Model, 64, NULL);
					SQLGetData(sqlstatementhandle, 11, SQL_C_CHAR, CompanySerial, 64, NULL);
					SQLGetData(sqlstatementhandle, 12, SQL_C_CHAR, MonitorSerial, 64, NULL);
					SQLGetData(sqlstatementhandle, 13, SQL_C_CHAR, PurchaseDate, 64, NULL);
					SQLGetData(sqlstatementhandle, 14, SQL_C_CHAR, WarrantyExpiration, 64, NULL);
					SQLGetData(sqlstatementhandle, 15, SQL_C_CHAR, LastMaintenance, 64, NULL);
					SQLGetData(sqlstatementhandle, 16, SQL_C_DOUBLE, &PurchaseCost, 0, NULL);
					SQLGetData(sqlstatementhandle, 17, SQL_C_ULONG, &Currency, 0, NULL);
					SQLGetData(sqlstatementhandle, 18, SQL_C_ULONG, &DeleteMode, 0, NULL);

                   /*
                    * special cases:
                    * when we want to execute query to get all the result and do some tasks like 
                    * counting the number of items etc, below code can explain that.
                    */

                    if (strColumnValue.compare ("") == 0)
                    {
                        g_tLog.WriteLog ("strColumnValue is empty, performing special request operation");
                        m_nsNumRowtblAssetDetails ++;
                        g_tLog.WriteLog ("found <%d> rows in table tblAssetDetails", m_nsNumRowtblAssetDetails);
                    }
                    else
                    {
				       /*
				        * form the return result according to the strColumnValue
				        */
					    if ( strColumnValue.compare("AssetCode") == 0)
					    {
						    string strTmpAssetCode = AssetCode;
                            m_strtblAssetDetails_AssetCode = "";
						    m_strtblAssetDetails_AssetCode.append(strTmpAssetCode);
						    m_strtblAssetDetails_AssetCode.append("\n");
						    g_tLog.WriteLog ("m_strtblAssetDetails_AssetCode = %s", m_strtblAssetDetails_AssetCode.c_str());
						    strTmpAssetCode = "";
					    }
					    else if ( strColumnValue.compare("AssetName") == 0)
					    {
						    string strTmpAssetName = AssetName;
						    m_strtblAssetDetails_AssetName.append(strTmpAssetName);
						    m_strtblAssetDetails_AssetName.append("\n");
						    g_tLog.WriteLog ("m_strtblAssetDetails_AssetName = %s", m_strtblAssetDetails_AssetName.c_str());
						    strTmpAssetName = "";
					    }
					    else if ( strColumnValue.compare("Description") == 0)
					    {
						    string strTmpDescription = Description;
						    m_strtblAssetDetails_Description.append(strTmpDescription);
						    m_strtblAssetDetails_Description.append("\n");
						    g_tLog.WriteLog ("m_strtblAssetDetails_Description = %s", m_strtblAssetDetails_Description.c_str());
						    strTmpDescription = "";
					    }
					    else if ( strColumnValue.compare("Model") == 0)
					    {
						    string strTmpModel = Model;
						    m_strtblAssetDetails_Model.append(strTmpModel);
						    m_strtblAssetDetails_Model.append("\n");
						    g_tLog.WriteLog ("m_strtblAssetDetails_Model = %s", m_strtblAssetDetails_Model.c_str());
						    strTmpModel = "";
					    }
					    else
					    {
						    g_tLog.WriteLog (" Not implemented yet !!!");
					    }
                    }
#if 0
					g_tLog.WriteLog ("%d\t%s\t%s\t%d\t%d\t%d\t%s\t%s\t%d\t%s\t%s\t%s\t%f\t%d\t%d", id, AssetCode, AssetName, Group, Supplier, SupportProvider, CatalogNumber, Description, AssetType, Model, CompanySerial, MonitorSerial, PurchaseCost, Currency, DeleteMode);
#endif
				}
			}
		}
		else
		{
			// handle the default query
			g_tLog.WriteLog ("No Query Found");
		}
	}
	else
	{
		// check the return value
		g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_tblAssetDetails ()");
		return CAMS_FAIL_DB;
	}
	g_tLog.WriteLog ("%d\t%s\t%s\t%d\t%d\t%d\t%s\t%s\t%d\t%s\t%s\t%s\t%f\t%d\t%d", id, AssetCode, AssetName, Group, Supplier, SupportProvider, CatalogNumber, Description, AssetType, Model, CompanySerial, MonitorSerial, PurchaseCost, Currency, DeleteMode);

    m_ntblAssetDetails_Id_Pub               = id;
    m_strtblAssetDetails_AssetCode_Pub      = AssetCode;
    m_strtblAssetDetails_AssetName_Pub      = AssetName;
    m_ntblAssetDetails_Group_Pub            = Group;
    m_ntblAssetDetails_Supplier_Pub         = Supplier;
    m_ntblAssetDetails_SupportProvider_Pub  = SupportProvider;
    m_strtblAssetDetails_CatalogNumber_Pub  = CatalogNumber;
    m_strtblAssetDetails_Description_Pub    = Description;
    m_ntblAssetDetails_AssetType_Pub        = AssetType;
    m_strtblAssetDetails_Model_Pub          = Model;
    m_strtblAssetDetails_CompanySerial_Pub  = CompanySerial;
    m_strtblAssetDetails_MonitorSerial_Pub  = MonitorSerial;

	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_tblAssetDetails ()");
	return "";
 }

#if 0
 //---------------------------------------------------------------------------------
 // Function		:	_Update_tbluserdetails
 // Description		:	Execute the table sepcific query 
 // Arguments		:	@strtblAssetDetails - table query
 // Returns			:	@strColumnValue	   - column value to be update
 // ToDo			:	
 //---------------------------------------------------------------------------------
 string CCAMSDBUtil :: _Update_tbluserdetails (string strtbluserdetails, string strColumnValue)
 {
	 g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: _Exec_tbluserdetails ()");

	 g_tLog.WriteLog ("------> tbluserdetails <------");

	 g_tLog.WriteLog ("Query received : <%s>", strtbluserdetails.c_str());

	 m_InvokeConnection ();
	 m_ReceiveSQLQuery (strtbluserdetails);

	if (m_bDriverConnectStatus)
	{
		if (m_strSQLQuery != "")
		{
			g_tLog.WriteLog ("Executing Query <%s>", m_strSQLQuery.c_str());
			if(SQL_SUCCESS!=SQLExecDirect(sqlstatementhandle, (UCHAR*)m_strSQLQuery.c_str(), SQL_NTS))
			{
				m_show_error(SQL_HANDLE_STMT, sqlstatementhandle);
				m_ReleaseConnection ();
			}
			else
			{
				char name[64];
				char address[64];

				while(SQLFetch(sqlstatementhandle)==SQL_SUCCESS)
				{
				   /*
					*0
					SQLRETURN SQLGetData(
										SQLHSTMT       StatementHandle,
										SQLUSMALLINT   Col_or_Param_Num,
										SQLSMALLINT    TargetType,
										SQLPOINTER     TargetValuePtr,
										SQLLEN         BufferLength,
										SQLLEN *       StrLen_or_IndPtr);
					*
				    */
					int id;
					char UserName[64];
					char Contact[64];
					char Email[64];
					int UserType;
					char Password[64];
					int Status;
					int DeleteNode;
					int dept;

					SQLGetData(sqlstatementhandle, 1, SQL_C_ULONG, &id, 0, NULL);
					SQLGetData(sqlstatementhandle, 2, SQL_C_CHAR, UserName, 64, NULL);
					SQLGetData(sqlstatementhandle, 3, SQL_C_CHAR, Contact, 64, NULL);
					SQLGetData(sqlstatementhandle, 4, SQL_C_ULONG, Email, 64, NULL);
					SQLGetData(sqlstatementhandle, 5, SQL_C_ULONG, &UserType, 0, NULL);
					SQLGetData(sqlstatementhandle, 6, SQL_C_CHAR, Password, 64, NULL);
					SQLGetData(sqlstatementhandle, 5, SQL_C_ULONG, &Status, 0, NULL);
					SQLGetData(sqlstatementhandle, 7, SQL_C_ULONG, &DeleteNode, 0, NULL);
					SQLGetData(sqlstatementhandle, 8, SQL_C_ULONG, &dept, 0, NULL);

				   /*
				    * form the return result according to the strColumnValue
				    */

					if ( strColumnValue.compare("UserName") == 0)
					{
						string strTmpUserName = UserName;
						m_strtbluserdetails_UserName.append(strTmpUserName);
						m_strtbluserdetails_UserName.append("\n");
						g_tLog.WriteLog ("m_strtbluserdetails_UserName = %s", m_strtbluserdetails_UserName.c_str());
						strTmpUserName = "";
					}
					else if ( strColumnValue.compare("Password") == 0)
					{
						string strTmpPassword = Password;
						m_strtbluserdetails_Password.append(strTmpPassword);
						m_strtbluserdetails_Password.append("\n");
						g_tLog.WriteLog ("m_strtbluserdetails_Password = %s", m_strtbluserdetails_Password.c_str());
						strTmpPassword = "";
					}
					else
					{
						g_tLog.WriteLog ("Unsupported Table Item Found");
					}

					//g_tLog.WriteLog ("%d\t\t%s\t\t%s\t\t%d\t\t%s\t\t%s\t\t%s\t\t%d", id, CatalogName, Model, Supplier, Manufacturer, CatalogNumber, Note, DeleteMode);
				}
			}
		}
		else
		{
			// handle the default query
			g_tLog.WriteLog ("No Query Found");
		}
	}
	else
	{
		// check the return value
		g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_tbluserdetails ()");
		return CAMS_FAIL_DB;
	}

	if (strColumnValue == "UserName")
	{
		g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_tbluserdetails ()");
		return m_strtbluserdetails_UserName;
	}
	else if (strColumnValue == "Password")
	{
		g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_tbluserdetails ()");
		return m_strtbluserdetails_Password;
	}
	else
	{
		g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_tbluserdetails ()");
		return m_strtbluserdetails;
	}

	 g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: _Exec_tbluserdetails ()");
	
	 return m_strtbluserdetails;
 }
#endif

 //---------------------------------------------------------------------------------
 // Function		:	m_INSERT_tbluserdetails_Query
 // Description		:	Execute the table sepcific query 
 // Arguments		:	
 // Returns			:	
 // ToDo			:	
 //---------------------------------------------------------------------------------
 int CCAMSDBUtil :: m_INSERT_tbluserdetails_Query (string m_strtbluserdetails_UserName, 
                                                   string m_strtbluserdetails_Contact,
                                                   string m_strtbluserdetails_Email,
                                                   int    m_ntbluserdetails_UserType, 
                                                   string m_strtbluserdetails_Password, 
                                                   string m_strtbluserdetails_Note,
                                                   int    m_ntbluserdetails_Status, 
                                                   int    m_ntbluserdetails_DeleteNode,
                                                   int    m_ntbluserdetails_dept)
 {
     g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: m_INSERT_tbluserdetails_Query ()");

     if (m_bINSERTtblAssetDetails)
     {
         g_tLog.WriteLog ("INSERT operation request found");
         // check the received parameters
         
         // username
         g_tLog.WriteLog ("m_strtbluserdetails_UserName = <%s>", m_strtbluserdetails_UserName.c_str());
         // contact
         g_tLog.WriteLog ("m_strtbluserdetails_Contact = <%s>", m_strtbluserdetails_Contact.c_str());
         // email
         g_tLog.WriteLog ("m_strtbluserdetails_Email = <%s>", m_strtbluserdetails_Email.c_str());
         // user type
         g_tLog.WriteLog ("m_ntbluserdetails_UserType = <%d>", m_ntbluserdetails_UserType);
         // password
         g_tLog.WriteLog ("skipping password print");
         // note
         g_tLog.WriteLog ("m_strtbluserdetails_Note = <%s>", m_strtbluserdetails_Note.c_str());
         // status
         g_tLog.WriteLog ("m_ntbluserdetails_Status = <%d>", m_ntbluserdetails_Status);
         // delete mode
         g_tLog.WriteLog ("m_ntbluserdetails_DeleteNode = <%d>", m_ntbluserdetails_DeleteNode);
         // dept
         g_tLog.WriteLog ("m_ntbluserdetails_dept = <%d>", m_ntbluserdetails_dept);

         g_tLog.WriteLog ("creating User Insert Query");

         //m_strInsertSQLQuery = "insert into tbluserdetails values ('support1', '+915222702903', 'support1@ccplusplus.com', 1, 'support', 'cams support team 1', 1, 0, 1)";

         // better to use c style to create the string
        char cQuery [512];
        sprintf (cQuery, 
                "%s into %s values ('%s', '%s', '%s', %d, '%s', '%s', %d, %d, %d)", 
                m_strINSERT.c_str(), 
                m_strtbluserdetails.c_str(), 
                m_strtbluserdetails_UserName.c_str(), 
                m_strtbluserdetails_Contact.c_str(), 
                m_strtbluserdetails_Email.c_str(),
                m_ntbluserdetails_UserType,
                m_strtbluserdetails_Password.c_str(),
                m_strtbluserdetails_Note.c_str(), m_ntbluserdetails_Status, m_ntbluserdetails_DeleteNode, m_ntbluserdetails_dept);

         m_strInsertSQLQuery = cQuery;
         memset (cQuery, '\0', sizeof (cQuery));

         g_tLog.WriteLog ("m_strInsertSQLQuery = <%s>", m_strInsertSQLQuery.c_str());

         g_tLog.WriteLog ("Invoking DB Connection");
         if (m_InvokeConnection () == CAMS_FAILURE)
         {
             g_tLog.WriteLog ("m_InvokeConnection failed");
         }
         else
         {
			g_tLog.WriteLog ("Executing SQL Operatiom");
			if(SQL_SUCCESS != SQLExecDirect(sqlstatementhandle, (UCHAR*)m_strInsertSQLQuery.c_str(), SQL_NTS))
			{
                g_tLog.WriteLog ("Query Execution Failed");
				m_show_error(SQL_HANDLE_STMT, sqlstatementhandle);
				m_ReleaseConnection ();
			}
            else
            {
                g_tLog.WriteLog ("Query Executed Successfully");
            }
         }
     }
     else
     {
         g_tLog.WriteLog ("No INSERT Operation selected");
     }

     g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_INSERT_tbluserdetails_Query () - <%d> -%d", CAMS_SUCCESS, __LINE__);
     return CAMS_SUCCESS;
 } 

  //---------------------------------------------------------------------------------
 // Function		:	_Exec_tblAssetDetails
 // Description		:	Execute the table sepcific query 
 // Arguments		:	@strtblAssetDetails - table query
 // Returns			:	@strColumnValue	    - colun value to be searched and return
 // ToDo			:	check for multiple column return and handling
 //---------------------------------------------------------------------------------
 int CCAMSDBUtil :: m_CreatetblAssetDetailsQuery (string strAssetCode, string strOperationType, string strNumItems)
 {
     g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: m_CreatetblAssetDetailsQuery ()");
     char ctblQuery [256];
     m_strtblAssetDetails_AssetCode = strAssetCode;
     sprintf (ctblQuery, "%s %s from %s where AssetCode = '%s'", strOperationType.c_str(), strNumItems.c_str(), m_strtblAssetDetails.c_str(), strAssetCode.c_str());
     m_strSQLQuery = ctblQuery;
     m_strCAMSDBSQLQuery = ctblQuery;
     g_tLog.WriteLog ("Query Created = <%s>", m_strSQLQuery.c_str());
     g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_CreatetblAssetDetailsQuery () - <%d> -%d", CAMS_OK, __LINE__);
     return CAMS_OK;
 }

 //---------------------------------------------------------------------------------
 // Function		:	m_GetNumOfAssetRow
 // Description		:	computes the number of rows in a table provided with query
 // Arguments		:	@strDBTable - table query
 // Returns			:	@strColumnValue	    - colun value to be searched and return
 // ToDo			:	check for multiple column return and handling
 //---------------------------------------------------------------------------------
 int CCAMSDBUtil :: m_GetNumOfAssetRow (string strDBTable, int &nNumOfRowInTablesTmp)
 {
	g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: m_GetNumOfAssetRow ()");

	int nNumOfTables;
    nNumOfRowInTablesTmp = 0;
	
	g_tLog.WriteLog ("Query received : <%s>", strDBTable.c_str());
	
    if (m_InvokeConnection () == CAMS_FAILURE)
    {
        g_tLog.WriteLog ("m_InvokeConnection failed");
        m_bDriverConnectStatus = false;
    }

	m_ReceiveSQLQuery (strDBTable);

	if (m_bDriverConnectStatus)
	{
		if (m_strSQLQuery != "")
		{
		   /*
		    *
		    SQLRETURN SQLExecDirect(
									SQLHSTMT     StatementHandle,
									SQLCHAR *    StatementText,
									SQLINTEGER   TextLength);
			*
			*/
			g_tLog.WriteLog ("Executing Query <%s>", m_strSQLQuery.c_str());
			if(SQL_SUCCESS!=SQLExecDirect(sqlstatementhandle, (UCHAR*)m_strSQLQuery.c_str(), SQL_NTS))
			{
				m_show_error(SQL_HANDLE_STMT, sqlstatementhandle);
				m_ReleaseConnection ();
			}
			else
			{
				while(SQLFetch(sqlstatementhandle)==SQL_SUCCESS)
				{
					SQLGetData(sqlstatementhandle, 1, SQL_C_ULONG, &nNumOfTables, 0, NULL);
                    nNumOfRowInTablesTmp ++;
                    g_tLog.WriteLog ("Found <%d> rows", nNumOfRowInTablesTmp);
				}
               
			}
		}
		else
		{
			// handle the default query
			g_tLog.WriteLog ("No Query Found");
		}
	}
	else
	{
		// check the return value
		g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_GetNumOfAssetRow ()");
		return CAMS_FAILURE;
	}
	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_GetNumOfAssetRow ()");

	return CAMS_SUCCESS;
 }

 //---------------------------------------------------------------------------------
 // Function		:	m_tblAssetDetails_HardWare_GetNumOfAsset
 // Description		:	computes the number of hardware items in table according to Asset Code
 // Arguments		:	@strDBTable - table query
 // Returns			:	@m_nNumOfHardwareAssetTmp - reference value for number of asset row found
 // ToDo			:	
 //---------------------------------------------------------------------------------
 int CCAMSDBUtil :: m_tblAssetDetails_HardWare_GetNumOfAsset (string strDBTable, int &m_nNumOfHardwareAssetTmp)
 {
	g_tLog.WriteLog ("ENTER : CCAMSDBUtil :: m_tblAssetDetails_HardWare_GetNumOfAsset ()");

    m_nNumOfHardwareAssetTmp = 0;
    char cAssetCode [32];
	
	g_tLog.WriteLog ("Query received : <%s>", strDBTable.c_str());
	
    if (m_InvokeConnection () == CAMS_FAILURE)
    {
        g_tLog.WriteLog ("m_InvokeConnection failed");
        m_bDriverConnectStatus = false;
    }

	m_ReceiveSQLQuery (strDBTable);

	if (m_bDriverConnectStatus)
	{
		if (m_strSQLQuery != "")
		{
			g_tLog.WriteLog ("Executing Query <%s>", m_strSQLQuery.c_str());
			if(SQL_SUCCESS!=SQLExecDirect(sqlstatementhandle, (UCHAR*)m_strSQLQuery.c_str(), SQL_NTS))
			{
				m_show_error(SQL_HANDLE_STMT, sqlstatementhandle);
				m_ReleaseConnection ();
			}
			else
			{
                g_tLog.WriteLog ("checkig whether vecot already has some entry");
                if (m_vtblAssetDetails_AssetCode.empty () == true)
                {
                    g_tLog.WriteLog ("vecor empty");
                }
                else
                {
                    g_tLog.WriteLog ("vecor not empty, clearing the vector elements now !!!");
                    m_vtblAssetDetails_AssetCode.clear();
                }

				while(SQLFetch(sqlstatementhandle)==SQL_SUCCESS)
				{
                    SQLGetData(sqlstatementhandle, 1, SQL_C_CHAR, cAssetCode, 64, NULL);
                    string strAssetCodeTmp = cAssetCode;
                    g_tLog.WriteLog ("Inserting [%s] in vector list", strAssetCodeTmp.c_str());
                    m_vtblAssetDetails_AssetCode.push_back (strAssetCodeTmp);
                    g_tLog.WriteLog ("Printing vector");
                    for (unsigned int i = 0; i < m_vtblAssetDetails_AssetCode.size(); i++)
                    {
                        g_tLog.WriteLog ("[%d] : AssedCode = %s", i, (m_vtblAssetDetails_AssetCode.at (i)).c_str());
                    }
                    strAssetCodeTmp = "";
                    m_nNumOfHardwareAssetTmp ++;
                    g_tLog.WriteLog ("Found <%d> rows", m_nNumOfHardwareAssetTmp);
				}
               
			}
		}
		else
		{
			// handle the default query
			g_tLog.WriteLog ("No Query Found");
            return CAMS_FAILURE;
		}
	}
	else
	{
		// check the return value
        g_tLog.WriteLog ("SQL Driver Connection not successful");
		g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_tblAssetDetails_HardWare_GetNumOfAsset ()");
		return CAMS_FAILURE;
	}

    if (m_nNumOfHardwareAssetTmp == 0) // return error when no row found
    {
        g_tLog.WriteLog ("No enrty for the selected operation, Row count is %d", m_nNumOfHardwareAssetTmp);
        return CAMS_FAILURE;
    }
	g_tLog.WriteLog ("EXIT  : CCAMSDBUtil :: m_GetNumOfAssetRow ()");

    // every seems to be OK now, we are good to return
	return CAMS_SUCCESS;
 }
