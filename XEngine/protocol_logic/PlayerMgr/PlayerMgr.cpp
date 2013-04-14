#include "PlayerMgr.h"
#include "Player.h"

INSTANCE_SINGLETON(CPlayerMgr)

#include "../Interface/Protocol_logic.h"
//#include "../network.h"



CPlayerMgr::CPlayerMgr()
{  
}

CPlayerMgr::~CPlayerMgr()
{ 
}

bool CPlayerMgr::Init()
{
	return true;
}

void CPlayerMgr::UnInit()
{

}

bool CPlayerMgr::OnLogin(UINT32 dwConnID,int  iAccountID, std::string strName, int iRMB ,int iGold , int iOil,
						 const std::vector<BuildingData> & rVecBuilding,const std::vector<StorageData> & rVecStoreData, __time64_t storetime,const LabData & labdata,
						 const SoldierData & soldierdata,const std::vector<CBuilderData>& vecBuilderData,const std::map<int,CampData>& mapCampData  )
{
	if(	m_MapConnID2Player.find(dwConnID) != m_MapConnID2Player.end() )
	{
		printf("CPlayerMgr::OnLogin Error ConnID already Exist ConnID = %u, AccountID = %i, name = %s, RMB = %i \n  ",dwConnID,iAccountID,strName,iRMB);
		return false;
	}
	
	if( m_MapAccountID2Player.find(iAccountID)	!= m_MapAccountID2Player.end() )
	{
		printf("CPlayerMgr::OnLogin Error AccountID already Exist ConnID = %u, AccountID = %i, name = %s, RMB = %i \n  ",dwConnID,iAccountID,strName,iRMB);
		KickOffLine(iAccountID);
		//return false;
	}

	if(	m_MapName2Player.find(strName) != m_MapName2Player.end() )
	{
		printf("CPlayerMgr::OnLogin Error Name already Exist ConnID = %u, AccountID = %i, name = %s, RMB = %i \n  ",dwConnID,iAccountID,strName,iRMB);
		KickOffLine(strName);
		//return false;
	}


	CPlayer * pPlayer  = new CPlayer(dwConnID,iAccountID,strName,iRMB,iGold,iOil,rVecBuilding,rVecStoreData,storetime,labdata,soldierdata,vecBuilderData,mapCampData);
	if(pPlayer == NULL)
	{
		printf("Create Player Error ",dwConnID,iAccountID,strName,iRMB);
		return false;
	}

	m_MapConnID2Player[dwConnID] = pPlayer;
	m_MapAccountID2Player[iAccountID] = pPlayer;
	m_MapName2Player[strName] = pPlayer;

	pPlayer->SavePlayer();

	return true;

}


CPlayer* CPlayerMgr::GetPlayerByConnID(UINT32 dwConnID)
{
	std::map<UINT32 , CPlayer*>::iterator it = m_MapConnID2Player.find(dwConnID);
	if(it != m_MapConnID2Player.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

CPlayer* CPlayerMgr::GetPlayerByAccountID(int iAccount)
{
	std::map<int , CPlayer*>::iterator it = m_MapAccountID2Player.find(iAccount);
	if(it != m_MapAccountID2Player.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

CPlayer* CPlayerMgr::GetPlayerByName(std::string strName)
{
	std::map<std::string  , CPlayer*>::iterator it = m_MapName2Player.find(strName);
	if(it != m_MapName2Player.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void CPlayerMgr::ReleasePlayer(UINT32 dwConnID)
{
	CPlayer * pPlayer = GetPlayerByConnID(dwConnID);
	if(pPlayer == NULL)
	{
		printf("Fatal Error  CPlayerMgr::ReleasePlayer Cannnot Find Player ConnID = %u ",dwConnID);
		return;
	}

	int iAccountID = pPlayer->GetAccountID();
	std::string strName	   = pPlayer->GetName();

	delete pPlayer;

	m_MapConnID2Player.erase(dwConnID);
	m_MapAccountID2Player.erase(iAccountID);
	m_MapName2Player.erase(strName);

}

void CPlayerMgr::ProcessPlayerMgr()
{
	std::map<int , CPlayer*>::iterator it = m_MapAccountID2Player.begin();
	for( ; it!=m_MapAccountID2Player.end() ;it++ )
	{
		CPlayer *pPlayer = it->second;
		if(pPlayer)
		{
			pPlayer->ProcessPlayer();
		}

	}

}

void CPlayerMgr::KickOffLine(int iAccountID)
{
	CPlayer *pPlayer = GetPlayerByAccountID(iAccountID);
	if(pPlayer)
	{
		UINT32 uConnID = pPlayer->GetConnID();
		CProtocolLogic * pProtocol = CProtocolLogic::GetProtocolLogic();
		pProtocol->SetDisconnectClientID(uConnID);
		//CNetProcessor::Instance()->Close(uConnID);
	}
}

void CPlayerMgr::KickOffLine( std::string strName )
{
	CPlayer *pPlayer = GetPlayerByName(strName);
	if(pPlayer)
	{
		UINT32 uConnID = pPlayer->GetConnID();
		CProtocolLogic * pProtocol = CProtocolLogic::GetProtocolLogic();
		pProtocol->SetDisconnectClientID(uConnID);
		//CNetProcessor::Instance()->Close(uConnID);
	}
}
