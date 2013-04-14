#include "PopulationStation.h"

#include "../protocol/buildingdata.h"
//class CPopStationData;

CPopStation::CPopStation (int iUniID , int iLevel, int iMaxPopulation , CPlayer *pPlayer)
{
	m_iUniID = iUniID;
	m_iLevel = iLevel;
	m_Player = pPlayer;

	m_iCurPopulaiton = 0;
	m_iMaxPopulation = iMaxPopulation;

	m_mapSoldierID2Count.clear();
	//m_iMaxPopulation = 
}

CPopStation::~CPopStation()
{

}

void CPopStation::InitData(const CPopStationData & data)
{
	m_iCurPopulaiton		= data.m_iCurPopulation;
	m_mapSoldierID2Count	= data.m_mapID2Count;
}

void CPopStation::GetData(CPopStationData & data)
{
	data.m_iUniID = m_iUniID;
	data.m_iCurPopulation = m_iCurPopulaiton;
	data.m_mapID2Count = m_mapSoldierID2Count;
	return;
}

bool CPopStation::TryAddToPopStation(int iSID,int iPopulation)
{
	if(m_iCurPopulaiton + iPopulation > m_iMaxPopulation)
	{	
		return false;
	}
	else
	{
		m_iCurPopulaiton += iPopulation;
		_InsertToMap(iSID);
		return true;
	}

}

bool CPopStation::TryRemoveFromPopStation(int iSID,int iPopulation)
{
	if(_RemoveFromMap(iSID) == false)
	{
		return false;
	}
	else
	{
		m_iCurPopulaiton -= iPopulation;
		if(m_iCurPopulaiton<0)
		{
			printf("Error CPopStation TryRemoveFromPopStation  m_iCurPopulaiton <0");
			m_iCurPopulaiton = 0;
		}
	}
	return true;
}

void CPopStation::_InsertToMap(int iSID)
{
	std::map<int,int>::iterator it =  m_mapSoldierID2Count.find(iSID);
	if(it!=m_mapSoldierID2Count.end())
	{
		it->second++;
		return;
	}
	m_mapSoldierID2Count[iSID] = 1;

}

bool CPopStation::_RemoveFromMap(int iSID)
{
	std::map<int,int>::iterator it =  m_mapSoldierID2Count.find(iSID);
	if(it!=m_mapSoldierID2Count.end())
	{
		int iCount = it->second;
		if(iCount>0)
		{
			iCount--;
			return true;
		}
	}
	return false;
}

void CPopStation::OnUpgradePopStation(int iLevel)
{
	m_iLevel = iLevel;
	//m_iMaxPopulation = 	
}
