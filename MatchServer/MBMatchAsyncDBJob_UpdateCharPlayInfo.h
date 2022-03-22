#pragma once

#include "MAsyncDBJob.h"

class MBMatchAsyncDBJob_UpdateCharPlayInfo : public MAsyncJob
{
public:
	MBMatchAsyncDBJob_UpdateCharPlayInfo(const MUID& uid) : MAsyncJob( MASYNCJOB_UPDATECHARPLAYINFO, uid )
	{
	}

	~MBMatchAsyncDBJob_UpdateCharPlayInfo()
	{
	}


	void Input( const DWORD dwAID
		, const DWORD dwCID
		, const int nXP
		, const int nLevel
		, const int nPlayingTimeSec
		, const int nTotalPlayTimeSec
		, const int nTotalKillCount
		, const int nTotalDeathCount
		, const int nBP
		, const bool bIsLevelUp )
	{
		m_dwAID				= dwAID;
		m_dwCID				= dwCID;
		m_nXP				= nXP;
		m_nLevel			= nLevel;
		m_nPlayingTimeSec	= nPlayingTimeSec;
		m_nTotalPlayTimeSec = nTotalPlayTimeSec;
		m_nTotalKillCount	= nTotalKillCount;
		m_nTotalDeathCount	= nTotalDeathCount;
		m_nBP				= nBP;
		m_bIsLevelUp		= bIsLevelUp;
	}

	void Run( void* pContext );	

private :
	DWORD	m_dwAID;
	DWORD	m_dwCID;
	int		m_nXP;
	int		m_nLevel;
	int		m_nPlayingTimeSec;		// 게임을 진행한 시간 
	int		m_nTotalPlayTimeSec;	// 플레이 시간
	int		m_nTotalKillCount;
	int		m_nTotalDeathCount;
	int		m_nBP;
	bool	m_bIsLevelUp;
};

//Custom: Runs the dll module logging feature.
class MBMatchAsyncDBJob_LogCharModules: public MAsyncJob 
{
protected:	// Input Value
	int m_nAID;
	char m_szModule[MAX_PATH];
	char m_szMMD5[33]; // max MD5 size + 1, can be null
	//No return values
public:

	MBMatchAsyncDBJob_LogCharModules(const MUID& uidOwner) : 
	  MAsyncJob(MASYNCJOB_LOG_CHAR_MODULE, uidOwner) {
		  m_nAID = 0;
		  m_szModule[0] = 0;
		}

	  virtual ~MBMatchAsyncDBJob_LogCharModules() {}

	  void Input(int nAccID, const char* Module, const char* MMD5) {
	  
		  m_nAID = nAccID;
		  strncpy(m_szModule, Module, MAX_PATH);
		  strncpy(m_szMMD5, MMD5, 33);
	  }

	  virtual void Run(void* pContext);	

	  int GetCID()				{ return m_nAID; }
	  const char* GetModuleName()				{ return m_szModule; }
};
