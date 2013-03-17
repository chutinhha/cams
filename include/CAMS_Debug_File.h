/******************************************************************************/
// File Name		:	CAMS_Debug_File.h
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS AGENT
//
// Purpose			:	debug module class
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
#include <stdio.h>

class CCAMSDebugFile
{
	private:
		FILE * pFile;
		int n;
		char name [100];

	public:
		CCAMSDebugFile ()
		{
		}
		~CCAMSDebugFile ()
		{
		}

		int test ();

};