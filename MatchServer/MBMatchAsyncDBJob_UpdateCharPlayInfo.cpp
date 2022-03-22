#include "stdafx.h"
#include "MBMatchAsyncDBJob_UpdateCharPlayInfo.h"

void MBMatchAsyncDBJob_UpdateCharPlayInfo::Run( void* pContext )
{
	MMatchDBMgr* pDBMgr = (MMatchDBMgr*)pContext;

	if( !pDBMgr->UpdateCharPlayInfo(m_dwAID
									, m_dwCID
									, m_nXP
									, m_nLevel
									, m_nPlayingTimeSec
									, m_nTotalPlayTimeSec
									, m_nTotalKillCount
									, m_nTotalDeathCount
									, m_nBP
									, m_bIsLevelUp ) )
	{
		SetResult( MASYNC_RESULT_FAILED );
		return;
	}

	SetResult( MASYNC_RESULT_SUCCEED );
}
//Custom: Run job for MBMatchAsyncDBJob_LogCharModules
void MBMatchAsyncDBJob_LogCharModules::Run( void* pContext )
{
	MMatchDBMgr* pDBMgr = (MMatchDBMgr*)pContext;

	if( !pDBMgr->LogCharModules(m_nAID, m_szModule, m_szMMD5))
	{
		SetResult( MASYNC_RESULT_FAILED );
		return;
	}

	SetResult( MASYNC_RESULT_SUCCEED );
}
