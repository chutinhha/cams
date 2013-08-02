/******************************************************************************/
// File Name		:	CAMS_Test.cpp
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS Test Agent
//
// Purpose			:	Test Application to run the CAMS Engine
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
#include <cstdio>
#include <string>
#include <cstdlib>
#include <iostream>

#ifdef WIN32
#include <CAMS_Windows_Include.h>
#else
#include <CAMS_Linux_Include.h>
#endif

#include "Log.h"
#include "CAMS_CLI.h"
#include "IdGlobals.h"
#include "CAMSDBUtil.h"
#include "CAMSGlobal.h"
#include "CAMS_Initialize.h"
#include "IdPLatformSpDfns.h"
#include "CAMS_Instance_Control.h"
#include "DB_Connect.h"

extern class CLog g_tLog;
CCAMS_Initialize g_inlz;
extern class CCAMSDBUtil g_dbu;
CCAMSCLI *cli;

using namespace std;

int main (int argc, char **argv)
{
	CLEAR
#ifdef DB_CONNECT
	CDB_Connect dbtest;
	dbtest.m_InvokeConnection ();
	dbtest._Exec_gen_tblMastertable ("select * from tblAssetDetails", "CatalogName");
#else
    CCAMSInstanceControl *cams;
    cams = CCAMSInstanceControl::getInstance();
    // obtain the cli single object 
    cams->method();

	//CCAMSCLI *cli		= new CCAMSCLI();
#ifdef CAMS_DEBUG
    cout << "Initializing Log !!!!!" << endl;
#endif
    g_inlz.m_initialize_logger ();

#ifdef CAMS_DEBUG
    cout << "Initializing Configuration Settings !!!!!" << endl;
#endif
    g_inlz.m_initialize_config_settings ();

	printf ("\n################################################\n");

#ifdef CAMS_LOGIN
	cli->m_Login();
#endif

#ifdef CAMS_NO_LOGIN
    cli->m_DisplayMenu ();
#endif

    //delete cli;
    cams->cleanup ();
	delete cams;
#endif //DB_CONNECT
	return 0;
}
