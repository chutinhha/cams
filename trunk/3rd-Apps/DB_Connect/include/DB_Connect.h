#ifndef __DB_CONNECT_H__
#define __DB_CONNECT_H__

#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <Sqlucode.h>
#include <Msdasql.h>
#include <Msdadc.h>

#include <iostream>
#include <string>	// c++ string class 
#include <vector>   // stl vector class


class CDB_Connect {
	private:
		std :: string m_strDataBase;
		std :: string m_strServer;
		std :: string m_strPort;
		std :: string m_strUID;
		std :: string m_strPWD;
		std :: string m_strConnectionString;
		std :: string m_strSQLQuery;

		// Table specific variables
		// Note: All the return values will be string only, hence all the values will be stored as a string

	   /*
		* tblCatalog
		*/
		std :: string m_strtblCatalog_Id;
		std :: string m_strtblCatalog_CatalogName;
		std :: string m_strtblCatalog_Model;
		std :: string m_strtblCatalog_Supplier;
		std :: string m_strtblCatalog_Manufacturer;
		std :: string m_strtblCatalog_CatalogNumber;
		std :: string m_strtblCatalog_Note;
		std :: string m_strtblCatalog_DeleteMode;

		// SQL sepecifc api's
		SQLHANDLE sqlenvhandle;    
		SQLHANDLE sqlconnectionhandle;
		SQLHANDLE sqlstatementhandle;
		SQLRETURN retcode;
		SQLCHAR retconstring[1024];

		bool m_bDriverConnectStatus;
		bool m_isQueryReceived;

	public:
        CDB_Connect ();
       ~CDB_Connect ();
       
	private:
		void m_ConnectionString ();
		int m_SQLDriverConnect ();
		void m_ReleaseConnection ();
		int m_CreateSQLQuery (std :: string);

    public:
		int m_InvokeConnection ();
		std :: string _Exec_gen_tblMastertable (std :: string, std :: string);
		//std :: string _Exec_gen_tblMastertable ();};
};
#endif //__DB_CONNECT_H__
