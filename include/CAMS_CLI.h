/******************************************************************************/
// File Name		:	CAMS_CLI.h
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS AGENT
//
// Purpose			:	CLI Class declaration
//
// Copy Right		:	
//
// Notes			:
//
// History			:
//		Created		:   02/09/2012
//		Version		:	1.0.0.0
//
//
//
/******************************************************************************/

#ifndef __CAMS_CLI__
#define __CAMS_CLI__

class CCAMSCLI
{
	private:
		string m_strAdminPhrase;
		bool m_bAdminRole;
		bool m_bUserNameValid;
		bool m_bPasswordValidated;
		bool m_bUserValidated;
        bool m_bUserAddStatus;
        int m_nSleepTime;

        static bool m_bOutSignal;

        // tbluserdetails, new user registration variables
		string m_strUserName;
        string m_strContactNumber;
        string m_strEmailAddress;
        int m_nUserType;
		string m_strPassword;
        string m_strNote;
        int m_nStatus;
        int m_nDeleteMode;
        int m_nDept;

        static int m_sLoginAttemptCount;

    public:
		CCAMSCLI ();
		~CCAMSCLI();

        void m_DisplayUserCommandList ();
		void m_DisplayBanner ();
		int m_ParseCommandLineArgument (int, char**);
		bool m_WaitForUserResponse ();
        bool m_GetQuickUserResponse ();
        bool m_GetQuickUserResponse (char);
		void m_DisplayMenu ();
		bool m_checkAdminUser ();
		int m_Login();
        int m_AddNewLoginUser ();
		int m_ValidateCredentials (string, string);
		int m_tblAssetDetails ();
		int m_tblAssetDetails_HardWare();
		int m_tblAssetDetails_Software();
        void m_AssetMenu ();
        int m_AboutCAMS ();
        bool m_EnterSleepMode ();
        int m_DashBoard ();

		// these function are display menu related,should not br used else where, they may not be functional
};

extern CCAMSCLI *cli;

#endif //__CAMS_CLI__