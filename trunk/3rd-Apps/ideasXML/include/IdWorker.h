/******************************************************************************
* File Name			:	IdWorker.h
*
* Author			:	Satheesh Kumar
*
* Project			:	IDEAS
*
* Module			:	IDEAS AGENT
*
* Purpose			:	Defines the IDEAS worker abstract base class. Specialized
*						implementations of this class can participate in a
*						Plugin/Workflow defined to achieve a business function.
*
*
* Copy Right		:	Copyright Kanrad(I)Pvt.Ltd.
*
* Notes			:
*
* History			:
*		Created		:   3/14/05
*		Version		:	1.0.0.0
*
*
******************************************************************************/

#ifndef __IDWORKER_H__
#define __IDWORKER_H__

#include "IdGlobals.h"
#include "IdEntityMap.h"
#include "KtiThread.h"
#include "Log.h"

using namespace std;

extern CLog g_tLog;
// Outcome types
#define NEGATIVE		0
#define POSITIVE		1
#define RUNNING			2
#define WAITING			3
#define READY			4
#define ABORTED			5
#define EXCEPTION		-1

// Output setting modes
#define OVERWRITE		1 // Overwrites previous value, if any
#define LT_APPEND		2 // Appends new output to the left of existing output, if any
#define RT_APPEND		3 // Appends new output to the right of existing output, if any

class CIdWorker
{
public:
	CIdWorker()
	{
		INIT_MUTEX(&m_tWorkerLock);
		m_nLifeCycleType	= LC_SINGLE_USE;
		m_strOutputs		= "";
	}

	virtual ~CIdWorker()
	{
		DELETE_MUTEX(&m_tWorkerLock);
	}

	void SetWorkerAttributes(const char* szModName, bool bLibWorker, int nLifeCycleType)
	{
		m_strModName		= szModName;
		m_bLibWorker		= bLibWorker;
		m_nLifeCycleType	= nLifeCycleType;
	}

	/******************************************************************************
	* Function name	  : CIdWorker:Prepare
	* Description  	  : Validates inputs, initializes member attributes, output map.
	*
	* Arguments		  : @szFunction[IN] - Function/Operation name to be invoked.
	*					@szType[IN] - Type of operation from workers xml.
	*					@pInputs[IN] - Pointer to Locked input map supplies the input values.
	*					@strOutputs[IN] - Output names of operation from workers xml.
	*
	* Return       	  : bool - success, if worker prepared to perform
	******************************************************************************/

	bool Prepare(const char* szFunction,
				const char* szType,
				CIdEntityMap* pInputs,
				string& strOutputs,
				bool bAutoUnlock = true)
	{
		/* Acquire lock for the worker to prevent reentrancy
		*  (All singleton worker objects implement object-level locking)
		*/
		LOCK_MUTEX(&m_tWorkerLock);
		m_tOutputs.AddNewEntity("?Exception", "", false);//Exception OP holder for all operations
		m_tOutputCopy.AddNewEntity("?Exception", "", false);
		m_bAutoUnlock	= bAutoUnlock;
		m_nOutcome		= READY;

		if(!ValidateInputs(pInputs))
		{
			g_tLog.WriteLogWithoutTimeStamp("Input validation failed !!");
			return false;
		}
		int nPrevIndex	= 0;
		int nIndex		= 0;
		if(!strOutputs.empty())
		{
			m_strOutputs	= strOutputs;
			string strTemp	= "";
			while(true)
			{
				nIndex = m_strOutputs.find(",", nPrevIndex);
				if(nIndex == string::npos)
				{
					strTemp = m_strOutputs.substr(nPrevIndex);
					m_tOutputs.AddNewEntity(strTemp, "", true);
					m_tOutputCopy.AddNewEntity(strTemp, "", true);
					break;
				}
				else
				{
					strTemp = m_strOutputs.substr(nPrevIndex, nIndex-nPrevIndex);
					m_tOutputs.AddNewEntity(strTemp, "", false);
					m_tOutputCopy.AddNewEntity(strTemp, "", false);
					nPrevIndex = nIndex+1;
				}
			}
		}
//		int nSize1			= m_tOutputCopy.Size();
//		int nSize2			= m_tOutputs.Size();
		m_strFunction		= szFunction;
		m_strType			= szType;
		m_pInputs			= pInputs;
		m_bOutputAvailable	= false;

		return true;
	}

	/******************************************************************************
	* Function name	  : CIdWorker:Perform
	* Description  	  : Entry-point function for any worker to get its operation
	*					started. It invokes the derived _Exec impl.
	*
	******************************************************************************/

