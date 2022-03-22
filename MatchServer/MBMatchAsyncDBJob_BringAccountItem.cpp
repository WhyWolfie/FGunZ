#include "stdafx.h"
#include "MBMatchAsyncDBJob_BringAccountItem.h"

void MBMatchAsyncDBJob_BringAccountItem::Run(void* pContext)
{
	MMatchDBMgr* pDBMgr = (MMatchDBMgr*)pContext;
	//Custom: Load iLevel in bringaccountitem queries.
	if (!pDBMgr->BringAccountItem(m_nAID,
		m_nCID, 
		m_nAIID,
		&m_noutCIID, 
		&m_noutItemID, 
		&m_boutIsRentItem, 
		&m_noutRentMinutePeriodRemainder,
		m_woutRentHourPeriod,
		&m_noutRarity,
		&m_noutiLevel))
	{
		SetResult(MASYNC_RESULT_FAILED);
		return;
	}

	SetResult(MASYNC_RESULT_SUCCEED);
}


bool MBMatchAsyncDBJob_BringAccountItem::Input(unsigned int nAID, unsigned int nCID, unsigned int nAIID)
{
	m_nAID = nAID;
	m_nCID = nCID;
	m_nAIID = nAIID;

	return true;
}