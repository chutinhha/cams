/******************************************************************************/
// File Name		:	CAMS_CLI.cpp
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS AGENT
//
// Purpose			:	CLI Framework. Direct connection with Databse module
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
#include "CAMS_CLI.h"
#include "CAMSDBUtil.h"
#include "CAMSGlobal.h"
#include "CAMS_Query_List.h"
#include "CAMS_Gen_Utility.h"
#include "IdPLatformSpDfns.h"

#include <string>
#include <iostream>

#ifdef WIN32
	#include <CAMS_Windows_Include.h>
#else
	#include "CAMS_Linux_Include.h"
#endif

extern class CLog g_tLog;
class CCAMSDBUtil g_dbu;
class CCAMSGenUtility g_GenUtl;

using namespace std;

//
// initialize the static member here
//
int CCAMSCLI  :: m_sLoginAttemptCount = 0;
bool CCAMSCLI :: m_bOutSignal         = false;

//--------------------------------------------------------------
// Function		:	
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
CCAMSCLI :: CCAMSCLI ()
{
	m_strAdminPhrase	= "cams";
	m_bAdminRole		= false;
	m_strUserName		= "";
	m_strPassword		= "";
	m_bUserValidated	= false;	// shuld be non validated initially
	m_bUserNameValid	= false;
	m_bPasswordValidated= false;
    m_bUserAddStatus    = false;    // people will always make mistake on first chance.

    m_nSleepTime        = 1000;
}

//--------------------------------------------------------------
// Function		:	
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
CCAMSCLI :: ~CCAMSCLI ()
{
}

//--------------------------------------------------------------
// Function		:	
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
void CCAMSCLI :: m_DisplayUserCommandList ()
{
}

//--------------------------------------------------------------
// Function		:	
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
void CCAMSCLI :: m_DisplayBanner ()
{
	g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_DisplayBanner");

	//g_tLog.WriteLog ("################################################");
	//g_tLog.WriteLog ("#####   Company Asset Management System    #####");
	//g_tLog.WriteLog ("#####   ###############################    #####");
	//g_tLog.WriteLog ("#####   Version 1.0.0	(Personal Edition) #####");
	//g_tLog.WriteLog ("#####   Copyright (c) ccplusplus.com       #####");
	//g_tLog.WriteLog ("################################################");

	printf ("\t\t################################################\n");
	printf ("\t\t#####   Company Asset Management System    #####\n");
	printf ("\t\t#####   ###############################    #####\n");
	printf ("\t\t#####   Version 1.0.0	(Personal Edition) #####\n");
	printf ("\t\t#####   Copyright (c)                      #####\n");
	printf ("\t\t################################################\n");

	g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_DisplayBanner");
}

//--------------------------------------------------------------
// Function		:	m_ParseCommandLineArgument
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
int CCAMSCLI :: m_ParseCommandLineArgument (int nNumArgument, char **arguments)
{
	g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_ParseCommandLineArgument");

    for( int count = 0; count < nNumArgument; count++ )
	{
		g_tLog.WriteLog("  argv[%d] = %s", count, arguments[count]);
	}
	g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_ParseCommandLineArgument");	
	return 0;
}

//--------------------------------------------------------------
// Function		:	m_WaitForUserResponse
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
bool CCAMSCLI :: m_WaitForUserResponse ()
{
	g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_WaitForUserResponse");	
    g_tLog.WriteLogConsole ("CAMS going to sleep, hit any key to get active !!!");
    while (m_bOutSignal != true)
    {
	    Sleep(m_nSleepTime);
	    cout << ". ";
	    if (kbhit())
	    {
		    m_bOutSignal = true;
	    }
    }
    g_tLog.WriteLogConsole (" Reactivating CAMS");
    m_bOutSignal = false;
	g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_WaitForUserResponse");	
	return true;
}

//--------------------------------------------------------------
// Function		:	m_GetQuickUserResponse
// Description	:
// Arguments	:
// Returns		:
// Note			:   
//--------------------------------------------------------------
bool CCAMSCLI :: m_GetQuickUserResponse ()
{
	g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_GetQuickUserResponse");	
    g_tLog.WriteLogConsole ("\n\nTo go back to Main Menu press [M/m], Press Enter Button.");

    char cTmpRecvChar  ='\0';
    // no need to press enter, make a common global settings
    //cin >> cTmpRecvChar;
    cTmpRecvChar = g_GenUtl.m_getChar();

    bool bStatus = false;

    while (cTmpRecvChar != '\0')
    {
        if (cTmpRecvChar == 'M' || cTmpRecvChar == 'm')
        {
            bStatus = true;
            break;
        }
        else
        {
            bStatus = false;
            break;
        }
    }

	g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_GetQuickUserResponse");	
    return bStatus;
}

