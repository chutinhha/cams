/******************************************************************************/
// File Name		:	CAMS_Instance_Control.cpp
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS AGENT
//
// Purpose			:	Maintains a single instance of modules
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

 #include "Log.h"
 #include "CAMS_CLI.h"
 #include "CAMS_Exec.h"
 #include "CAMSDBUtil.h"
 #include "CAMS_Instance_Control.h"
 
 using namespace std;

 bool CCAMSInstanceControl :: instanceFlag = false;
 CCAMSInstanceControl* CCAMSInstanceControl :: single = NULL;

 CCAMSInstanceControl* CCAMSInstanceControl :: getInstance() 
 {
    if(! instanceFlag) 
	{
        single = new CCAMSInstanceControl();
        instanceFlag = true;
        return single;
    }
    else 
	{
        return single;
    }
 }
 
 void CCAMSInstanceControl :: method() 
 {
	CCAMSCLI *cli		= new CCAMSCLI();
	//CCAMSDBUtil *dbu	= new CCAMSDBUtil();

	//CAMS_Startup (cli, dbu);
 }

 void CCAMSInstanceControl :: cleanup ()
 {
     delete cli;
 }