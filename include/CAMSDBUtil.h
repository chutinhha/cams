/******************************************************************************/
// File Name		:	CAMSDBUtil.h
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS AGENT
//
// Purpose			:	DB class declaration
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

#ifndef __CAMSDBUTIL_H__
#define __CAMSDBUTIL_H__

//#include "CAMSDBUtil.h"

#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <Sqlucode.h>
#include <Msdasql.h>
#include <Msdadc.h>

#include <string>	// c++ string class 
#include <vector>   // stl vector class

class CCAMSDBUtil
{
	private:
		string m_strDataBase;
		string m_strServer;
		string m_strPort;
		string m_strUID;
		string m_strPWD;
		string m_strConnectionString;
		string m_strSQLQuery;

		// SQL sepecifc api's
		SQLHANDLE sqlenvhandle;    
		SQLHANDLE sqlconnectionhandle;
		SQLHANDLE sqlstatementhandle;
		SQLRETURN retcode;
		SQLCHAR retconstring[1024];

		bool m_bDriverConnectStatus;
		bool m_isQueryReceived;

		// Database Tables Names
		string m_strgen_tblMastertable;
		string m_strgen_tblMastertablevalues;
		string m_strtblAssetDetails;
		string m_strtblCatalog;
		string m_strTblItemGroupMaster;
		string m_strtblSoftwareProDetails;
		string m_strtblSupplierDetails;
		string m_strtbluserdetails;
		string m_strtblusergroupmaster;

		// Table specific variables
		// Note: All the return values will be string only, hence all the values will be stored as a string

	   /*
		* tblCatalog
		*/
		string m_strtblCatalog_Id;
		string m_strtblCatalog_CatalogName;
		string m_strtblCatalog_Model;
		string m_strtblCatalog_Supplier;
		string m_strtblCatalog_Manufacturer;
		string m_strtblCatalog_CatalogNumber;
		string m_strtblCatalog_Note;
		string m_strtblCatalog_DeleteMode;

	   /*
		* tbluserdetails
		*/
		string m_strtbluserdetails_UserName;
		string m_strtbluserdetails_Contact;
		string m_strtbluserdetails_Email;
		int m_ntbluserdetails_UserType;
		string m_strtbluserdetails_Password;
        string m_strtbluserdetails_Note;
		int m_ntbluserdetails_Status;
		int m_ntbluserdetails_DeleteNode;
		int m_ntbluserdetails_dept;

        string m_strInsertSQLQuery;

       /*
		* tblAssetDetails
		*/
        int    m_ntblAssetDetails_Id;
		string m_strtblAssetDetails_AssetCode;
        string m_strtblAssetDetails_AssetName;
        int    m_ntblAssetDetails_Group;
        int    m_ntblAssetDetails_Supplier;
        int    m_ntblAssetDetails_SupportProvider;
        string m_strtblAssetDetails_CatalogNumber;
		string m_strtblAssetDetails_Description;
        int    m_ntblAssetDetails_AssetType;
		string m_strtblAssetDetails_Model;
        string m_strtblAssetDetails_CompanySerial;
        string m_strtblAssetDetails_MonitorSerial;
        //PurchaseDate
        //WarrantyExpiration
        //LastMaintenance
        double m_dtblAssetDetails_PurchaseCost;
        int m_ntblAssetDetails_Currency;
        int m_ntblAssetDetails_DeleteMode;
        int m_nNumOfHardwareAsset;

       /*
        * db specific operations
        */
        string m_strINSERT;

	private:
		void m_ConnectionString ();
		int m_CreateDataBase(string);
		int m_SQLDriverConnect ();
		void m_ReleaseConnection ();
		int m_CreateSQLQuery (string);

	public:
		CCAMSDBUtil ();
		CCAMSDBUtil (string, string, string, string, string);
		~CCAMSDBUtil();

		int m_ReceiveSQLQuery (string);
		int m_InvokeConnection ();

        // create insert query for specefic tables
        int m_INSERT_tbluserdetails_Query (string, string, string, int, string, string, int, int, int);

		//  DB table specific function, direct access from the UI
		string _Exec_gen_tblMastertable (string, string);
		string _Exec_tbluserdetails (string, string);
		string _Exec_tblAssetDetails (string, string);
        //string _Update_tbluserdetails (string, string);

        int m_CreatetblAssetDetailsQuery (string, string, string);

        // general purpose utility classes e.g. count the number rows in a selected table.
        int m_GetNumOfAssetRow (string, int &nNumOfRowInTablesTmp);

       /*
        * tblAssetDetails Specific util methods
        */
        // general purpose utility classes e.g. count the number Hardware Asset in table
        int m_tblAssetDetails_HardWare_GetNumOfAsset (string, int &m_nNumOfHardwareAssetTmp);


    public:
        // take mutext loxk before making a SQL operations
        MUTEX_OBJECT m_stSyncDB;
       /*
        * status of the table operations, like insert, update, delete etc.
        * Usage : make this true if you want to insert, operations will be done based on its status
        */
        bool m_bINSERTtblAssetDetails;

        // a public query object incase it is needed by other classes
        string m_strCAMSDBSQLQuery;

        // public asset details to be used for other claass
        int m_ntblAssetDetails_Id_Pub;
        string m_strtblAssetDetails_AssetCode_Pub;
        string m_strtblAssetDetails_AssetName_Pub;
        int m_ntblAssetDetails_Group_Pub;
        int m_ntblAssetDetails_Supplier_Pub;
        int m_ntblAssetDetails_SupportProvider_Pub;
        string m_strtblAssetDetails_CatalogNumber_Pub;
        string m_strtblAssetDetails_Description_Pub;
        int m_ntblAssetDetails_AssetType_Pub;
        string m_strtblAssetDetails_Model_Pub;
        string m_strtblAssetDetails_CompanySerial_Pub;
        string m_strtblAssetDetails_MonitorSerial_Pub;

       /*
        * static table count variables, for all the tables to add here.
        */
        static int m_nsNumRowtblAssetDetails;

       /*
        * vector to store the respective elements of the table recursively when fetching from db.
        */
        vector <string> m_vtblAssetDetails_AssetCode;

        // make this class as a friend of the other class
        friend class CCAMS_Initialize;
};

extern CCAMSDBUtil g_dbu;

#endif //__CAMSDBUTIL_H__