//--------------------------------------------------------------
// Function		:	m_GetQuickUserResponse
// Description	:
// Arguments	:
// Returns		:
// Note			:   overloaded
//--------------------------------------------------------------
bool CCAMSCLI :: m_GetQuickUserResponse (char ch)
{
	g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_GetQuickUserResponse");	

    m_bOutSignal = false;
    m_nSleepTime = 50000;

    g_tLog.WriteLogConsole ("CAMS going to sleep, hit enter key to get active !!!");
    while (m_bOutSignal != true)
    {
	    Sleep(m_nSleepTime);
	    //cout << ". ";
	    if (kbhit())
	    {
		    m_bOutSignal = true;
	    }
    }
    g_tLog.WriteLogConsole (" Reactivating CAMS");

	g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_GetQuickUserResponse");	
    return m_bOutSignal;
}

//--------------------------------------------------------------
// Function		:	m_DisplayMenu
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
void CCAMSCLI :: m_DisplayMenu ()
{
	char cTmp = '\0';
	char ch = '\0';
    bool bStatus = false;
	cout << endl;
	cout << " 1 -> About CAMS" << endl;
	cout << " 2 -> DashBoard" << endl;
	cout << " 3 -> Supported Assets" << endl;
	cout << " 4 -> Run as a Admin User" << endl;
	cout << " 5 -> Go to Sleep Mode" << endl;
	cout << " 6 -> Completely Exit CAMS" << endl;
	cout << " \nPlease enter your choice : ";

    //cin >> ch;
    ch = g_GenUtl.m_getChar();
	g_tLog.WriteLog("ch = %c", ch);

	switch (ch)
	{
		case '1':
            CLEAR
            m_DisplayBanner ();
            m_AboutCAMS();
            ch = '\0';
            if (m_GetQuickUserResponse ())
            {
                CLEAR
                m_DisplayBanner();
                m_DisplayMenu ();
            }
			break;

		case '2':
			// DashBoard
            m_DashBoard ();
            ch = '\0';
            if (m_GetQuickUserResponse ())
            {
                CLEAR
                m_DisplayBanner();
                m_DisplayMenu ();
            }
			break;

		case '3':
			// Supported Assets
			m_tblAssetDetails();
			break;
		case '4':
			cout << "Do you want to run with Admin Role !!! [y/n]" << endl;
			m_checkAdminUser ();
			break;
		case '5':
			// Go to Sleep Mode
            m_EnterSleepMode ();
            //ch = '\n';
            if (m_GetQuickUserResponse ())
            {
                CLEAR
                m_DisplayBanner();
                m_DisplayMenu ();
            }
            break;
		case '6':
		   /*
            * Completely Exit CAMS,
            * idea, take this time to clean up the files and the objects within the sleep time provided. This looks like future :)
            */
            g_tLog.WriteLogConsole ("\n\nCAMS Exiting Completely, Wait till exit !!!");
            SLEEP (5000);
			break;
		default:
			// have top decide what to do for now
			break;
	}
}

//--------------------------------------------------------------
// Function		:	m_checkAdminUser
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
bool CCAMSCLI :: m_checkAdminUser ()
{
	g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_checkAdminUser");
	char ch;
	string strAdminPhrase;
	cin >> ch;
	if (ch == 'y' || ch == 'Y')
	{
		cout << "Enter the Admin pass phrase" << endl;
		cin >> strAdminPhrase;
		if (m_strAdminPhrase == strAdminPhrase)
		{
			cout << "-----------------------------------" << endl;
			cout << " You running as a Admin Role now !!" << endl;
			cout << "-----------------------------------" << endl;
			m_bAdminRole = true;
		}
		else
		{
			cout << " Wrong Pass Phrase !!!" << endl;
			cout << " To re-enter the Pass Phrase press [R/r]" << endl;
			cout << " To go back to Main Menu press		[M/m]" << endl;

			ch = '\0';
			ch = getchar ();
			if (ch == 'R' || ch == 'r')
			{
				m_checkAdminUser ();
			}
			else if (ch == 'M' || ch == 'm')
			{
				m_DisplayMenu ();
			}
			else
			{
				return false;
			}
		}
	}
	g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_checkAdminUser");
	return true;
}

