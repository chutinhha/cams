#ifndef __IDTYPES_H__
#define __IDTYPES_H__
#include "Log.h"
#include "IdFileUtils.h"

enum __id_return_t
{
  IERR_SUCCESS = 0,		/* successful exit */
  IERR_FAILURE = 1,		/* general failure */
  IERR_ALLOC = 2,		/* memory allcocation failure */
  IERR_NOTFOUND = 3,		/* not found error */
  IERR_SYSCALL = 4,		/* failure */
  IERR_TCP = 5,			/* tcp failure */
  IERR_UDP = 6,			/* udp failure */
  IERR_ICMP = 7,		/* icmp failure */
  IERR_DATABASE = 8,		/* database error */
  IERR_INVALID_IP = 9,		/* invalid IP address */
  IERR_SERVER_NOT_RESPOND = 10,	/* server not responding */
  IERR_CLIENT_NOT_RESPOND = 11,	/* client not responding */
};

enum __icmp_type_t
{
 ICMP_TYPE_ECHO_REPLY=0,
 ICMP_TYPE_DEST_NOT_REACH=3,
 ICMP_TYPE_SOURCE_QUENCH=4,
 ICMP_TYPE_REDIRECT=5,
 ICMP_TYPE_ECHO=8,
 ICMP_TYPE_ROUTER_ADVT,
 ICMP_TYPE_ROUTER_SELECT,
 ICMP_TYPE_TIME_EXCEEDED,
 ICMP_TYPE_PARAMETER_PROBLEM,
 ICMP_TYPE_TIME_STAMP,
 ICMP_TYPE_TIME_STAMP_REPLY,
 ICMP_TYPE_INFO_REQUEST,
 ICMP_TYPE_INFO_REPLY,
 ICMP_TYPE_ADDR_MASK_REQ,
 ICMP_TYPE_ADDR_MASK_REPLY,
 ICMP_TYPE_TRACEROUTE=30
};

enum __icmp_code_t
{
ICMP_NET_UNREACHABLE,
ICMP_HOST_UNREACHABLE,
ICMP_PROTOCOL_UNREACHABLE,
ICMP_PORT_UNREACHABLE,
ICMP_FRAGMENTATION_REQUIRED,
ICMP_SOURCE_ROUTE_FAILED,
ICMP_DESTINATION_NETWORK_UNKNOWN,
ICMP_DESTINATION_HOST_UNKNOWN,
ICMP_SOURCE_HOST_ISOLATED,
ICMP_NETWORK_ADMINISTRATIVELY_PROHIBITED,
ICMP_HOST_ADMINISTRATIVELY_PROHIBITED,
ICMP_NETWORK_UNREACHABLE_FOR_TOS,
ICMP_HOST_UNREACHABLE_FOR_TOS,
ICMP_COMMUNICATION_ADMINISTRATIVELY_PROHIBITED
};

typedef enum __id_return_t id_return_t;
typedef enum __icmp_code_t icmp_code_t;
typedef enum __icmp_type_t icmp_type_t;

#if defined(WIN32) && !defined(__CYGWIN__)
	#define IDEBUG(format, ...) do { if ( CIdFileUtils::ideas_debug_enabled() )	g_DebugLog.WriteLogWithoutTimeStamp(format, __va_args__); }while (0)
	#define ILOG(format, ...) do { g_tLog.WriteLogWithoutTimeStamp(format, __VA_ARGS__); }while (0)
#else
	#define IDEBUG(format, args...) do { if ( CIdFileUtils::ideas_debug_enabled() )	g_DebugLog.WriteLogWithoutTimeStamp(format, ## args); g_tLog.WriteLogWithoutTimeStamp(format, ## args); }while (0)
	#define ILOG(format, args...) do { g_tLog.WriteLogWithoutTimeStamp(format, ## args); }while (0)
	#endif
#endif
