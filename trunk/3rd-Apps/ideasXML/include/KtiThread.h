/******************************************************************************/
// File Name		:	KtiThread.h
//
// Author			:	K. Satheesh Kumar
//
// Project			:	IDEAS FRAMEWORK
//
// Source			:	Kanrad re-usable modules
//
// Module			:	IDEAS AGENT
//
// Purpose			:	Declares the thread base class
//
// Copy Right		:	Copyright © Kanrad Tech, Inc.
//
// Notes			:
//
// History			:
//		Created		:   02/01/2005
//		Version		:	1.0.0.0
//
//
//
/******************************************************************************/

#ifndef	__KTITHREAD_H_
#define	__KTITHREAD_H_
// Platform specific definitions
#include "IdPLatformSpDfns.h"
#include "Log.h"
#include <iostream>
#include <string>
#include <cstdio>



using namespace std;



// Thread states
#define TH_SUSPENDED	0
#define TH_RUNNING		1
#define TH_DEAD			2
#define TH_SIGKILL		3
#define TH_INIT			4
#define TH_SIGSUSPEND	5

extern CLog g_tLog;

class CKtiThread
{
public:
	char*			m_szThreadName;
	THREAD_REF		m_hThreadRef;

	/******************************************************************************/
	// Function name : CKtiThread::CKtiThread
	// Description   : Default Constructor
	/******************************************************************************/

	CKtiThread()
	{
		printf("\nThread default constructor..");
		m_bStartSuspended = false;
		m_szThreadName = "Ideas Agent thread";
		INIT_MUTEX(&m_tSyncMutex);
		m_nLockId = 0;
		m_bLockAcquired = false;
		SetThreadState(TH_INIT);
	}

	/******************************************************************************/
	// Function name : CKtiThread::CKtiThread
	// Description   : Constructor
	/******************************************************************************/

	CKtiThread(char* szThreadName)
	{
		printf("\nThread constructor..");
		m_bStartSuspended = false;
		m_szThreadName = szThreadName;
		INIT_MUTEX(&m_tSyncMutex);
		SetThreadState(TH_INIT);
	}


	/******************************************************************************/
	// Function name : CKtiThread::~CKtiThread
	// Description   : Destructor
	/******************************************************************************/

	virtual ~CKtiThread()
	{
		DELETE_MUTEX(&m_tSyncMutex);
	}

	/******************************************************************************/
	// Function name : CKtiThread::Execute
	// Description   : Pure virtual function that makes this class an ABC.
	//				   Invoked by CKtiThread::Run() to execute thread-specific function.
	// Return        : void
	/******************************************************************************/

	virtual void Execute() = 0;

	/******************************************************************************/
	// Function name : CKtiThread::Cleanup
	// Description   : Pure virtual function to be implemented by derived thread class.
	//				   Invoked by CKtiThread::Stop() to perform cleanup of derived class.
	//
	// Return        : void
	/******************************************************************************/

	virtual void Cleanup() = 0;

	/******************************************************************************/
	// Function name : CKtiThread::PrintStack
	// Description   : Pure virtual function to be implemented by derived thread class
	//				   for debugging purposes. Invoked by CLog::LogStackTrace
	// Arguments	 : @strStackLog - Printable information about this thread to be logged
	//					in a thread-safe manner.
	//
	// Return        : void
	/******************************************************************************/

	virtual void PrintStack(string& strStackLog) = 0;

	/******************************************************************************/
	// Function name : CKtiThread::CreateOSThread
	// Description   : Creates the thread
	// Return        : bool - success/failure
	/******************************************************************************/

