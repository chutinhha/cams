/******************************************************************************/
// File Name		:	CAMS_Instance_Control.h
//
// Author			:	
//
// Project			:	CAMS FRAMEWORK
//
// Source			:	CAMS re-usable modules
//
// Module			:	CAMS AGENT
//
// Purpose			:	singleton class declaration
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

#ifndef __CAMS_INSTANCE_CONTROL_CPP__
#define __CAMS_INSTANCE_CONTROL_CPP__

 class CCAMSInstanceControl
 {
	private:
    		static bool instanceFlag;
    		static CCAMSInstanceControl *single;
    		CCAMSInstanceControl() 
			{
    		}
	public:
    		static CCAMSInstanceControl* getInstance();
    		void method ();
            void cleanup ();
    		~CCAMSInstanceControl() 
			{
        		instanceFlag = false;
    		}
 };
#endif //__CAMS_INSTANCE_CONTROL_CPP__
