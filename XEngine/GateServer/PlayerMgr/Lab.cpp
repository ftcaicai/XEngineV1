#include "Lab.h"

#include "../CSVData/CsvReaderMgr.h"
#include "Player.h"
#include <atltime.h>

#include "../protocol/buildingdata.h"
#include "../protocol/PtcSoldier/PtcReqUpgradeSoldier.h"

CLab::CLab ( CPlayer * pPlayer,int iLevel )
{
	m_Player = pPlayer;
	m_iLevel = iLevel;
	m_iSoldierIDInUpgrade = -1;
	m_TimeBeginUpgrade = 0;
	m_TimeEndUpgrade   = 0;
	m_mapSoldierID2Level.clear();
}

CLab::~CLab()
{ 
}

void CLab::InitData(const LabData & labdata)
{
	m_iSoldierIDInUpgrade	= labdata.m_iSoldierIDInUpgrade;
	m_TimeBeginUpgrade		= labdata.m_TimeBeginUpgrade;
	m_TimeEndUpgrade		= labdata.m_TimeEndUpgrade;
	m_mapSoldierID2Level	= labdata.m_mapSoldierID2Level;
}

void CLab::GetLabData(LabData & data )
{
	data.m_iSoldierIDInUpgrade	= m_iSoldierIDInUpgrade;
	data.m_TimeBeginUpgrade		= m_TimeBeginUpgrade;
	data.m_TimeEndUpgrade		= m_TimeEndUpgrade;
	data.m_mapSoldierID2Level	= m_mapSoldierID2Level;
	return;
}

void CLab::ProcessLab()
{
	if(IsInUpgrade()== false)
	{
		return;
	}


	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime();
	
	if(now > m_TimeEndUpgrade)
	{
		std::map<int,int>::iterator it =  m_mapSoldierID2Level.find(m_iSoldierIDInUpgrade);
		if(it != m_mapSoldierID2Level.end())
		{
			it->second += 1;
		}	
		else
		{
			printf("error on this soldier id=%d",m_iSoldierIDInUpgrade);
		}
	

		AckUpgradeSoldierSuccessEnd(m_iSoldierIDInUpgrade);
		m_iSoldierIDInUpgrade = -1;
		if(m_Player)
		{
			m_Player->SavePlayer();
		}	
	}

}

bool CLab::ReqUpgradeSoldier(int iSoldierID)
{
	if(IsInUpgrade()== true)
	{
		printf("error upgrade soldier  another task is running and id=%u",GetInUpgradeSoldierID());
		return false;
	}

	if(IsUnLock(iSoldierID) == false)
	{
		//尚未解锁 
		return false;
	}

	int iLevel = GetSoldierLevel(iSoldierID);
	if(iLevel == -1)
	{
		//不存在与map之中 证明是首次升级 初始化为0级
		m_mapSoldierID2Level[iSoldierID] = 1;
		iLevel = 0;
		//printf("error upgrade soldier  no this soldierID and id=%u",iSoldierID);
		//return false;
	}

	int iLevelMax = 10;//Get from soldier csv  now we use 10 first
	if(iLevel>=iLevelMax)
	{
		printf("error upgrade soldier this soldier already maxlevel and id=%u",iSoldierID);
		return false;
	}

	if(iLevel>=m_iLevel)
	{
		printf("error upgrade soldier soldier level can not be higher than lab and id=%u, lab level= %d",iSoldierID,m_iLevel);
		return false;
	}


	int iOilCost  = CCsvReaderMgr::Instance()->GetSoldierUpgradeOilCost(iSoldierID,iLevel+1); 
	int iTimeCost = CCsvReaderMgr::Instance()->GetSoldierUpgradeTimeCost(iSoldierID,iLevel+1); 

	if( m_Player &&
		m_Player->DecOil(iOilCost) == true)
	{
		_UpGradeSoldier(iSoldierID,iTimeCost,iOilCost);	
		return true;
	}
	else
	{
		printf("error upgrade soldier player have not enough oil soldierid=%u, need oil = %d ",iSoldierID,iOilCost);
		return false;
	}

}

void CLab::OnUpgradeLab(int iLevel)
{
	m_iLevel = iLevel;
}

int	 CLab::GetSoldierLevel(int iSoldierID)
{
	std::map<int,int>::iterator it =  m_mapSoldierID2Level.find(iSoldierID);
	if(it!=m_mapSoldierID2Level.end())
	{
		return it->second;
	}

	return -1;
}

bool CLab::IsInUpgrade()
{
	if(m_iSoldierIDInUpgrade == -1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int  CLab::GetInUpgradeSoldierID()
{
	return m_iSoldierIDInUpgrade;
}

void CLab::_UpGradeSoldier(int iSoldierID,int iTimeCost,int iOilCost)
{
	m_iSoldierIDInUpgrade = iSoldierID;

	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime();
	m_TimeBeginUpgrade = now;
	m_TimeEndUpgrade = now + iTimeCost;

	if(m_Player)
	{
		m_Player->SavePlayer();
	}

	AckUpgradeSoldierSuccessBegin(iOilCost);

}

void CLab::AckUpgradeSoldierSuccessBegin(int iOilCost)
{
	if( m_Player )
	{
		CAckUpgradeSoldierProtocol ptc; 
		ptc.m_iSoldierID		= m_iSoldierIDInUpgrade;
		ptc.m_TimeBeginUpgrade	= m_TimeBeginUpgrade;
		ptc.m_TimeEndUpgrade	= m_TimeEndUpgrade;
		ptc.m_iOilCost			= iOilCost;
		m_Player->Send(ptc);
	}
}

void CLab::AckUpgradeSoldierSuccessEnd(int iSoldierID)
{
	if( m_Player )
	{
		CAckUpgradeSoldierSuccProtocol ptc; 
		ptc.m_iSoldierID = iSoldierID;
		m_Player->Send(ptc);
	}
}

bool CLab::IsUnLock(int iSoldierID)
{
	//检查是否解锁 将来应该去camp中遍历 这里默认所有兵都已经解锁
	return true;
}
