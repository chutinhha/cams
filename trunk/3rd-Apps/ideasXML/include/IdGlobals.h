/******************************************************************************
* File Name			:	IdGlobals.h
*
* Author			:	Satheesh Kumar
*
* Project			:	IDEAS
*
* Module			:	IDEAS AGENT
*
* Purpose			:	Macro definitions, Typedefs, Struct definitions
*
* Copy Right		:	Copyright © Kanrad(I)Pvt.Ltd.
*
* Notes			:
*
* History			:
*		Created		:   3/14/05
		Updated		:	7/24/07
*		Version		:	1.0.0.0
*
*
******************************************************************************/


#ifndef __IDGLOBALS_H__
#define __IDGLOBALS_H__
#include "IdTypes.h"

#define CURRENT_VER	    "1.001b"
#define SEM_KEY			0x70
	// Agent Startup modes
#define MODE_MAIN		0
#define MODE_STOP		1
#define MODE_VERSION	2
#define MODE_REMSEM		3
#define MODE_INVALID	-1

#define DEF_LIMIT		5
#define DEF_MAXINT		18000
#define DEF_ASYNCTO		120
#define DEF_TCPTO		300
#define DEF_PROXYTO		1800

#define KEY			    2007 // External identifier to the process control semaphore
#define MAX_DTL_SIZE    2048
#define MAX_EVT_SIZE    4000
#define BUFF_64K   	    65536
#define BUFF_4K   	    4096
#define BUFF_1K   	    1024
#define BUFLEN_512   	512
#define BUFLEN_256		256
#define BUFLEN_128		128
#define BUFLEN_64		64
#define BUFLEN_32		32
#define LISTEN_PORT     7635 // Agent listening port number
#define QLEN            6    // size of request queue

// Thread intervals in milliseconds
#define INT_JOBCHECK	5000
#define INT_HEARTBEAT 	600000

//Settings configuration entries
#define SELF_GUID		"/SETTINGS/CONFIGURATION/self_guid"
#define SELF_NAME		"/SETTINGS/CONFIGURATION/self_name"
#define SELF_ADDRESS	"/SETTINGS/CONFIGURATION/self_address"
#define SELF_IPLAN		"/SETTINGS/CONFIGURATION/self_ip_lan"
#define SELF_IPWAN		"/SETTINGS/CONFIGURATION/self_ip_wan"
#define SELF_DNWAN		"/SETTINGS/CONFIGURATION/self_dn_wan"
#define SELF_PORT		"/SETTINGS/CONFIGURATION/self_listen_port"
#define SELF_OSTYPE		"/SETTINGS/CONFIGURATION/os_family"
#define DUMB_RELATIVE   "/SETTINGS/CONFIGURATION/dumb_relative"
#define PARALLEL_LIMIT	"/SETTINGS/CONFIGURATION/parallel_exec_limit"
#define MAX_DURATION	"/SETTINGS/CONFIGURATION/max_job_duration"
#define ASYNC_TIMOUT	"/SETTINGS/CONFIGURATION/async_worker_timout"
#define CONSOLE_LOG		"/SETTINGS/CONFIGURATION/console_logging"
#define CRON_RECOVERY	"/SETTINGS/CONFIGURATION/cron_track_missed"
#define MAX_FILEREF  	"/SETTINGS/CONFIGURATION/max_fileRef_xchg"
#define MASTER_PATH     "/SETTINGS/MASTER"
#define MASTER_IPLAN    "/SETTINGS/MASTER/iplan"
#define MASTER_IPWAN    "/SETTINGS/MASTER/ipwan"
#define MASTER_DNWAN    "/SETTINGS/MASTER/dnwan"
#define MASTER_PORT		"/SETTINGS/MASTER/port"
#define CHILDREN		"/SETTINGS/CHILDREN"
#define CHILD_PATH      "/SETTINGS/CHILDREN/CHILD"
#define TCP_PROXYPORT	"/SETTINGS/CONFIGURATION/self_proxy_port"
#define TCP_SHELL_VER	"/SETTINGS/CONFIGURATION/shell_version"
#define TCP_TIMEOUT		"/SETTINGS/CONFIGURATION/tcp_session_timeout"
#define MAX_FILEXCHGS   "/SETTINGS/CONFIGURATION/max_fileRef_xchg"

