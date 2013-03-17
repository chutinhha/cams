/******************************************************************************/
// File Name		:	CAMS_Debug_File.cpp
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS AGENT
//
// Purpose			:	Debug module for CAMS
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

#include "CAMS_Debug_File.h"

using namespace std;

		int CCAMSDebugFile :: test ()
		{
			pFile = fopen ("myfile.txt","w");
			for (n=0 ; n<3 ; n++)
			{
				fprintf (pFile, "ccplusplus.com");
			}
			fclose (pFile);
			return 0;
		}