	void Perform(CKtiThread* pThrHost=NULL)
	{
		if(m_nOutcome != READY)
			return;

		m_nOutcome = RUNNING;
		m_pThreadHost = pThrHost;
		_Exec(m_strFunction.c_str(), m_pInputs);
		m_bOutputAvailable	= true;
		/*if(m_strType.find("bool") == string::npos)
		{
			if(m_nOutcome != EXCEPTION)
				m_nOutcome = POSITIVE; //Non-bool workers can only have +ve outcome
		}*/
	}

	/******************************************************************************
	* Function name	  : CIdWorker:Clean
	* Description  	  : Function relevant only for async workers to clean any
	*					dynamic memory. It invokes the derived _Clean impl.
	*
	*
	* Return       	  : bool - success, if worker impl fn succeeds/failure
	******************************************************************************/

	bool Clean()
	{
		if(m_pInputs != NULL)
			delete m_pInputs;

		m_tOutputs.RemoveAll();
		m_tOutputCopy.RemoveAll();
		m_strOutputs = "";
		
		if((m_nOutcome == RUNNING) || (m_nOutcome == WAITING))
		{
			SetOutcome(ABORTED);
			UNLOCK_MUTEX(&m_tWorkerLock);
			return _Clean();
		}
		UNLOCK_MUTEX(&m_tWorkerLock);
		return true;
	}

	/******************************************************************************
	* Function name	  : CIdWorker:GetLifecycle
	* Description  	  : Function that returns the lifecycle model of this worker.
	*
	* Return       	  : int - LC_SINGLETON/LC_SINGLE_JOB/LC_SINGLE_USE
	******************************************************************************/

	int GetLifecycle()
	{
		return m_nLifeCycleType;
	}

	/******************************************************************************
	* Function name	  : CIdWorker:IsLibWorker
	* Description  	  : Function to check if this worker is resident in an external
	*					library.
	*
	* Return       	  : bool - True, if it's library resident
	******************************************************************************/

	bool IsLibWorker()
	{
		return m_bLibWorker;
	}

	/******************************************************************************
	* Function name	  : CIdWorker:GetModName
	* Description  	  : Function that returns module name of this worker.
	* Arguments		  : @strModName[OUT] - Returns the mod name
	*
	* Return       	  : void
	******************************************************************************/

	void GetModName(string& strModName)
	{
		strModName =  m_strModName;
	}

	/******************************************************************************
	* Function name	  : CIdWorker:GetOutcome
	* Description  	  : Function that returns the final outcome of this worker.
	*
	* Return       	  : int - Possible:-1(Exception),0(-ve),1(+ve),2(RUNNING),3(WAITING)
	******************************************************************************/

	int GetOutcome()
	{
		return m_nOutcome;
	}

	/******************************************************************************
	* Function name	  : CIdWorker:GetProgressOutputs
	* Description  	  : Function that returns progress outputs (peek) from the
	*					worker. It invokes the derived _Peek impl.
	*
	* Arguments		  : @tPeekOp[IN/OUT] - Locked input map supplies the peekop names.
	*
	* Return       	  : bool - success, if read/failure, if _peek failed.
	******************************************************************************/

	bool GetProgressOutputs(CIdEntityMap& tPeekOp)
	{
		return _PeekOp(m_strFunction.c_str(), tPeekOp);
	}

	/******************************************************************************
	* Function name	  : CIdWorker:GetMainOutputs
	* Description  	  : Function that returns the last ready-outputs updated by the
	*					worker. It resets the m_bOutputAvailable flag so that next time
	*					SetOutput() is called by the worker, outputs gets copied again.
	*
	* Arguments		  : @tMainOp[IN/OUT] - Locked input map supplies the output names.
	*
	* Return       	  : bool - success, if read/failure, if outputs are not ready.
	******************************************************************************/

	bool GetMainOutputs(CIdEntityMap& tMainOp)
	{
		if(!m_bOutputAvailable)
			return false;

		int nLock;
		if(m_pThreadHost)
		{
			nLock	= m_pThreadHost->AcquireLock();
		}
		m_tOutputCopy.FlushOut(tMainOp);
		m_bOutputAvailable = false;
		if(m_pThreadHost)
			m_pThreadHost->ReleaseLock(nLock);
		return true;
	}
	virtual bool _Startup() = 0;
protected:

	

	/******************************************************************************
	* Function name	  : CIdWorker:_Exec
	* Description  	  : Pure virtual function that derived workers must implement
	*					to execute different functions/operations supported by this
	*					workers as defined in Workers xml. Derived Worker's  _Exec
	*					impl should callback using SetOutput(), SetOutcome() or
	*					SetException() methods to report their output.
	*
	* Arguments		  : @szFunction[IN] - Function name to be invoked
	*					@pInputs[IN] - Input map for the function
	*
	* Return       	  : void.
	******************************************************************************/

