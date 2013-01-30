/******************************************************************************/
// File Name		:	CAMS_Gen_Utility.cpp
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS AGENT
//
// Purpose			:	common purpose utility for CAMS
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
#include "CAMSGlobal.h"
#include "CAMS_Gen_Utility.h"
#include "CAMS_Credential_Utility.h"

#include <ctype.h>
#include <fstream>
#include <conio.h>      //provides non standard getch() function
#include <string.h>
#include <iostream>

extern class CLog g_tLog;

using namespace std;

CCAMSGenUtility :: CCAMSGenUtility ()
{
    m_bPasswordStatus   =   false;
}

CCAMSGenUtility :: ~CCAMSGenUtility ()
{
}

//--------------------------------------------------------------
// Function		:	m_getChar
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
char CCAMSGenUtility :: m_getChar()
{
	g_tLog.m_bConsoleLog = false;
	g_tLog.WriteLog ("ENTER : CCAMSGenUtility :: m_getChar");
	m_cUserInput = GETCHE
	g_tLog.WriteLog (" Received <%c> from user", m_cUserInput);
	if (m_cUserInput == '\n')
	{
		cout << "Enter Command is not a valid input." << endl;
		g_tLog.WriteLog (" Enter Command is not a valid input.");
		m_getChar ();
	}
	
	g_tLog.WriteLog ("EXIT  : CCAMSGenUtility :: m_getChar");
	return m_cUserInput;
}

//--------------------------------------------------------------
// Function		:	m_getPassword
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
string CCAMSGenUtility :: m_getPassword()
{
	g_tLog.m_bConsoleLog = false;
	g_tLog.WriteLog ("ENTER : CCAMSGenUtility :: m_getPassword");
	string strPassword;
	while(true)
	{
		char ch = _getch();

        if(ch=='\r')    //when a carriage return is found [enter] key
		{		 
            break;
        }

        strPassword+=ch;             
        cout << "*";
    }
	GETCH
	g_tLog.WriteLog ("EXIT  : CCAMSGenUtility :: m_getPassword");
	return strPassword;
}

//--------------------------------------------------------------
// Function		:	m_ValidatePassword
// Description	:   validate the user password
// Arguments	:   user supplied password
// Returns		:   success/failure
// Note			:   1. add the logic to make the password strong
//              :   2. write to a tmp file instead of log file
//                     for verification purpose
//--------------------------------------------------------------
int CCAMSGenUtility :: m_ValidatePassword(const string const strPassword)
{
    g_tLog.WriteLog ("ENTER : CCAMSGenUtility :: m_ValidatePassword");

    int nPasLength = strPassword.length ();
    g_tLog.WriteLog (" password length = <%d>", nPasLength);

    if (nPasLength <= MIN_PASSWORD_LENGTH && nPasLength <= MAX_PASSWORD_LENGTH)
    {
        g_tLog.WriteLogConsole (" Minimum password length should be 6.");
        g_tLog.WriteLog ("EXIT  : CCAMSGenUtility :: m_ValidatePassword - %d", CAMS_FAILURE);
        return CAMS_FAILURE;
    }
    else
    {
        g_tLog.WriteLog (" Password length is OK");
        m_bPasswordStatus = true;
    }

#if 0
    g_tLog.WriteLog (" Opening paassword file");
    ofstream pwd ("password");
    if (pwd.is_open())
    {
        g_tLog.WriteLog (" password file open");
    }
    else
    {
        g_tLog.WriteLog (" Unable to open password manipulation file");
    }
    pwd.close();
#endif

    if (m_bPasswordStatus)
    {
        g_tLog.WriteLog ("EXIT  : CCAMSGenUtility :: m_ValidatePassword - %d", CAMS_SUCCESS);
        return CAMS_SUCCESS;
    }
    else
    {
        g_tLog.WriteLog ("EXIT  : CCAMSGenUtility :: m_ValidatePassword - %d", CAMS_FAILURE);
        return CAMS_FAILURE;        
    }

    return CAMS_OK; // stop compiler to shout on us :)
}

//--------------------------------------------------------------
// Function		:	m_ValidateUserResponse
// Description	:	validate the user response
// Arguments	:	character to validate
// Returns		:	true (match)/false(mismatch)
// Note			:	add multiple match option
//--------------------------------------------------------------
bool CCAMSGenUtility :: m_ValidateUserResponse(char cRequired)
{
  	g_tLog.WriteLog ("ENTER : CCAMSGenUtility :: m_ValidateUserResponse");

    char cReceived;
	cRequired = '\0';

	cout << " Please enter your choice :";
	while (true)
	{
		cReceived = _getch();
		if (cReceived == cRequired)
		{
            g_tLog.WriteLog ("EXIT  : CCAMSGenUtility :: m_ValidateUserResponse - %d", true);
			return true;
		}
		else
		{
			cout << "\nNot a valid option\n" << endl;
            g_tLog.WriteLog ("EXIT  : CCAMSGenUtility :: m_ValidateUserResponse - %d", false);
			return false;
		}
	}
    g_tLog.WriteLog ("EXIT  : CCAMSGenUtility :: m_ValidateUserResponse - %d", true);
	return true;
}

//--------------------------------------------------------------
// Function		:	m_ValidateUserResponse
// Description	:	validate the user response
// Arguments	:	string to validate
// Returns		:	true (match)/false(mismatch)
// Note			:	add multiple match option
//--------------------------------------------------------------
bool CCAMSGenUtility :: m_ValidateUserResponse(string strRequired)
{
    g_tLog.WriteLog ("ENTER : CCAMSGenUtility :: m_ValidateUserResponse");
	while (true)
	{
		if (strRequired == "")
		{
            g_tLog.WriteLog ("EXIT  : CCAMSGenUtility :: m_ValidateUserResponse - %d", false);
			return false;
		}
		else
		{
            g_tLog.WriteLog ("EXIT  : CCAMSGenUtility :: m_ValidateUserResponse - %d", true);
			return true;
		}
	}
	return true;
}

//--------------------------------------------------------------
// Function		:	m_getString
// Description	:
// Arguments	:
// Returns		:
// Note			:
//--------------------------------------------------------------
string CCAMSGenUtility :: m_getString()
{
    return "";
}