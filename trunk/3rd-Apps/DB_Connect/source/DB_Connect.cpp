#include <iostream>
#include <stdio.h>
#include <string>	// c++ string class 

#include "DB_Connect.h"

using namespace std;

CDB_Connect :: CDB_Connect () {
    m_strDataBase		            =   "cams";
	m_strServer			            =   "SAURABH1GUPTA";
	m_strPort			            =   "1433";
	m_strUID			            =   "sgupta";
	m_strPWD			            =   "password@123";
}

CDB_Connect :: ~CDB_Connect () {
}

//--------------------------------------------------------------
// Function		:	show_error
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 int show_error(unsigned int handletype, const SQLHANDLE& handle)
 {
	printf ("ENTER : show_error\n");
    SQLCHAR sqlstate[1024];
    SQLCHAR message[1024];
    if(SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL))
	{
		printf ("SQLSTATE: = %s %s\n", message, sqlstate);
	}
	printf ("EXIT  : show_error\n");
	return 0;
 }

//--------------------------------------------------------------
// Function		:	m_ConnectionString
// Description	:   DRIVER={SQL Server};SERVER=localhost,1433;DATABASE=MY DATABASE;UID=sgupta;PWD=password@123;
// Arguments	:
// Returns		:
// Note			:   Add arguments check and return accordingly, make to int return 
//--------------------------------------------------------------
 void CDB_Connect :: m_ConnectionString ()
 {
	printf ("ENTER : CDB_Connect :: m_ConnectionString\n");
	printf ("CS    : Forming Connection String");

	m_strConnectionString = "DRIVER={SQL Server};SERVER="+m_strServer+","+m_strPort+";DATABASE="+m_strDataBase+";UID="+m_strUID+";PWD="+m_strPWD+";";

	printf ("CS    : m_strConnectionString = %s\n", m_strConnectionString.c_str());
	printf ("EXIT  : CDB_Connect :: m_ConnectionString\n");
 }

 //--------------------------------------------------------------
// Function		:	m_SQLDriverConnect
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 int CDB_Connect :: m_SQLDriverConnect ()
 {
	printf ("ENTER : CDB_Connect :: m_SQLDriverConnect\n");

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
            show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
			printf ("SQL_SUCCESS_WITH_INFO");
            break;
        case SQL_INVALID_HANDLE:
        case SQL_ERROR:
            show_error(SQL_HANDLE_DBC, sqlconnectionhandle);
			printf ("SQL_ERROR");
			m_bDriverConnectStatus = false;
			m_ReleaseConnection ();
		default:;
            break;
    }
    
    if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_STMT, sqlconnectionhandle, &sqlstatementhandle))
    {
		m_bDriverConnectStatus = false;
		m_ReleaseConnection ();
    	printf ("EXIT  : CDB_Connect :: m_SQLDriverConnect - [%d] - %d\n", -1, __LINE__);
		return -1;
	}

	printf ("EXIT  : CDB_Connect :: m_SQLDriverConnect - [%d] - %d\n", 0, __LINE__);
	return 0;
 }

 //--------------------------------------------------------------
// Function		:	m_ReleaseConnection
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 void CDB_Connect :: m_ReleaseConnection ()
 {
	printf ("ENTER : CDB_Connect :: m_ReleaseConnection ()\n");

	SQLFreeHandle(SQL_HANDLE_STMT, sqlstatementhandle );
	SQLDisconnect(sqlconnectionhandle);
	SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
	SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);

	printf ("EXIT  : CDB_Connect :: m_ReleaseConnection ()\n");
 }

 //--------------------------------------------------------------
