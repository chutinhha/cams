/******************************************************************************/
// File Name		:	CAMS_Wrapper.cpp
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS AGENT
//
// Description		:	Interface between UI and backend class
//					:	function here are only used to interact 
//					:	with the c++ core engine
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

 #include <iostream>
 #include <stdio.h> 
 #include <string>
 #include <cstring>

 #include "Log.h"
 #include "CAMSDBUtil.h"
 #include "CAMSGlobal.h"
 #include "CAMS_Debug_File.h"

 using namespace std;

 extern class CLog g_tLog;
 extern class CCAMSDBUtil g_dbu;

 extern "C"
 {

/**************************************************************
 * Function		:	_CAMS_Exec_Start_Up
 * Description	:	initializer function for all the global startup
 *					operation. Put every initials here
 * Return		:	success / fail
 **************************************************************/
 __declspec(dllexport) bool _CAMS_Exec_Start_Up () 
 {
	 g_tLog.Open("./log/", false);
	 g_tLog.WriteLog ("test");

	 //CCAMSDBUtil dbu;
	 //dbu.m_InvokeConnection();
	 g_dbu._Exec_gen_tblMastertable("select * from tblCatalog WHERE Manufacturer='Lenovo'", "Manufacturer");

	 //CCAMSDebugFile df;
	 //df.test();

	 return true;
 }

/**************************************************************
 * Function		:	_CAMS_Exec_DB_Op
 * Description	:	receives db query to execute.
 * Return		:	query result in C string
 * Note			:	
 **************************************************************/
 __declspec(dllexport) char *_CAMS_Exec_DB_Op(char *szDBQuery) 
 {
	 //class CCAMSDBUtil dbu;
	 //extern class CLog g_tLog;
	// g_tLog.Open("../../log/", false);
	 //CCAMSDBUtil *dbu = new CCAMSDBUtil ();
	 //CCAMSDBUtil dbu;
	 //dbu.m_InvokeConnection();
	 //g_tLog.WriteLog("szDBQuery = %s", szDBQuery);
	 ////free (dbu);
	 return szDBQuery;
 }
/**************************************************************
 * Function		:	_CAMS_Exec_Clean_Up
 * Description	:	call all the destructors here
 * Return		:	
 * Note			:	
 **************************************************************/
 __declspec(dllexport) bool _CAMS_Exec_Clean_Up() 
 {
	return true;
 }

}