//Settings configuration entries for CAMS Database specific
#define CONN_DATABASE   "/SETTINGS/CONFIGURATION/CONNECTION/DATABASE"
#define CONN_SERVER     "/SETTINGS/CONFIGURATION/CONNECTION/SERVER"
#define CONN_PORT       "/SETTINGS/CONFIGURATION/CONNECTION/PORT"
#define CONN_UID        "/SETTINGS/CONFIGURATION/CONNECTION/UID"
#define CONN_PASSWORD   "/SETTINGS/CONFIGURATION/CONNECTION/PASSWORD"

//Jobs configuration entries
#define CFG_JOBS			"/JOBS"
#define CFG_JOBDEF			"/JOBS/Job"
#define CFG_JOBPROPS		"/Properties"
#define CFG_PLUGINS			"/Plugins/Plugin"
#define CFG_EVENT			"/Events/Event"
#define CFG_WORKER			"/Workers/Worker"
#define CFG_OPER			"/Operation"

//Plugin configuration entries
#define CFG_RULES			"/Plugin/Rules"
#define CFG_RULE			"/Rule"
#define CFG_TASK			"/Task"

#if defined(WIN32) && !defined(__CYGWIN__)
#define	IDEAS_SESS_TOKEN	"..\\logs\\iTRxSession"
#else
#define	IDEAS_SESS_TOKEN	"../logs/iTRxSession"
#endif

//MediaWorker Status
#define ITRX_DB_PATH	"../work/iTRxDB"
typedef enum DB_REC_STATUS {
	DB_RUNNING=0,
	DB_DONE,
	DB_ABORTED,
	DB_DELETING
};

//Coding macros
#define CHECK_DELETE(X)		if(X){delete(X); X=NULL;}

// Device registry entries
#define REG_DATE		"CurrentDate"
#define REG_INSTID		"LastInstanceID"
#define REG_CRONTIME	"LastCronTriggerTime"

#define JOBSTACK_TRACE	"CurrentJob:%s\nCurrentPlugin:%s\nCurrentXML:%s\nCurrentScope:%s\nCurrentRule:%s\n \
						++++++++++++++++++++LOCALS\n++++++++++++++++++++++\n%s\n \
						++++++++++++++++++++GLOBALS\n++++++++++++++++++++++\n%s"


// System Event Types - Standard events generated for all plugins
#define SYSEVT_TRIGGER		0	// Trigger
#define SYSEVT_STARTMAIN	1	// Start of plugin
#define SYSEVT_PREOUT		2	// PreExecutable output
#define SYSEVT_STARTPRE		3	// Starting PreJob
#define SYSEVT_ENDPRE		4	// Exited PreJob
#define SYSEVT_EXCEPTION	5	// Plugin Exception
#define SYSEVT_TIMEOUT		6	// Engine stopping timed out job
#define SYSEVT_STARTPOST	7	// Starting PostJob
#define SYSEVT_ENDPOST		8	// Exited PostJob
#define SYSEVT_POSTOUT		9	// PostExecutable output
#define SYSEVT_ENDMAIN		10	// End of plugin
#define SYSEVT_CHILD		11	// Child Trigger
#define SYSEVT_ABORTRECD	12	// Received request from user to abort a job
#define SYSEVT_ABORT		13	// Abort specified job using its transaction id and target
#define SYSEVT_ABORTMAIN	14	// Main job aborted
#define SYSEVT_ABORTCHILD	15	// Child job aborted


// System-defined plugin event GUID-suffixes (eg.<PluginGUID><Suffix>=xxx01)
#define SFX_TRIGGER			"00"	// Trigger
#define SFX_STARTMAIN		"01"	// Start of plugin
#define SFX_PREOUT			"02"	// PreExecutable output
#define SFX_STARTPRE		"03"	// Starting PreJob
#define SFX_ENDPRE			"04"	// Exited PreJob
#define SFX_EXCEPTION		"05"	// Plugin Exception
#define SFX_JOBTIMEOUT		"06"	// Job timed out hence aborting
#define SFX_STARTPOST		"07"	// Starting PostJob
#define SFX_ENDPOST			"08"	// Exited PostJob
#define SFX_POSTOUT			"09"	// PostExecutable output
#define SFX_ENDMAIN			"10"	// End of plugin
#define SFX_CHILD			"11"	// Child Trigger
#define SFX_ABORTRECD		"12"	// Request to stop a job
#define SFX_ABORT			"13"	// Stop a child job. UMS could use this to stop a main job.
#define SFX_ABORTMAIN		"14"	// Main job aborted
#define SFX_ABORTCHILD		"15"	// Child job aborted