// Function		:	m_InvokeConnection
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
 int CDB_Connect :: m_InvokeConnection ()
 {
	printf ("ENTER : CDB_Connect :: m_InvokeConnection\n");

    if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlenvhandle))
    {
		printf ("IC :: SQLAllocHandle Fails : Releasing Connection\n");
		m_ReleaseConnection ();
	}

    if(SQL_SUCCESS!=SQLSetEnvAttr(sqlenvhandle,SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0)) 
    {
		printf ("IC :: SQLSetEnvAttr Fails : Releasing Connection\n");
        m_bDriverConnectStatus  = false;
		m_ReleaseConnection ();
	}
    
    if(SQL_SUCCESS!=SQLAllocHandle(SQL_HANDLE_DBC, sqlenvhandle, &sqlconnectionhandle))
    {
		printf ("IC :: SQLAllocHandle Fails : Releasing Connection\n");
        m_bDriverConnectStatus  = false;
		m_ReleaseConnection ();
	}
	
	if (m_SQLDriverConnect () == -1)
    {
        printf ("IC :: m_SQLDriverConnect failed\n");
        m_bDriverConnectStatus  = false;
        //printf ("EXIT  : CDB_Connect :: m_InvokeConnection - [%d] - %d", CAMS_FAILURE, __LINE__);
    }
    else
    {
       	m_bDriverConnectStatus = true;
        printf ("IC :: m_SQLDriverConnect success\n");
    }

	if (m_bDriverConnectStatus == false)
	{
        printf ("EXIT  : CDB_Connect :: m_InvokeConnection - [%d] - %d\n", -1, __LINE__);
		return -1;
	}
    else
    {
    	printf ("EXIT  : CDB_Connect :: m_InvokeConnection - [%d] - %d\n", 1, __LINE__);
		return 1;
    }
	
	printf ("EXIT  : CDB_Connect :: m_InvokeConnection - [%d] - %d\n", 0, __LINE__);
	return 0;
 }

 //----------------------------------------------------------------------------
// Function		:	_Exec_gen_tblMastertable
// Description	:	Execute the table sepcific query 
// Arguments	:	@strgen_tblMastertable - table query
// Returns		:	@strColumnValue		   - colun value to be searched and return
// ToDo			:	check for multiple column return and handling
//----------------------------------------------------------------------------
 string CDB_Connect :: _Exec_gen_tblMastertable (string strgen_tblMastertable, string strColumnValue)
 //string CDB_Connect :: _Exec_gen_tblMastertable ()
 {
	 printf ("ENTER : CCAMSDBUtil :: _Exec_gen_tblMastertable ()\n");

	 printf ("------> gen_tblMastertable <------\n");
	 //printf ("Query received : <%s>\n", strgen_tblMastertable.c_str());
	 m_strSQLQuery = "select * from tblAssetDetails";
	 //m_InvokeConnection ();
	 //m_ReceiveSQLQuery (strgen_tblMastertable);
	//strgen_tblMastertable = "";

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
			printf ("Executing Query <%s>\n", m_strSQLQuery.c_str());
			if(SQL_SUCCESS!=SQLExecDirect(sqlstatementhandle, (UCHAR*)m_strSQLQuery.c_str(), SQL_NTS))
			{
				show_error(SQL_HANDLE_STMT, sqlstatementhandle);
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
						printf ("m_strtblCatalog_CatalogName = %s", m_strtblCatalog_CatalogName.c_str());
						strTmpCatalogName = "";
					}
					else if ( strColumnValue.compare("Manufacturer") == 0)
					{
						string strTmpManufacturer = Manufacturer;
						m_strtblCatalog_Manufacturer.append(strTmpManufacturer);
						m_strtblCatalog_Manufacturer.append("\n");
						printf ("m_strtblCatalog_Manufacturer = %s", m_strtblCatalog_Manufacturer.c_str());
						strTmpManufacturer = "";
					}
					else
					{
						printf ("Unsupported Table Item Found");
					}

					printf ("%d\t\t%s\t\t%s\t\t%d\t\t%s\t\t%s\t\t%s\t\t%d", id, CatalogName, Model, Supplier, Manufacturer, CatalogNumber, Note, DeleteMode);
				}
			}
		}
		else
		{
			// handle the default query
			printf ("No Query Found");
		}
	}
	else
	{
		// check the return value
		return "CAMS_FAIL_DB";
	}

	 printf ("EXIT  : CCAMSDBUtil :: _Exec_gen_tblMastertable ()\n");
	 return strgen_tblMastertable;
 }

