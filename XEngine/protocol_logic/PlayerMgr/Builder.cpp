#include "Camp.h"
#include "../CSVData/CsvReaderMgr.h"

#include "Builder.h"
#include <atltime.h>

#include "Player.h"

#include "../protocol/PtcAckBuildLevelUp.h"

CBuilder::CBuilder (int iUniID ,CPlayer *pPlayer)
{
	m_iUniID = iUniID;
	m_Player = pPlayer;

	m_iUniIDToUp = -1;
}

CBuilder::~CBuilder()
{ 
}

void CBuilder::InitBuilderData(int iUniID,int iUniIDInLevelUp,int iIDInLevelUp,int iLevel,__time64_t timeEnd)
{
	m_iUniIDToUp	= iUniIDInLevelUp;
	m_iIDToUp		= iIDInLevelUp;	
	m_iLevelToUp	= iLevel;
	m_TimeEnd		= timeEnd;
}


void CBuilder::ProcessBuilder()
{
	if(IsInUpgrade()== false)
	{
		return;
	}

	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime();

	if(now > m_TimeEnd)
	{
		OnSuccessLevelUp();
	}

}

bool CBuilder::ReqLevelUp(int iUniID, int iID,int iLevel)
{
	if(IsInUpgrade()== true)
	{
		return false;
	}

	m_iUniIDToUp = iUniID;
	m_iIDToUp = iID;
	m_iLevelToUp = iLevel;

	int iTimeForLevelUp = CCsvReaderMgr::Instance()->GetBuildTime(iID,iLevel); 

	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime();

	m_TimeBegin = now;
	m_TimeEnd	= now + iTimeForLevelUp;

	_AckBuildLevelUpBegin();

	return true;
}

bool CBuilder::ReqCancelLevelUp(int iUniID)
{
	return false;
}

int	CBuilder::GetUniID()
{
	return m_iUniID;
}

int CBuilder::GetUniIDInLevelUp()
{
	return m_iUniIDToUp;
}

int CBuilder::GetLevelInLevelUp()
{
	return m_iLevelToUp;
}

__time64_t	CBuilder::GetTimeEndCreating()
{
	return m_TimeEnd;
}
	
bool CBuilder::IsInUpgrade()
{
	if(m_iUniIDToUp == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void CBuilder::OnSuccessLevelUp()
{
	if(m_Player)
	{
		if(m_iLevelToUp == 1)
			m_Player->OnBuildAddSuccess(m_iUniIDToUp,m_iIDToUp);
		else
			m_Player->OnUpgradeSuccess(m_iUniIDToUp);
	}
	_AckBuildLevelUpEnd();
	m_iUniIDToUp = -1;
}


void CBuilder::_AckBuildLevelUpBegin()
{
	CAckBuildLevelUpBeginProtocol ptc;
	ptc.m_iUniID			= m_iUniID;
	ptc.m_iUniIDInLevelUp	= m_iUniIDToUp;
	ptc.m_timeEnd			= m_TimeEnd;
	if(m_Player)
	{
		m_Player->Send(ptc);
	}
}

void CBuilder::_AckBuildLevelUpEnd()
{
	CAckBuildLevelUpEndProtocol ptc;
	ptc.m_iUniID = m_iUniID;
	ptc.m_iUniIDInLevelUp = m_iUniIDToUp;
	ptc.m_iLevel = m_iLevelToUp;
	if(m_Player)
	{
		m_Player->Send(ptc);
	}
}