#define ADDR_UMS			"ums"	// Well-known address of UMS Server
#define ADDR_PROXY			"PROXY."	// Well-known address of any PROXY destination. Event's "Target" determine proxy session.

// GUIDs of well-known Event Sources
#define GUID_UMS			"000"	// UMS Event Source
#define GUID_AGENT			"001"   // CRON Event Source
#define GUID_SHELL			"002"   // SHELL Event Source
#define GUID_PROXY			"003"   // External Proxy Event Source

// GUIDs of well-known plugins
#define GUID_RECOVERY		"005"   // Client recovery plugin

// GUIDs of common events generated by all agents
#define GUID_CRON			"001"   // Linux CRON Event Source
#define GUID_HBEAT			"00100" // Heartbeat event
#define GUID_RCVDJOB		"00101" // Received Job Signal
#define GUID_RCVDPLUGIN		"00102" // Received Plugin Trigger
#define GUID_RECOVERED		"00103" // Recovered client doing firstboot
#define GUID_TXERROR		"001TX"	// Transmission error
#define GUID_DEPLOY_ACK		"001DS"	//Agent starting after deployment
#define GUID_DEPLOY_ERR		"001DF"	//Deployment failed
#define GUID_DEPLOYMENT		"000DD"	//Deployment event from UMS
#define GUID_SHUTDOWN       "00109" // Shutdown event
#define GUID_JOBKILLED      "001XX" // send killed job's transaction id  						


// Implicit job name assigned to any system-defined job start
#define SYS_JOB				"SYSTEM JOB"

// Messages for common events
#define MSG_HEARTBEAT		"Agent alive!!"
#define MSG_SIGNALJOB		"Received request to start job [%s]"
#define MSG_TXERROR			"Transmission error while sending event [%s] for [%s]"
#define MSG_TRIGJOB			"Received trigger to start job %s"
#define MSG_TRIGPLUGIN		"Received trigger to start plugin [%s] for job [%s]"
#define MSG_RECOVERED		"Client successfully recovered from disaster by iTRx!!"
#define MSG_SHUTDOWN        "Client Shutdown trigger!!"
#define MSG_JOBKILLED		"%s"

// Messages for System-defined plugin events
#define MSG_STARTMAIN		"Starting Main Job [%s]"
#define MSG_PREOUT			"Output from pre-executable command [%s]\n"
#define MSG_STARTPRE		"Starting Pre Job [%s]"
#define MSG_ENDPRE			"Ended Pre Job [%s]"
#define MSG_EXCEPTION		"Exception reported by job [%s]. Error stack:\n"
#define MSG_JOBTIMEOUT		"Stopping job [%s] due to timeout. Exceeded permitted runtime [%d]!!!"
#define MSG_ABORTRECD		"Received request to stop job [%s]."
#define MSG_STARTPOST		"Starting Post Job [%s]"
#define MSG_ENDPOST			"Ended Post Job [%s]"
#define MSG_POSTOUT			"Output from post-executable command [%s] = [%s]\n"  //GURU: added missing second %s
#define MSG_ENDMAIN			"Ended Main Job [%s]"
#define MSG_ABORTMAIN		"Aborted Main Job [%s]"
#define MSG_ABORTCHILD		"Aborted Child Job [%s]"
#define MSG_ABORTED			"Server aborted Job!!"

// Reference variable syntaxes - Event Parameter map/File references map
#define CHAR_OPEN		'{'
#define CHAR_PARAM		'$'
#define CHAR_OUTPUT		'?'
#define CHAR_FILEREF	'~'
#define CHAR_PLUGINVAR	'@'
#define CHAR_SYSVAR		'%'
#define CHAR_CLOSE		'}'
#define CHAR_NUL		'\0'
#define CHAR_BSLASH		'\\'
#define CHAR_COMMA		','
#define CHAR_EQUAL		'='

// System defined job variables
#define SYSVAR_JOB		"%JobName"
#define SYSVAR_TRANSID	"%TransactID"
#define SYSVAR_NUMID	"%NumericID"
#define SYSVAR_GUID		"%SelfGuid"
#define SYSVAR_NAME		"%SelfName"
#define SYSVAR_ADDR		"%SelfAddress"
#define SYSVAR_LOOP		"%LoopVar"
#define SYSVAR_DATE		"%Date"
#define SYSVAR_TIME		"%Time"
#define SYSVAR_DTIME	"%DateTime"
#define SYSVAR_ERROR	"%Exception"
#define SYSVAR_PATHSEP	"%PATH_SEP"
#define SYSVAR_ROOT		"%INSTALL_ROOT"
#define SYSVAR_LANIP	"%LanIP"
#define SYSVAR_WANIP	"%WanIP"
#define SYSVAR_WANDN	"%WanDN"
#define SYSVAR_PORT		"%ListenPort"
#define SYSVAR_OSTYPE	"%OS_TYPE"
#define SYSVAR_ACTIVEFILEXCHGS 	"%Active_File_Xchgs"
#define SYSVAR_FILEREF 	"%fileRef_xchg"