	bool CreateOSThread(bool bThreadJoin = false, THREAD_ATTR dwMode = TH_RUNNING, unsigned nSuspendInterval = 0)
	{
		bool bResult = true;
		if(dwMode == TH_SUSPENDED)
		{
			m_bStartSuspended = true;
			m_nSuspendInterval = nSuspendInterval;
		}
		#if defined(WIN32) && !defined(__CYGWIN__)
			m_tSuspendCond = INIT_COND(NULL);
			SECURITY_ATTRIBUTES sa;
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = NULL;
			sa.bInheritHandle = TRUE;
			m_hThreadRef = ::CreateThread(&sa, 0, CKtiThread::ThreadProc, this , TH_RUNNING, &m_dwThreadID);
		#else
			INIT_COND(&m_tSuspendCond);
			int nResult = pthread_create(&m_hThreadRef,NULL,&(CKtiThread::ThreadProc),(void*)this);
			g_tLog.WriteLogWithoutTimeStamp("TRACE..KtiThread: <%d>",nResult);
			if(nResult != 0)
			{
				if(nResult == EAGAIN)
					g_tLog.WriteLogWithoutTimeStamp("Thread creation failed <EAGAIN>");
				else if(nResult == EINVAL)
					g_tLog.WriteLogWithoutTimeStamp("Thread creation failed <EINVAL>");
				else if(nResult == EPERM)
					g_tLog.WriteLogWithoutTimeStamp("Thread creation failed <EPERM>");
				else
					g_tLog.WriteLogWithoutTimeStamp("Thread creation failed <%d>", nResult);

				// SetThreadState(TH_DEAD);
				bResult = false;
			}
		#endif
		if(bResult && !bThreadJoin)
			DETACH_THREAD(m_hThreadRef);

		return bResult;
	}

	/******************************************************************************/
	// Function name : CKtiThread::SuspendOSThread()
	// Description   : Suspends the calling thread. To be voluntarily invoked,
	//				   if there is a need to suspend thread function to wait for I/O.
	//				   Alternately, if CKtiThread::Suspend() was called by an external
	//				   thread changing the state to TH_SIGSUSPEND. This function blocks
	//				   until either CKtiThread::Resume() is called by an external thread
	//				   or time out happens.
	// Arguments	 : @nTimeout[IN] - Time interval(ms) for suspension
	//				   @nLockId[IN] - Caller should pass lockid obtained from AcquireLock().
	//
	// Return        : bool - true, if resume signalled. false, if timed out.
	/******************************************************************************/

	bool SuspendOSThread(int nTimeOutMillis, int nLockId)
	{
		if((nLockId != m_nLockId) || (!m_bLockAcquired))
			return false;

		SetThreadState(TH_SUSPENDED);
		#if defined(WIN32) && !defined(__CYGWIN__)
			UNLOCK_MUTEX(&m_tSyncMutex);
			if(nTimeOutMillis == 0) // 0 = INFINITE WAITING
			{
				printf("\nWait Infinite");
				m_dwCondState = WAIT_COND(m_tSuspendCond);
			}
			else
			{
				printf("\nWait for %d", nTimeOutMillis);
				m_dwCondState = WAIT_COND_TO(m_tSuspendCond, nTimeOutMillis);
			}
			LOCK_MUTEX(&m_tSyncMutex);
		#else
			printf("\nSuspending thread for <%d> milliseconds.\n", nTimeOutMillis);
			if(nTimeOutMillis == 0)
				m_dwCondState = WAIT_COND(&m_tSuspendCond, &m_tSyncMutex);
			else
			{
				struct timeval tvTimeNow;	//Absolute current time
				struct timezone tzTimeZone;  //Timezone
				struct timespec tsTimeOut;   // Input for timedwait

				/* Get the current time */
				gettimeofday (&tvTimeNow, &tzTimeZone);

				/* Calculate the absolute end time  - nTimeOutMillis milli-seconds wait */
				long lTotalMicros = tvTimeNow.tv_usec + ((nTimeOutMillis%1000) * 1000);
				if(lTotalMicros >= 1000000)
				{
					tsTimeOut.tv_sec  = tvTimeNow.tv_sec + nTimeOutMillis/1000 + 1;
					tsTimeOut.tv_nsec = (lTotalMicros-1000000)*1000;
				}
				else
				{
					tsTimeOut.tv_sec  = tvTimeNow.tv_sec + nTimeOutMillis/1000;
					tsTimeOut.tv_nsec = lTotalMicros*1000;
				}
				m_dwCondState = WAIT_COND_TO(&m_tSuspendCond, &m_tSyncMutex, &tsTimeOut);
			}
		#endif
		if(m_dwCurrentState == TH_SUSPENDED)
			SetThreadState(TH_RUNNING);
		UNLOCK_MUTEX(&m_tSyncMutex);
		if(m_dwCondState == TIMEOUT)
			return false;

		return true;
	}

	/******************************************************************************/
	// Function name : CKtiThread::GetThreadState()
	// Description   : Returns the current state of this thread
	// Return        : THREAD_ATTR - DWORD/long
	/******************************************************************************/