//--------------------------------------------------------------
// Function		:	m_Login	
// Description	:	take CLI username and password
//                  cams/cams is the defaut username/password
//                  try relogin if login fails
//                  add a new user also
// Arguments	:	username and password
// Returns		:	CAMS_SUCCESS / CAMS_FAILURE
// Note			:	need enhancement
//--------------------------------------------------------------
int CCAMSCLI :: m_Login ()
{
	g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_Login");
	CLEAR
	m_DisplayBanner();
    m_bUserAddStatus = true;
	cout << endl;
	cout << endl;
    cout << " New User Sing Up          [n/N]" << endl;
    cout << " Existing User Sign In     [l/L]" << endl;
    cout << endl;
    cout << " Select your choice : ";
    char ch = g_GenUtl.m_getChar ();
    cout << endl;

    if (ch == 'n' || ch == 'N')
    {
        if (m_AddNewLoginUser () == CAMS_FAILURE)
        {
            m_bUserAddStatus = false;
            g_tLog.WriteLogConsole ("New User Sign Up fails");
            SLEEP (2500);
            m_Login ();
    	    g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_Login - %d - %d", CAMS_FAILURE, __LINE__);
	        return CAMS_FAILURE;
        }
        else
        {
            g_tLog.WriteLog ("Adding new CAMS user");
            if (!g_dbu.m_bINSERTtblAssetDetails)
            {
                // make status flag true, take a lock before changing
                //LOCK_MUTEX(&(g_dbu.m_stSyncDB));
                g_dbu.m_bINSERTtblAssetDetails = true;
                // can we make a common function for all the sql query operations ???
                if (g_dbu.m_INSERT_tbluserdetails_Query (m_strUserName, 
                                                         m_strContactNumber, 
                                                         m_strEmailAddress, 
                                                         m_nUserType, 
                                                         m_strPassword, 
                                                         m_strNote, 
                                                         m_nStatus, 
                                                         m_nDeleteMode,
                                                         m_nDept) == CAMS_FAILURE)
                {
                    g_tLog.WriteLog ("New User Add fails.");
                }
                else
                {
                    g_tLog.WriteLogConsole (" User %s added in CAMS System, Please login with again !!!", m_strUserName.c_str());
                    SLEEP (1000);
                    m_Login ();
                    // unlock status for others
                    //UNLOCK_MUTEX(&(g_dbu.m_stSyncDB));
                }
            }

        }
    }
    else if (ch == 'l' || ch == 'L')
    {
        CLEAR
        m_DisplayBanner();
        cout << " [ Note : ]\n";
	    cout << " [ -> UserName & Password are case sensitive   ]\n";
	    cout << " [ -> Hit Enter button after entering Password ]\n";
	    cout <<"\n Please Enter Your UserName & Password\n";
    	cout << endl;
	    
        cout << " Username : ";
	    cin >> m_strUserName;
        g_tLog.WriteLog ("Recevived Username <%s>", m_strUserName.c_str());
	    cout << endl;
	    
        cout << " Password : ";
        //cout << endl;
        m_strPassword = g_GenUtl.m_getPassword();

        if (m_ValidateCredentials(m_strUserName, m_strPassword) == CAMS_FAILURE)
	    {
            if (m_sLoginAttemptCount == 3)
            {
                g_tLog.WriteLogConsole ("\n\nWarning !!! Max login attempt reached,\nPlease contact your system administrator.");
                g_tLog.WriteLogConsole ( "\nTo create a new user account at CAMS press [n/N]");
       		    char cTmp = g_GenUtl.m_getChar();
		        if (cTmp == 'n' || cTmp == 'N')
		        {
			        m_AddNewLoginUser ();
		        }
            }
            else
            {
                m_sLoginAttemptCount ++;
                g_tLog.WriteLogConsole ("\n\nUserName & Password Mismatch");
                g_tLog.WriteLogConsole ("\n\nRemaining Trails Left : [%d], redirecting to login .....", 3 - m_sLoginAttemptCount);
                SLEEP (5000);
                g_tLog.WriteLog ("m_sLoginAttemptCount = %d", m_sLoginAttemptCount);
                m_Login ();
            }
	    }
	    else
	    {
            g_tLog.WriteLogConsole ("\n\n User authenticated successfully, procedding to main menu.");
            SLEEP (1500);
            CLEAR
            m_DisplayBanner ();
            m_DisplayMenu ();
	    }
    }
    else
    {
        g_tLog.WriteLogConsole (" Not a Valid Option");
        SLEEP (2500);
        m_Login ();
    }
    
	g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_Login - [%d] - %d", CAMS_SUCCESS, __LINE__);
	return CAMS_OK;
}