// Internal system workers
#define WKR_CORE		"sys-core"
#define WKR_SHELL		"sys-shell"
#define WKR_STRING		"sys-string"
#define WKR_FILE		"sys-file"
#define WKR_MATH		"sys-math"
#define WKR_MEDIA       "sys-media"
#define WKR_XBSA		"sys-xbsa"

// Internal worker functions
#define FN_SUSPEND		"suspend-job-thread" // Suspends main job-thread
#define FN_PEEK			"async-peek"
#define FN_OUTPUT		"async-output"
#define FN_OUTCOME		"async-outcome"
#define FN_RESUME		"async-resume" // Resumes a worker-thread suspended by its worker.
#define FN_RELEASE		"async-release"
#define FN_EXEC			"exec-command"
#define FN_CALLPROXY	"invoke-proxy"
#define FN_OPENFILE		"open-shared-file"
#define FN_OPMULTIPART	"open-multipart-file"
#define FN_WRITEFILE	"write-shared-file"
#define FN_READFILE		"read-shared-file"
#define FN_CLOSEFILE	"close-shared-file"

// Other standard job registry variables
#define VAR_PREEXEC		"@PreResult"
#define VAR_POSTEXEC	"@PostResult"
#define VAR_PROXYEXEC	"@ProxyResult"
#define VAR_STATE		"@State"
#define VAR_RESULT		"@Result"

// Standard storage specifiers
#define STORE_PRE		"@PreResult={?Result}"
#define STORE_POST		"@PostResult={?Result}"
#define STORE_PROXY		"@ProxyResult={?Result}"

// Trigger parameter specs for standard jobs
#define PARAM_RECOVERY	"$Client=%s"

// Agent thread identifiers
#define TH_CRON			"Cron Service"
#define TH_ENGINE		"Engine Thread"
#define TH_JOB			"Job Thread"
#define TH_WORKER		"Worker Thread"

// Event types
#define EV_SYS			1 // System event - For local logging only
#define EV_INFO			2 // Data event - Information
#define EV_ERROR		3 // Data event - Error
#define EV_ALERT		4 // Data event - Alert
#define EV_START		5 // Trigger a plugin and let it run standalone

// Severity levels
#define SE_LOW			1
#define SE_HIGH			2
#define SE_CRITICAL		3

// Priority levels
#define PR_LOW			1
#define PR_NORMAL		2
#define PR_HIGH			3
#define PR_VHIGH		4

// Event dispatch Levels
#define DL_NONE			1 // Donot report any events to UMS
#define DL_ERROR		2 // Reports all data events to UMS, if there's at least one error during plugin execution
#define DL_ALERT		3 // Reports all data events to UMS, if there's at least one alert/error
#define DL_ALL			4 // Reports all data and trigger events

// Execution modes
#define EX_AUTO			1
#define EX_SSAUTO		2
#define EX_SSDEBUG		3


// System error codes
#define ERR_ENGINE_INIT		100
#define ERR_XML_LOAD		101

// Possible sub-jobs within a Job Object
#define PRE_JOB			1
#define ACTUAL			2
#define POST_JOB		3

// Worker lifecycle models
#define LC_SINGLETON			1 //Single global instance
#define LC_SINGLE_JOB			2 //Only single instance permitted within a job thread
#define LC_SINGLE_USE			3 //New instance created for every worker function call

// File open modes
#define MOD_READ			1
#define MOD_OVERWRITE		2
#define MOD_APPEND			3

#define READ_MORE			1
#define READ_END			2
#define READ_ERROR			-1

#define MAX_FILEBUF		131072
#define MAX_FILESIZE	2147483648


