/******************************************************************************/
// File Name		:	CAMS_Gen_Utility.h
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS AGENT
//
// Purpose			:	General purpose utility
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

#ifndef __CAMS_GEN_UTILITY_H__
#define __CAMS_GEN_UTILITY_H__

#include <string.h>

class CCAMSGenUtility
{
	private:
		char m_cUserInput;
        bool m_bPasswordStatus;

	public:
		CCAMSGenUtility();
		~CCAMSGenUtility();

	public:
		char m_getChar ();
		string m_getPassword ();
        int m_ValidatePassword (const string const);
		bool m_ValidateUserResponse (char);
        bool m_ValidateUserResponse (string);
        string m_getString ();
};

extern CCAMSGenUtility g_GenUtl;

#endif // __CAMS_GEN_UTILITY_H__