//--------------------------------------------------------------
// Function		:	m_AddNewLoginUser	
// Description	:	add a new user to DB
// Arguments	:	none
// Returns		:	CAMS_SUCCESS / CAMS_FAILURE
// Note			:	add the below user fields also
//                  1. UserType -admin/normal
//                  2. Dept
//--------------------------------------------------------------
int CCAMSCLI :: m_AddNewLoginUser ()
{
    g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_AddNewLoginUser");

    // username
    g_tLog.WriteLogConsole (" Enter your desired UserName : ");
    cin >> m_strUserName;
    if (!g_GenUtl.m_ValidateUserResponse (m_strUserName))
    {
        g_tLog.WriteLogConsole (" UserName can not be empty");
        g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_AddNewLoginUser - [%d] - %d", CAMS_FAILURE, __LINE__);
        return CAMS_FAILURE;
    }

    // contact no
    g_tLog.WriteLogConsole (" Enter your Contact Number : ");
    cin >> m_strContactNumber;
    if (!g_GenUtl.m_ValidateUserResponse (m_strContactNumber))
    {
        g_tLog.WriteLogConsole (" Contact Number can not be empty");
        g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_AddNewLoginUser - [%d] - %d", CAMS_FAILURE, __LINE__);
        return CAMS_FAILURE;
    }

    // email address
    g_tLog.WriteLogConsole (" Enter your Email Address : ");
    cin >> m_strEmailAddress;
    if (!g_GenUtl.m_ValidateUserResponse (m_strEmailAddress))
    {
        g_tLog.WriteLogConsole (" Email address should be in valid format.\n");
        g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_AddNewLoginUser - [%d] - %d", CAMS_FAILURE, __LINE__);
        return CAMS_FAILURE;
    }

    // password
    g_tLog.WriteLogConsole (" Enter your Password : ");
    m_strPassword = "";
    m_strPassword = g_GenUtl.m_getPassword ();
    if (!g_GenUtl.m_ValidatePassword (m_strPassword))
    {
        g_tLog.WriteLogConsole (" Minimum password requirement fail");
        g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_AddNewLoginUser - [%d] - %d", CAMS_FAILURE, __LINE__);
        return CAMS_FAILURE;
    }
    else
    {
        g_tLog.WriteLog ("Password Verified SUccessfully !!!");
    }

    m_strNote       =   "CAMS User";
    m_nUserType     =   USER_TYPE_GENERAL;
    m_nStatus       =   STATUS_ACTIVE;
    m_nDeleteMode   =   DELETE_MODE;
    m_nDept         =   DEPT;

    g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_AddNewLoginUser - [%d] - %d", CAMS_SUCCESS, __LINE__);
    return CAMS_SUCCESS;
}