/*
//GURU: added extern declarations for global variables here.
//class CIdWorkMgr;
//class CLog;
//extern CIdWorkMgr g_tWorkMgr;
//extern CLog g_tLog;
//GURU

#define INSTALL_ROOT "."
#define CONFIG_DIR "/Agent/config"
#define CONFIG_FILE_NAME "/settings.xml"
#define TEMP_FILE_PATH "../temp"
#define PKT_PATH "../Transport/Backup/%u.pkt"
#define TRALOG_PATH		"../Transport/Active/TransportLog"
#define FILE_REFE_LOG_PATH "../Transport/Active/MasterLog"


//ANOOP:added error and status codes for TX_Threads
	  //:added packet types
*/

#define TX_ERR_CONNECT    400
#define TX_ERR_ACCEPT     403 //(For some reason, inbound thread receiving hits but when it attempts to accept, it fails and hence no xchange threads getting started.)
#define TX_ERR_ALIEN      405 //(Not containing proper ideas-hdr)
#define TX_ERR_TIMEDOUT   402 //(Say, expected response didn't arrive from the other end and the xchange thread got terminated by comm worker).
#define TX_ERR_BRKCONN	  406 //(Connection broken)

#define TX_ERR_CHECKSUM   401 //(Invalid cs='##' in packet string)
#define TX_ERR_FORMAT	  404 //(General Syntax error in packet string format)
#define TX_ERR_VERSION	  407 //(Wrong version of ideas header)
#define TX_ERR_PKT_TYPE	  408 //(Invalid packet type)
#define TX_ERR_SOCKERR	  409 //(recv/send error)

	//There need to be other status codes for other states of xchange, like below (order them as needed) -
#define TX_CREATED         421
#define TX_CONNECTED       422
#define TX_WAITING         423 (Say, if any blocking reads used, set to this state before making the call)
#define TX_SUCCESS         424 (If packet was properly sent/received successfully)

//Used by BOTH FileTransmitter and FileReceiver
//#define FILE_CHUNK_SIZE 25600
#define FILE_FAIL		0
#define FILE_SUCCESS	1
#define FILE_SOCK_FAIL  2
#define FILE_SKIP		3


	//Packet Types
#define PKT_TYPE_ALIEN		0
#define PKT_TYPE_DAT		1
#define PKT_TYPE_ERR		2
#define PKT_TYPE_ACK		3
#define PKT_TYPE_REQ		4
#define PKT_TYPE_UID		5
#define PKT_TYPE_FILE_CHNK  6

#define IDEAS_DATA_VER   "V1.1"

//ANOOP
#include <string>
//#include "Log.h"
#include "IdDeliUtility.h" //for IdEvent::load/store


struct IdEvent
{

	//Deployment Flag .Should be '0' when an event is created.
	//For deployment event commLayer will update this member.
	unsigned int uDeploy;

	// GUID of Ideas Event (char[5] in 36-base number system) - 00000 to ZZZZZ
	std::string strEventID;

	// GUID of Host that generated this event
	std::string strHostGUID;

	// Event Type - 1=Sys/2=Info/3=Error/4=Alert/5=Trigger
	unsigned uType;

	// Severity level - 1=Low/2=High/3=Critical - relevant for ERROR type only
	unsigned uSeverity;

	// Priority level - 1 to 4 - For Info/Error/Alert events, it will be the same
	// as priority of the trigger event that initiated the plugin execution
	// (Used by COMM layer while transporting data events)
	unsigned uPriority;

	// Ideas-address of source agent that generated this event
	std::string strOrigAddr;

	// Ideas-address of target agent -
	// Info/Alert/Error types generated by any parent plugin - ums
	// Info/Alert/Error types generated by any child plugin - address of parent
	// Trigger events - Address of the destination agent to execute the trigger (maybe self)
	std::string strDestAddr;

	// Name of the parent job that has executed the current plugin
	// Inherited by all child plugins as well.
	std::string strJobName;

	// Relevant only for blocking trigger events generated during a plugin execution.
	// Append the current PluginID to the existing value of strWaitToken within current plugin.
	// If the target plugin of new wait trigger already exists in the existing
	// strWaitToken, donot invoke it - rather generate a 'cyclic-wait error' and 'plugin
	// termination' events addressed to either "ums" or the waiting parent plugin,
	// if it started due to another wait trigger. Prevents dead-lock situtations where-in
	// waiting plugins cross reference into an infinite loop.
	std::string strWaitToken;

	// GUID of source that generated the event - Either ums, cron or plugin.
	// GUID for ums - 000, GUID for cron - 001, GUID for plugins - XXX
	std::string strSource;

	// GUID of target that consumes the event - Either ums or plugin.
	// GUID for ums - 000, GUID for plugins - XXX
	std::string strTarget;

