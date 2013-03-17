 #ifndef _CAMS_QUERY_LIST_H__
 #define _CAMS_QUERY_LIST_H__

#include <string>

/*
 * tbluserdetails queries
 */
 const char * const SELECT_TBLUSERDETAILS	=	"select * from tbluserdetails";
 const char * const SELECT_USERNAME			=	"select UserName from tbluserdetails where id=1";

/*
 * tblAssetDetails queries
 */
 const char * const SELCT_TBLASSETDETAILS	=	"select * from tblAssetDetails";

// add Asset Codes here
 string AssestCode[2] = {"001", "002"};

 #endif //_CAMS_QUERY_LIST_H__