//--------------------------------------------------------------
// Function		:	m_ValidateCredentials	
// Description	:	peforms the first lgoin verification
// Arguments	:	username and password
// Returns		:	CAMS_SUCCESS / CAMS_FAILURE
// Note			:	only one user is supported currenty
//					1. add new user add
//--------------------------------------------------------------
int CCAMSCLI :: m_ValidateCredentials (string strUserName, string strPassword)
{
	g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_ValidateCredentials");

	size_t found;

	g_tLog.WriteLog ("validating the username and password");

	string strUserList = g_dbu._Exec_tbluserdetails (SELECT_TBLUSERDETAILS, "UserName");
	g_tLog.WriteLog ("strUserList = %s", strUserList.c_str());

	// UserName Validation
	found=strUserList.rfind(strUserName);
	g_tLog.WriteLog ("found = %d", found);
	if (found!=string::npos)
	{
		g_tLog.WriteLog ("UserName <%s> found !!!", strUserName.c_str());
		m_bUserNameValid = true;
	}
	else
	{
		g_tLog.WriteLog ("UserName <%s> not found !!!", strUserName.c_str());
	}


	// Password Validation:
    // sensible to go in if username is valid
    g_tLog.WriteLog ("Skipping validation of paasowrd");
    if (m_bUserNameValid)
    {
	    found = 0;

	    string strPasswordList = g_dbu._Exec_tbluserdetails (SELECT_TBLUSERDETAILS, "Password");
	    g_tLog.WriteLog ("strPasswordList = %s", strPasswordList.c_str());

	    found=strPasswordList.rfind(strPassword);
	    g_tLog.WriteLog ("found = %d", found);
	    if (found!=string::npos)
	    {
		    g_tLog.WriteLog ("Password Match Found !!!");
		    m_bPasswordValidated = true;
	    }
	    else
	    {
		    g_tLog.WriteLog ("Password Mismatch !!!");
	    }
    }

	if (m_bUserNameValid && m_bPasswordValidated)
	{
        g_tLog.WriteLog ("setting m_bUserValidated to true");
        
        m_bUserValidated = true;

        g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_ValidateCredentials - [%d] - %d", CAMS_SUCCESS, __LINE__);
		return CAMS_SUCCESS;
	}
	else
	{

		g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_ValidateCredentials - [%d] - %d", CAMS_FAILURE, __LINE__);
		return CAMS_FAILURE;
	}
	g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_ValidateCredentials - [%d] - %d", CAMS_OK, __LINE__);

	return CAMS_OK;
}

//--------------------------------------------------------------
// Function		:	m_tblAssetDetails	
// Description	:	
// Arguments	:	username and password
// Returns		:	CAMS_SUCCESS / CAMS_FAILURE
// Note			:	
//--------------------------------------------------------------
int CCAMSCLI :: m_tblAssetDetails ( )
{
	g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_tblAssetDetails");
	CLEAR
	m_DisplayBanner();

    g_tLog.WriteLogConsole ("\n Supported Assests List");
	cout << endl;
	//cout << " 1. Hardware Assest's" << endl;
	//cout << " 2. Software Assest's" << endl;
    g_tLog.WriteLogConsole (" 1. Hardware Assest's");
    g_tLog.WriteLogConsole (" 2. Software Assest's");

	char cTmp = g_GenUtl.m_getChar();
	g_tLog.WriteLog("cTmp = %c", cTmp);

	if (cTmp == '1')
	{
		CLEAR
		m_DisplayBanner();
		cout << endl;
		cout << "----------------------------------------" << endl;
		cout << "          Hardware Assest's             " << endl;
		cout << "----------------------------------------" << endl;

        m_tblAssetDetails_HardWare ();

		cout << " To go back to Main Menu press [M/m]" << endl;
		cout << " To go back to Previous menu press [R/r]" << endl;
		cTmp = '\0';
		cTmp = g_GenUtl.m_getChar();
		if (cTmp == 'm' || cTmp == 'M')
		{
			// go to main menu
            CLEAR
            m_DisplayBanner ();
			m_DisplayMenu ();
		}
		else if (cTmp == 'R' || cTmp == 'r')
		{
            // return to the previous menu
            CLEAR
            m_DisplayBanner ();
			m_tblAssetDetails();
		}
		else
		{
            m_tblAssetDetails_HardWare ();
		}
	}
	else if (cTmp == '2')
	{
		cout << "----------------------------------------" << endl;
		cout << "          Software Assest's             " << endl;
		cout << "----------------------------------------" << endl;
		m_tblAssetDetails_Software ();

        cout << " To go back to Main Menu press [M/m]" << endl;
		cout << " To go back to Previous menu press [R/r]" << endl;
		cTmp = '\0';
		cTmp = g_GenUtl.m_getChar();
		if (cTmp == 'm' || cTmp == 'M')
		{
			// go to main menu
            CLEAR
            m_DisplayBanner ();
			m_DisplayMenu ();
		}
		else if (cTmp == 'R' || cTmp == 'r')
		{
            CLEAR
            m_DisplayBanner ();
			m_tblAssetDetails();
		}
		else
		{
            m_tblAssetDetails_Software ();
		}

    }
	else
	{
		cout << " Not a valid selection" << endl;
		cout << " To go back to Main Menu press [M/m]" << endl;
		cout << " To go back to Previous menu press [R/r]" << endl;
		cTmp = '\0';
		cTmp = g_GenUtl.m_getChar();
		if (cTmp == 'm' || cTmp == 'M')
		{
			// go to main menu
			m_DisplayMenu ();
		}
		else if (cTmp == 'R' || cTmp == 'r')
		{
			m_tblAssetDetails();
		}
		else
		{

		}
	}

	//g_dbu._Exec_tblAssetDetails (SELCT_TBLASSETDETAILS, "AssetCode");

	g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_tblAssetDetails - [%d] - %d", CAMS_OK, __LINE__);
	return CAMS_OK;
}