	// Contains the transaction ID associated with plugin execution instance. The
	// originating agent or UMS that generates a start trigger assigns a unique ID
	// (<ClientGUID>.<current day>.<seqno>) for this field. All subsequent child/query/wait
	// triggers, info/alert/error events generated by the parent as well as children
	// will inherit the originator's TransactID.
	std::string strTransactID;

	// Detailed diagnostics
	std::string strDetails;

	// Named parameter values as name-value pairs. For Query triggers, only parameter
	// names would be passed with empty values. The returned wait trigger parameters
	// would contain name with value.
	std::string strParameters;

	// Name-value pairs contain <origin-file-path> & <destination-file-path>
	std::string strFileRefs;

	// Date and time of event generation	Date time
	std::string strCreateTime;

bool Store(unsigned int uIndexPkt)
	 {

		CIdDeliUtility tDeliUti;
		std::fstream  m_hFile;
		char szFileName[100]={0};
		SNPRINTF(szFileName,sizeof(szFileName),FPATH_PKT,uIndexPkt);
		string strBuff="";

		m_hFile.open(szFileName,ios::app | ios::out);

		if (!m_hFile)
			{
				//cout<< "Unable to open file: "<<szFileName<<endl;

				return false;
    		}

		//Create delimited string from variables
		tDeliUti.AddBegDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(uDeploy);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff+= tDeliUti.GetUnDeliFld(strEventID,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff+= tDeliUti.GetUnDeliFld(strHostGUID,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(uType);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(uSeverity);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(uPriority);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strOrigAddr,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strDestAddr,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strJobName,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strWaitToken,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strSource,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strTarget,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strTransactID,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strDetails,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strParameters,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strFileRefs,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strCreateTime,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI); //Not added

		tDeliUti.AddEndDeli(strBuff,EVT_DELI);

	//	cout<<"DEli"<<strBuff<<endl;
		m_hFile<<strBuff;
		m_hFile.close();
		return true;
	 }

	 bool Store(string& strEscStr)
	 {

		CIdDeliUtility tDeliUti;
		string strBuff="";

		//Create delimited string from variables
		tDeliUti.AddBegDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(uDeploy);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff+= tDeliUti.GetUnDeliFld(strEventID,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff+= tDeliUti.GetUnDeliFld(strHostGUID,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(uType);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(uSeverity);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(uPriority);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strOrigAddr,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strDestAddr,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strJobName,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strWaitToken,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strSource,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strTarget,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strTransactID,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strDetails,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strParameters,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strFileRefs,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI);

		strBuff += tDeliUti.GetUnDeliFld(strCreateTime,FLD_DELI,EVT_DELI);
		tDeliUti.AddEndDeli(strBuff,FLD_DELI); //Not added

		tDeliUti.AddEndDeli(strBuff,EVT_DELI);

		strEscStr = strBuff; //Add an Event to the Input string

		return true;
	 }

void Load(string& strUnEscStr )
	{
		CIdDeliUtility tDeliUti;
		Fields tFields=tDeliUti.EscFields(strUnEscStr,FLD_SIZ,NUM_EVT_FLDS,FLD_DELI,EVT_DELI);
		int cnt=0;
		uDeploy  = atoi(tFields[cnt++].c_str());
		strEventID = tFields[cnt++];
		strHostGUID = tFields[cnt++];
		uType=atoi(tFields[cnt++].c_str());
		uSeverity=atoi(tFields[cnt++].c_str());
		uPriority=atoi(tFields[cnt++].c_str());
		strOrigAddr = tFields[cnt++];
		strDestAddr = tFields[cnt++];
		strJobName = tFields[cnt++];
		strWaitToken = tFields[cnt++];
		strSource= tFields[cnt++];
		strTarget= tFields[cnt++];
		strTransactID = tFields[cnt++];
		strDetails= tFields[cnt++];
		strParameters= tFields[cnt++];
		strFileRefs= tFields[cnt++];
		strCreateTime = tFields[cnt++];
	}
};

struct CompareEventPriority
{
	bool operator () (IdEvent &a, IdEvent & b)
	{
		bool flag = (a.uPriority < b.uPriority )? true: false;
		return flag;
	}
};

struct CompareEventPriority2
{
	bool operator () (IdEvent* a, IdEvent* b)
	{
		bool flag = (a->uPriority < b->uPriority )? true: false;
		return flag;
	}
};

#endif //__IDGLOBALS_H__