	virtual void _Exec(const char* szFunction,
							CIdEntityMap* pInputs) = 0;

	/******************************************************************************
	* Function name	  : CIdWorker:_Clean
	* Description  	  : Pure virtual function that the derived worker must implement,
	*					if they have to perform any cleanup of allocated memory.
	*
	* Arguments		  : none
	*
	* Return       	  : bool - success/failure on executing the overriden clean impl.
	******************************************************************************/

	virtual bool _Clean() = 0;

	/****************************************************
	* Function name	  : CIdWorker:Abort
	* Description  	  : Pure virtual function that the derived
	* worker must implement, if they have to perform any
	* cleanup of allocated memory.
	*
	* Arguments	  : none
	*
	* Return       	  : bool - success/failure on executing
	* the overriden clean impl.
	********************************************************/

	virtual bool Abort() = 0;

	/******************************************************************************
	* Function name	  : CIdWorker:_PeekOp
	* Description  	  : Pure virtual function that the derived worker must implement,
	*					if they produce periodic progress outputs as defined by
	*					"Peekop" attribute of Operation tag defined in Workers xml. .
	*
	* Arguments		  : @szFunction[IN] - Function name to be peeked (outputs could differ)
	*					@tPeekOp[IN/OUT] - Locked input map supplies the output names.
	*					When function executes, the derived impl can only set values
	*					for supplied inputs. The impl must set peek outputs for
	*					different functions as per workers xml definition.
	*
	* Return       	  : bool - success/failure on executing the overriden peekop impl.
	******************************************************************************/

	virtual bool _PeekOp(const char* szFunction, CIdEntityMap& tPeekOp) = 0;

	/******************************************************************************
	* Function name	  : CIdWorker:SetOutput
	* Description  	  : For derived workers to set their outputs. The new output
	*					is appended to the existing output. If an external thread
	*					has reset the m_bOutputAvailable flag, it creates a copy of the
	*					existing outputs and sets the m_bOutputAvailable flag again for
	*					external thread indicating output is ready.
	*
	* Arguments		  : @szOutputName[IN] - Name of valid output
	*					@szOutputValue[IN] - New value for this output
	*					@bSetAvailability[IN] - To be used by derived workers, if
	*					they have multiple outputs to be updated as part of a
	*					single transaction before making outputs available for
	*					external threads. Defaults to true - Set this flag to
	*					true while setting the last output in the transaction.
	*					@nMode[IN] - How to set the new output value - whether to
	*					overwrite existing value or append to it.
	*					@szDelim[IN] - Ignored if 'nMode=OVERWRITE'. Else, if the existing
	*					value is not empty, it inserts the delimiter before appending
	*					the new output.
	*
	* Return       	  : bool - success/failure on setting the output. This calls
	*					fails, if the derived worker implementation attempts to
	*					set an unknown output that's not defined in workers xml for
	*					the current operation.
	******************************************************************************/

	// TODO: Take care of reentrant external threads spawned by this worker
	// Introduce another mutex ?
	bool SetOutput(const char* szOutputName, const char* szOutputValue, bool bSetAvailability,
					int nMode = OVERWRITE, const char* szDelim = NULL)
	{
		string strOutput = szOutputName;
		string strNewValue = "";

		if(nMode == OVERWRITE)
		{
			strNewValue = szOutputValue;
		}
		else
		{
			string strExistvalue = "";
			if(!m_tOutputs.GetEntityValue(strOutput, strExistvalue))
			{
				//g_tLog.WriteLogWithoutTimeStamp("Unsupported output - %s", szOutputName);
				return false;
			}
			if(strExistvalue.empty())
				strNewValue = szOutputValue;
			else
			{
				if(nMode == LT_APPEND)
				{
					strNewValue = szOutputValue;
					if(szDelim != NULL)
						strNewValue += szDelim;
					strNewValue += strExistvalue;

				}
				else
				{
					strNewValue = strExistvalue;
					if(szDelim != NULL)
						strNewValue += szDelim;
					strNewValue += szOutputValue;
				}
			}
		}
		m_tOutputs.SetEntityValue(strOutput, strNewValue);
		int nLock;
		if(m_pThreadHost)
			nLock = m_pThreadHost->AcquireLock();
		if(!m_bOutputAvailable && bSetAvailability) // For external monitoring threads
		{
			//g_tLog.WriteLogWithoutTimeStamp("!m_bOutputAvailable && bSetAvailability", true); //Rama
			m_tOutputs.FlushOut(m_tOutputCopy);
			m_tOutputs.EmptyAll();
			m_bOutputAvailable = true;
		}
		if(m_pThreadHost)
			m_pThreadHost->ReleaseLock(nLock);
		return true;
	}

