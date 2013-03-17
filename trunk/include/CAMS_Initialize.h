/******************************************************************************/
// File Name		:	CAMS_Initialize.h
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS AGENT
//
// Purpose			:	
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
#ifndef __CAMS_INITIALIZE_H__
#define __CAMS_INITIALIZE_H__

class CCAMS_Initialize
{
    public:
        CCAMS_Initialize ();
        ~CCAMS_Initialize();
        void m_initialize_logger ();
        void m_initialize_config_settings ();
};

extern CCAMS_Initialize g_inlz;

#endif //__CAMS_INITIALIZE_H__