	THREAD_ATTR GetThreadState()
    {
	   return m_dwCurrentState;
	}

	/******************************************************************************/
	// Function name : CKtiThread::Stop()
	// Description   : Control function for external threads to cancel this thread.
	//				   Sets thread state to TH_SIGKILL. If thread is in suspended state,
	//				   sets the suspend conditional variable/event. Invokes
	//				   thread-specific Cleanup() of derived class for cleanup.
	// Return        : void
	/******************************************************************************/

	void Stop()
	{
		int nLock = AcquireLock();
		if(m_dwCurrentState == TH_SUSPENDED)
		{
			SetThreadState(TH_SIGKILL);
			SET_COND(m_tSuspendCond);
			ReleaseLock(nLock);
			//Resume(nLock);
			return;
		}
		else if(m_dwCurrentState != TH_DEAD)
		{
			SetThreadState(TH_SIGKILL);
			// KILL_THREAD(m_hThreadRef);
			/*
			* Let derived class object perform cleanup as required ???
			* Cleanup(); // Clean up the derived thread object and its dependencies
			*/
		}
		ReleaseLock(nLock);
	}

	/******************************************************************************/
	// Function name : CKtiThread::Resume()
	// Description   : Resumes the suspended thread by setting the condition/event.
	//
	// Return        : void
	/******************************************************************************/

	bool Resume(int nLockId)
	{
		if((nLockId != m_nLockId) || (!m_bLockAcquired))
			return false;

		m_bLockAcquired = false;
		if(m_dwCurrentState == TH_SUSPENDED)
			SET_COND(m_tSuspendCond);

		UNLOCK_MUTEX(&m_tSyncMutex);
		return true;
	}

	bool ReleaseLock(int nLockId)
	{
		if((nLockId != m_nLockId) || (!m_bLockAcquired))
			return false;

		m_bLockAcquired = false;
		UNLOCK_MUTEX(&m_tSyncMutex);
		return true;
	}

	int AcquireLock()
	{
		LOCK_MUTEX(&m_tSyncMutex);
		m_bLockAcquired = true;
		return (m_nLockId<1000)?++m_nLockId:m_nLockId=1;
	}

	/******************************************************************************/
	// Function name : CKtiThread::ThreadProc
	/******************************************************************************/

	static THREAD_FUNCTION ThreadProc( void* pthis)
	{
		CKtiThread* pThread = static_cast<CKtiThread*>(pthis);

		//Blocks here until suspend interval or
		if(pThread->m_bStartSuspended)
		{
			int nLock	= pThread->AcquireLock();
			pThread->SuspendOSThread(pThread->m_nSuspendInterval, nLock);
		}

		if(pThread !=NULL)
			pThread->Run();

		return NULL;
	}

	/******************************************************************************/
	// Function name : CKtiThread::Run
	/******************************************************************************/

	void Run()
    {

	   SetThreadState(TH_RUNNING);
	   Execute();
	  // DELETE_COND(&m_tSuspendCond);
	   SetThreadState(TH_DEAD);
	   g_tLog.WriteLogWithoutTimeStamp("TRACE..Setting ThreadState_DEAD: ");
	}

protected:
	THREAD_ATTR 	m_dwThreadID;
	THREAD_ATTR		m_dwCurrentState;
	unsigned		m_nSuspendInterval;

	bool  SetThreadState(THREAD_ATTR dwState)
    {
	   //if(m_dwCurrentState != TH_SIGKILL && m_dwCurrentState != TH_DEAD)
	      	//m_dwCurrentState = dwState;
		//else if(m_dwCurrentState == TH_SIGKILL && dwState == TH_DEAD)
		g_tLog.WriteLogWithoutTimeStamp("TRACE...CKtiThread::SetThreadState()<%u>Request State<%d> to<%d> ",m_hThreadRef,m_dwCurrentState,dwState);	
			m_dwCurrentState = dwState;
		
		return true;
	}

private:
	THREAD_ATTR		m_dwCondState;
	cond_var_t		m_tSuspendCond; // HANDLE or pthread_cond_t
	bool			m_bStartSuspended;
	MUTEX_OBJECT	m_tSyncMutex; // Mutex or CriticalSection
	bool			m_bLockAcquired;
	int				m_nLockId;
};


#endif //__KTITHREAD_H_