	/******************************************************************************
	* Function name	  : CIdWorker:SetOutcome
	* Description  	  : For derived workers to set their outcome. Setting the outcome
	*					to "0" is permitted only for those workers that are declared
	*					as type "bool" in Workers xml. For all other workers, there
	*					is only a single default outcome "true".
	*
	* Arguments		  : @nOutcome[IN] - Outcome of worker's operation
	*					0:Negative (only for bool), 1:Negative, -1:exception
	*
	* Return       	  : bool - success/failure on setting the outcome.
	******************************************************************************/

	bool SetOutcome(int nOutcome)
	{
		int nLock;
		if(m_nOutcome != RUNNING)
		{
			//g_tLog.WriteLogWithoutTimeStamp("Expected to call SetOutcome only once..");
			return false;
		}
		if(nOutcome == WAITING)
		{
			if(m_pThreadHost==NULL)
			{
				g_tLog.WriteLogWithoutTimeStamp("This outcome permitted only for Async workers..");
				return false;
			}
			else
			{
				nLock	= m_pThreadHost->AcquireLock();
				m_nOutcome	= WAITING;
				m_pThreadHost->SuspendOSThread(0, nLock);//Worker blocks here
				m_nOutcome	= RUNNING;
				return true;
			}
		}
		if(nOutcome == ABORTED)
		{
			SetException("Worker operation aborted !!");
			if(m_pThreadHost)
			{
				nLock	= m_pThreadHost->AcquireLock();
				if(m_nOutcome == WAITING)
					m_pThreadHost->Resume(nLock);
				else
					m_pThreadHost->ReleaseLock(nLock);
			}
			return true;
		}

		if(m_pThreadHost)
			nLock	= m_pThreadHost->AcquireLock();

		//If any final outputs are pending to be flushed out
		string strName			= "";
		string strValue			= "";
		string strExistValue	= "";
		if(m_tOutputs.GetFirstEntry(strName, strValue))
		{
			do
			{
				if(strValue.empty())
					continue;

				m_tOutputCopy.GetEntityValue(strName, strExistValue);
				m_tOutputCopy.SetEntityValue(strName, strExistValue + strValue);

			}while(m_tOutputs.GetNextEntry(strName, strValue));
		}

		if(m_strType.find("bool") == string::npos)
		{
			if(nOutcome == NEGATIVE)
			{
				//g_tLog.WriteLogWithoutTimeStamp("Unexpected Outcome!! Setting default POSITIVE.");
				m_nOutcome = POSITIVE;
				if(m_pThreadHost)
					m_pThreadHost->ReleaseLock(nLock);
				return true;
			}
		}
		m_nOutcome = nOutcome;
		if(m_pThreadHost)
			m_pThreadHost->ReleaseLock(nLock);
		return true;
	}

	/******************************************************************************
	* Function name	  : CIdWorker:SetException
	* Description  	  : For derived workers to set an exception output.
	*
	* Arguments		  : @szOutputValue[IN] - Exception details
	*
	* Return       	  : void
	******************************************************************************/

	void SetException(const char* szOutputValue)
	{
		SetOutput("?Exception", szOutputValue, true);
		SetOutcome(EXCEPTION);
	}

	/******************************************************************************
	* Function name	  : CIdWorker:ValidateInputs
	* Description  	  : For additional validation of inputs supplied to this worker.
	*					Derived worker impl may override this function.
	*
	* Arguments		  : @pInputs[IN] - Input map to be validated.
	*
	* Return       	  : bool - success/failure based on validation output.
	******************************************************************************/

	virtual bool ValidateInputs(CIdEntityMap* pInputs)
	{
		return true;
	}

private:
	CIdEntityMap m_tOutputs;
	CIdEntityMap m_tOutputCopy;

	string		 m_strModName;
	int			 m_nLifeCycleType;
	bool		 m_bLibWorker;
	string		 m_strOutputs;
	string		 m_strFunction;
	string		 m_strType;
	CIdEntityMap* m_pInputs;
	CKtiThread*  m_pThreadHost;
	int			 m_nOutcome;
	bool		 m_bOutputAvailable; // Used to sync read/write output with an ext thread.

	bool		 m_bAutoUnlock;
	MUTEX_OBJECT m_tWorkerLock; // To prevent reentrant calls for singleton workers
};

#endif //__IDWORKER_H__