//--------------------------------------------------------------
// Function		:	m_tblAssetDetails_HardWare	
// Description	:	
// Arguments	:	
// Returns		:	CAMS_SUCCESS / CAMS_FAILURE
// Note			:	Regular User can only view the assets.
//                  User with admin role can add the assets too
//--------------------------------------------------------------
int CCAMSCLI :: m_tblAssetDetails_HardWare ( )
{
	g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_tblAssetDetails_HardWare");

    int nNumOfRowInTables;
    int nNumAsset = 0;

    g_tLog.WriteLog ("creating query to fetch the no of hardware assets");
 
    string strTmpQuery = "select * from tblAssetDetails";
    g_dbu._Exec_tblAssetDetails (strTmpQuery, ""); // in  case passing empty string assume, we are using the 
    g_dbu.m_GetNumOfAssetRow (strTmpQuery, nNumOfRowInTables);
    g_tLog.WriteLog ("Found a total of [%d] entries", nNumOfRowInTables);


    strTmpQuery = "";
    strTmpQuery = "select AssetCode from tblAssetDetails";
    int nNumOfHardwareAsset = 0;
    g_dbu.m_tblAssetDetails_HardWare_GetNumOfAsset (strTmpQuery, nNumOfHardwareAsset);

    string strAssetCode;
    g_tLog.WriteLogConsole (" S.No	- Asset Name	- Asset Code");
	g_tLog.WriteLogConsole ("----------------------------------------");

    for (; nNumAsset < nNumOfHardwareAsset; nNumAsset ++)
    {
        // check for assed code, if it works make for asset name too
        g_tLog.WriteLogConsole (" [%d]	      Laptop          [%s]    ", nNumAsset+1, (g_dbu.m_vtblAssetDetails_AssetCode.at(nNumAsset)).c_str());
    }
	g_tLog.WriteLogConsole ("----------------------------------------\n");
    g_tLog.WriteLogConsole (" Enter the Asset Code : ");
    cin  >> strAssetCode;

    CLEAR
    m_DisplayBanner ();

    g_tLog.WriteLogConsole (" \nFetching Asset Details for Asset Code %s\n", strAssetCode.c_str());
    SLEEP (1500);

    g_tLog.WriteLog ("Validating user input");
    bool bCheckUserInput = true;
    //for (int nNumAsset = 0; nNumAsset < nNumOfHardwareAsset; nNumAsset ++)
    nNumAsset = 0;
    while (nNumAsset < nNumOfHardwareAsset)
    {
        g_tLog.WriteLog ("Matching [%s] against [%s]", strAssetCode.c_str(), (g_dbu.m_vtblAssetDetails_AssetCode.at(nNumAsset)).c_str());
        //if ((g_dbu.m_vtblAssetDetails_AssetCode.at(nNumAsset))) == strAssetCode)
        string strTmp = g_dbu.m_vtblAssetDetails_AssetCode.at(nNumAsset);
        if (strAssetCode.compare(strTmp) == 0)
        {
            g_tLog.WriteLog ("AssetCode [%s] found in the Database", strAssetCode.c_str());
            bCheckUserInput = true;
        }
        else
        {
            g_tLog.WriteLog ("AssetCode [%s] not found in the Database", strAssetCode.c_str());
            bCheckUserInput = false;
        }
        nNumAsset ++;
        if (bCheckUserInput)
            break;
    }
//#if 0
    // just a basic check of input validation, and collect the result according to that.
    if (bCheckUserInput == true)
    {
    //if (strAssetCode == "001")
    //{
        g_tLog.WriteLog ("Creating Query to fetch the Asset details for Asset Code <%s>", strAssetCode.c_str());
        g_dbu.m_CreatetblAssetDetailsQuery (strAssetCode, "select", "*");
        g_dbu._Exec_tblAssetDetails (g_dbu.m_strCAMSDBSQLQuery, "AssetCode");
        g_tLog.WriteLogConsole (" Asset Details with Asset Code [%s]\n", strAssetCode.c_str());
        g_tLog.WriteLogConsole ("\n ----------------------------------------\n");
        g_tLog.WriteLogConsole ("    Id                 :   %d", g_dbu.m_ntblAssetDetails_Id_Pub);
        g_tLog.WriteLogConsole ("    Asset Code         :   %s", g_dbu.m_strtblAssetDetails_AssetCode_Pub.c_str());
        g_tLog.WriteLogConsole ("    Asset Name         :   %s", g_dbu.m_strtblAssetDetails_AssetName_Pub.c_str());
        g_tLog.WriteLogConsole ("    Asset Group        :   %d", g_dbu.m_ntblAssetDetails_Group_Pub);
        g_tLog.WriteLogConsole ("    Supplier           :   %d", g_dbu.m_ntblAssetDetails_Supplier_Pub);
        g_tLog.WriteLogConsole ("    Support Provider   :   %d", g_dbu.m_ntblAssetDetails_SupportProvider_Pub);
        g_tLog.WriteLogConsole ("    Catalog No         :   %s", g_dbu.m_strtblAssetDetails_CatalogNumber_Pub.c_str());
        g_tLog.WriteLogConsole ("    Description        :   %s", g_dbu.m_strtblAssetDetails_Description_Pub.c_str());
        g_tLog.WriteLogConsole ("    Asset Type         :   %d", g_dbu.m_ntblAssetDetails_AssetType_Pub);
        g_tLog.WriteLogConsole ("    Asset Model        :   %s", g_dbu.m_strtblAssetDetails_Model_Pub.c_str());
        g_tLog.WriteLogConsole ("    Company Serial     :   %s", g_dbu.m_strtblAssetDetails_CompanySerial_Pub.c_str());
        g_tLog.WriteLogConsole ("    Asset Model        :   %s", g_dbu.m_strtblAssetDetails_MonitorSerial_Pub.c_str());
        g_tLog.WriteLogConsole ("\n ----------------------------------------\n");
    }
    //else if (strAssetCode == "002")
    //{
    //    g_tLog.WriteLog ("Creating Query to fetch the Asset details for Asset Code <%s>", strAssetCode.c_str());
    //    g_dbu.m_CreatetblAssetDetailsQuery (strAssetCode, "select", "*");
    //    g_dbu._Exec_tblAssetDetails (g_dbu.m_strCAMSDBSQLQuery, "AssetCode");
    //    g_tLog.WriteLogConsole (" Asset Details with Asset Code [%s]\n", strAssetCode.c_str());
    //    g_tLog.WriteLogConsole ("\n ----------------------------------------\n");
    //    g_tLog.WriteLogConsole ("    Id                 :   %d", g_dbu.m_ntblAssetDetails_Id_Pub);
    //    g_tLog.WriteLogConsole ("    Asset Code         :   %s", g_dbu.m_strtblAssetDetails_AssetCode_Pub.c_str());
    //    g_tLog.WriteLogConsole ("    Asset Name         :   %s", g_dbu.m_strtblAssetDetails_AssetName_Pub.c_str());
    //    g_tLog.WriteLogConsole ("    Asset Group        :   %d", g_dbu.m_ntblAssetDetails_Group_Pub);
    //    g_tLog.WriteLogConsole ("    Supplier           :   %d", g_dbu.m_ntblAssetDetails_Supplier_Pub);
    //    g_tLog.WriteLogConsole ("    Support Provider   :   %d", g_dbu.m_ntblAssetDetails_SupportProvider_Pub);
    //    g_tLog.WriteLogConsole ("    Catalog No         :   %s", g_dbu.m_strtblAssetDetails_CatalogNumber_Pub.c_str());
    //    g_tLog.WriteLogConsole ("    Description        :   %s", g_dbu.m_strtblAssetDetails_Description_Pub.c_str());
    //    g_tLog.WriteLogConsole ("    Asset Type         :   %d", g_dbu.m_ntblAssetDetails_AssetType_Pub);
    //    g_tLog.WriteLogConsole ("    Asset Model        :   %s", g_dbu.m_strtblAssetDetails_Model_Pub.c_str());
    //    g_tLog.WriteLogConsole ("    Company Serial     :   %s", g_dbu.m_strtblAssetDetails_CompanySerial_Pub.c_str());
    //    g_tLog.WriteLogConsole ("    Asset Model        :   %s", g_dbu.m_strtblAssetDetails_MonitorSerial_Pub.c_str());
    //    g_tLog.WriteLogConsole ("\n ----------------------------------------\n");
    //}
    else 
    {
        g_tLog.WriteLogConsole (" Sorry ! This Asset Code not found.");
		g_tLog.WriteLogConsole (" To go back to Main Menu press [M/m]");
		g_tLog.WriteLogConsole (" To go back to Previous menu press [R/r]");

        char cTmp = g_GenUtl.m_getChar();
		cTmp = g_GenUtl.m_getChar();

        if (cTmp == 'm' || cTmp == 'M')
		{
			// go to main menu
			m_DisplayMenu ();
		}
		else if (cTmp == 'R' || cTmp == 'r')
		{
			m_tblAssetDetails_HardWare();
		}
		else
		{
            m_tblAssetDetails_HardWare ();  // whats wrong with user's eye ?, lets give a try again
		}
    }
//#endif // 0
    g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_tblAssetDetails_HardWare - [%d] - %d", CAMS_SUCCESS, __LINE__);

	return CAMS_SUCCESS;
}

