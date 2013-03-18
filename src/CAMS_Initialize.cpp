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
// Purpose			:	use this class as a friend to initialize the required modules.
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
#include "IdGlobals.h"
#include "CAMSDBUtil.h"
#include "CAMSGlobal.h"
#include "CAMS_Initialize.h"

#include "IdXmlWkr.h"
#include "IdXmlUtility.h"

#include <iostream>

extern class CLog g_tLog;
extern class CCAMSDBUtil g_dbu;
extern class CCAMS_Initialize g_inlz;

using namespace std;

CCAMS_Initialize :: CCAMS_Initialize ()
{
}

CCAMS_Initialize :: ~CCAMS_Initialize ()
{
}

/*----------------------------------------------------------------------------
 * Function     :   m_initialize_logger
 * Description  :   Initialize the logging framework
 * Arguments    :   none
 * Return       :   none
 * Note         :
 *----------------------------------------------------------------------------*/
 void CCAMS_Initialize :: m_initialize_logger ()
 {
	g_tLog.Open("../../log/", false);
 }

/*----------------------------------------------------------------------------
 * Function     :   m_initialize_config_settings
 * Description  :   Initialize the cams global setting before start.
 * Arguments    :   none
 * Return       :   none
 * Note         :
 *----------------------------------------------------------------------------*/
 void CCAMS_Initialize :: m_initialize_config_settings ()
 {
    g_tLog.WriteLog ("ENTER : CCAMS_Initialize :: m_initialize_config_settings");

	CIdXmlUtility xmlu;

    g_tLog.WriteLog ("Opening Configuration Settings file <%s>", CAMS_CONFIG_SETTINGS);

	xmlu.OpenXMLFile (CAMS_CONFIG_SETTINGS);

    xmlu.GetValue (CONN_DATABASE, g_dbu.m_strDataBase);
    g_tLog.WriteLog ("Found database = %s", (g_dbu.m_strDataBase).c_str());

    xmlu.GetValue (CONN_SERVER, g_dbu.m_strServer);
    g_tLog.WriteLog ("Found server = %s", (g_dbu.m_strServer).c_str());

    xmlu.GetValue (CONN_PORT, g_dbu.m_strPort);
    g_tLog.WriteLog ("Found port = %s", (g_dbu.m_strPort).c_str());

    xmlu.GetValue (CONN_UID, g_dbu.m_strUID);
    g_tLog.WriteLog ("Found uid = %s", (g_dbu.m_strUID).c_str());

    xmlu.GetValue (CONN_PASSWORD, g_dbu.m_strPWD);
    g_tLog.WriteLog ("Found password, skipping print for security reason");

    g_tLog.WriteLog ("EXIT  : CCAMS_Initialize :: m_initialize_config_settings");
 }