//--------------------------------------------------------------
// Function		:	m_tblAssetDetails_Software	
// Description	:	
// Arguments	:
// Returns		:	CAMS_SUCCESS / CAMS_FAILURE
// Note			:	
//--------------------------------------------------------------
int CCAMSCLI :: m_tblAssetDetails_Software ( )
{
	g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_tblAssetDetails_Software");
	//cout << " 1. DCD" << endl;
    g_tLog.WriteLogConsole (" No Software Assets Found !!!");
	g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_tblAssetDetails_Software - [%d] - %d", CAMS_SUCCESS, __LINE__);

	return CAMS_SUCCESS;
}

//--------------------------------------------------------------
// Function		:	m_AboutCAMS	
// Description	:	
// Arguments	:
// Returns		:	None
// Note			:	
//--------------------------------------------------------------
int CCAMSCLI :: m_AboutCAMS ( )
{
	g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_AboutCAMS");
    //char cTemp[28];
    FILE *fp;
    char c;
    string strTemp = ABOUT_CAMS_PATH;
    string strAboutCAMSPath = "..\\.." + strTemp;
    //sprintf (cTemp, "");
    fp = fopen(strAboutCAMSPath.c_str(), "r");
    if (fp == NULL)
	{
        g_tLog.WriteLogConsole ("\n ---------------------------------");
        g_tLog.WriteLogConsole ("\n Missing About Content in About_CAMS.txt");
        g_tLog.WriteLogConsole ("\n ---------------------------------");
    }
    else 
    {
        cout << endl;
        do 
        {
            c = getc(fp); /* get one character from the file*/
            putchar(c); /* display it on the monitor*/
        }
        while (c != EOF); /* repeat until EOF (end of file)*/
    }
    fclose(fp);

    g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_AboutCAMS - [%d] - %d", CAMS_SUCCESS, __LINE__);
    return CAMS_SUCCESS;
}

//--------------------------------------------------------------
// Function		:	m_EnterSleepMode	
// Description	:	
// Arguments	:
// Returns		:	None
// Note			:	
//--------------------------------------------------------------
bool CCAMSCLI :: m_EnterSleepMode ()
{   
    g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_EnterSleepMode");

    //m_nSleepTime = 50000;   // enough time to sleep
    //m_WaitForUserResponse ();
    g_tLog.WriteLogConsole ("\n\nCAMS Version 1.0.0 doesn't support Sleep Mode !!!");

    g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_EnterSleepMode - [%d] - %d", CAMS_SUCCESS, __LINE__);
    return true;
}


//--------------------------------------------------------------
// Function		:	m_DashBoard	
// Description	:	
// Arguments	:
// Returns		:	
// Note			:	
//--------------------------------------------------------------
int CCAMSCLI :: m_DashBoard ()
{
    g_tLog.WriteLog ("ENTER : CCAMSCLI :: m_DashBoard");

    g_tLog.WriteLogConsole ("\n\n-----------------------------------------------------\n");
    g_tLog.WriteLogConsole (" DashBoard feature is not available in Version 1.0.0\n");
    g_tLog.WriteLogConsole ("-----------------------------------------------------\n");

    g_tLog.WriteLog ("EXIT  : CCAMSCLI :: m_DashBoard - [%d] - %d", CAMS_SUCCESS, __LINE__);
    return CAMS_SUCCESS;
}