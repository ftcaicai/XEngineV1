#include "Player.h"

#include <atltime.h>

#include "../MySQL/CMysql.h"

#include "../CSVData/CsvReaderMgr.h"

#include "../protocol/PtcBuildResourceCollect.h"
#include "../protocol/PtcHarry/PtcHarryResoure.h"
#include "../protocol/PtcSoldier/PtcSyncPopulation.h"
#include "../protocol/PtcAckBuildLevelUp.h"
#include "../Interface/Protocol_logic.h"
//#include "../clientlink.h"

#include "Camp.h"
#include "Lab.h"
#include "Builder.h"
#include "PopulationStation.h"
#include "PlayerWarReport.h"

CPlayer::CPlayer ( UINT32 dwConnID,int iAccount,std::string strName , int iRMB ,int iGold ,int iOil , 
				  const std::vector<BuildingData> & rVecBuilding,const std::vector<StorageData> & rVecStoreData, __time64_t storetime,const LabData & labdata,
				  const SoldierData& soldierData,const std::vector<CBuilderData>& vecBuilderData,const std::map<int,CampData>& mapCampData,
				  const std::map<int,CPopStationData>& mapPopStationData,const std::vector<CPlayerWarReportData> &vPlayerWarReport,__time64_t ProtectTime)
{
	m_dwConnID		= dwConnID;
	m_iAccount		= iAccount;
	m_strName		= strName;
	m_iRMB			= iRMB;
	m_iGold			= iGold;
	m_iOil			= iOil;
	m_oBuildings	= rVecBuilding;
	m_oStorageData	= rVecStoreData;
	m_StoreTime		= storetime;
	m_lab			= NULL;
	m_ProtectTime   = ProtectTime;

	m_iSoldierCurrentPopulation = 0;
	m_iSoldierCurrentPopulation	= soldierData.m_iSoldierCurrentPopulation;
	m_mapSoldierList			= soldierData.m_mapSoldierList;

	InitBuildFactor();
	InitLabData(labdata);
	InitCampData(mapCampData);
	InitBuilderData(vecBuilderData);
	InitPopuStationData(mapPopStationData);
	InitWarReport(vPlayerWarReport);
	

	CaculateResourceMineStore();
	CaculateCampToSoldierList();
}

CPlayer::~CPlayer()
{ 
}

void CPlayer::ProcessPlayer()
{
	_AutoSavePlayer();

	std::map<int,CCamp*>::iterator it = m_mapUniID2Camp.begin();
	for(;it!=m_mapUniID2Camp.end();it++)
	{
		CCamp* pCamp = it->second;
		pCamp->ProcessCamp();
	}

	if(m_lab)
	{
		m_lab->ProcessLab();
	}

	std::map<int,CBuilder*>::iterator itr = m_mapBuilder.begin();
	for(;itr!=m_mapBuilder.end();itr++)
	{
		CBuilder* pBuilder = itr->second;
		pBuilder->ProcessBuilder();
	}

}

void CPlayer::_AutoSavePlayer()
{
	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime(); 

	if(now - m_lastSaveTime > 30)//每30秒自动存储一次玩家数据
	{
		SavePlayer();
	}
}

void CPlayer::SavePlayer()
{
	CMysql::Instance()->SavePlayer(m_iAccount);

	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime(); 
	
	m_lastSaveTime = now;

}

bool CPlayer::Send(const CProtocol& roPtc)
{
	CProtocolLogic * pProtocol = CProtocolLogic::GetProtocolLogic();
	pProtocol->SetSendMsg(m_dwConnID,roPtc);
	return true;
	//return CClientLink::Instance()->Send(m_dwConnID,roPtc);
}

UINT32	CPlayer::GetConnID()
{
	return m_dwConnID;
}

int	CPlayer::GetAccountID()
{
	return m_iAccount;
}
std::string CPlayer::GetName()
{
	return m_strName;
}

int	CPlayer::GetRMB()
{
	return m_iRMB;
}

int CPlayer::GetGold()
{
	return m_iGold;
}

int CPlayer::GetOil()
{
	return m_iOil;
}

__time64_t CPlayer:: GetStoreTime()
{
	return m_StoreTime;
}

int CPlayer::GetPopulation()
{
	return m_iPopulation;
}

int CPlayer::GetSoldierLevel(int iSoldierID)
{
	int iLevel = 1;
	if(m_lab)
	{
		iLevel = m_lab->GetSoldierLevel(iSoldierID);
	}
	if(iLevel == -1)
		iLevel = 1;

	return iLevel;
}

int CPlayer::GetCurSoldierPopulation()
{
	return m_iSoldierCurrentPopulation;
}
__time64_t CPlayer::GetProtectTime()
{
	return m_ProtectTime;
}

const std::vector<BuildingData> & CPlayer:: GetBuildingDataVec()
{
	return m_oBuildings;
}

const std::vector<StorageData> & CPlayer::GetStorageDataVec()
{
	return m_oStorageData;
}
void CPlayer::SetStorageDataVec(const std::vector<StorageData> &vecStorageData,int nLostPrecent,int nAccountID)
{
	CMysql::Instance()->UpdateTargetData(nAccountID,vecStorageData);
	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime();
	if(nLostPrecent >= 40 && nLostPrecent < 90)
	{
		now = now + 8 * 60 * 60;
		CMysql::Instance()->SetPlayerProtectTime(nAccountID,now);
	}
	else if(nLostPrecent >= 90)
	{
		now = now + 16 * 60 * 60;
		CMysql::Instance()->SetPlayerProtectTime(nAccountID,now);
	}
	else
	{
		CMysql::Instance()->SetPlayerProtectTime(nAccountID,0);
	}
	
}
void  CPlayer::GetLabData(LabData& data)
{
	if(m_lab)
	{
		m_lab->GetLabData(data);
		data.m_bUse = 1;
	}
	else
	{
		data.m_bUse = 0;
	}
	
	
}

void CPlayer::GetSoldierData(SoldierData & data)
{
	data.m_iSoldierCurrentPopulation = m_iSoldierCurrentPopulation;
	data.m_mapSoldierList			 =	m_mapSoldierList;
}

void CPlayer::GetCampDataToClient(std::map<int,CCampDataToClient> & mapUniID2CampDataToClient)
{
	mapUniID2CampDataToClient.clear();
	std::map<int,CCamp*>::iterator it =	m_mapUniID2Camp.begin();
	for( ; it!=m_mapUniID2Camp.end() ; it++)
	{
		CCamp *pCamp = it->second;
		if(pCamp)
		{
			CCampDataToClient data;
			int iUniID = pCamp->GetUniID();
			data.m_iLevel = pCamp->GetLevel();
			data.m_iSoldierIDCreated		= pCamp->GetSoldierCreated();
			data.m_TimeEndCreating			= pCamp->GetTimeEndCreating();
			data.m_iPopulationInWaiting		= pCamp->GetPopulationInWaiting();
			data.m_iMaxPopulationWaiting	= pCamp->GetPopulaitonMax();
			data.m_vecSoldierWaitID			= pCamp->GetVecSoldierWaitingID();
			data.m_mapSoldierWaitNum		= pCamp->GetMapSoldierWaitingNum();

			mapUniID2CampDataToClient[iUniID] = data;
		}
	}

}

void CPlayer::GetCampData(std::map<int,CampData> & mapCampData)
{
	mapCampData.clear();
	std::map<int,CCamp*>::iterator it =	m_mapUniID2Camp.begin();
	for( ; it!=m_mapUniID2Camp.end() ; it++)
	{
		CCamp *pCamp = it->second;
		if(pCamp)
		{
			CampData data;
			int iUniID = pCamp->GetUniID();
			data.m_bInCreating				= pCamp->GetIsCreating();
			data.m_iSoldierIDCreated		= pCamp->GetSoldierCreated();
			data.m_TimeEndCreating			= pCamp->GetTimeEndCreating();
			data.m_iPopulationInWaiting		= pCamp->GetPopulationInWaiting();
			data.m_vecSoldierWaitID			= pCamp->GetVecSoldierWaitingID();
			data.m_mapSoldierWaitNum		= pCamp->GetMapSoldierWaitingNum();

			mapCampData[iUniID] = data;
		}
	}
}

void CPlayer::GetBuilderDataToClient(std::vector<CBuilderDataToClient> & vecBuilderDataToClient)
{
	vecBuilderDataToClient.clear();

	std::map<int,CBuilder*>::iterator it = m_mapBuilder.begin();
	for(;it!=m_mapBuilder.end();it++)
	{
		CBuilder *pBuilder = it->second;
		if(pBuilder)
		{
			CBuilderDataToClient data;
			data.m_iUniID = pBuilder->GetUniID();
			data.m_iUniIDInLevelUp = pBuilder->GetUniIDInLevelUp();
			data.m_TimeEnd = pBuilder->GetTimeEndCreating();

			vecBuilderDataToClient.push_back(data);
		}
	}

}

void CPlayer:: GetBuilderData( std::vector<CBuilderData> & vecBuilderData)
{
	vecBuilderData.clear();
	std::map<int,CBuilder*>::iterator it = m_mapBuilder.begin();
	for(;it!=m_mapBuilder.end();it++)
	{
		CBuilder *pBuilder = it->second;
		if(pBuilder)
		{
			CBuilderData data;
			data.m_iUniID			= pBuilder->GetUniID();
			data.m_iUniIDInLevelUp	= pBuilder->GetUniIDInLevelUp();
			data.m_iLevelInLevelUp	= pBuilder->GetLevelInLevelUp();
			data.m_TimeEnd			= pBuilder->GetTimeEndCreating();

			vecBuilderData.push_back(data);
		}
	}
	return;
}

void CPlayer::GetPopuStationData(std::map<int , CPopStationData>  & mapPopStationData)
{
	mapPopStationData.clear();
	std::map<int,CPopStation*>::iterator it = m_mapUniID2PopuStation.begin();
	for(; it != m_mapUniID2PopuStation.end() ;it++)
	{
		CPopStation *pPopStation = it->second;
		if(pPopStation)
		{
			CPopStationData data;
			pPopStation->GetData(data);
			mapPopStationData[data.m_iUniID] = data;
		}
	}
	return;
}
void CPlayer::GetWarReportData(std::vector<CPlayerWarReportData> & vPlayerWarReport)
{
	vPlayerWarReport.clear();
	vPlayerWarReport = m_pPlayerWarReport->GetWarReport();
}
void CPlayer::UpdateWarReportData(std::vector<CPlayerWarReportData> & vPlayerWarReport)
{
	CMysql::Instance()->UpdateWarReport(vPlayerWarReport);
}
void CPlayer::SetWarReportTop(int iIndex,int iIsTop)
{
	CMysql::Instance()->SetWarReportTop(iIndex,iIsTop);
}
int CPlayer::GetBuildID(int iUniID)
{
	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		if((*it).m_nUniID == iUniID)
		{
			return (*it).m_nID;
		}
	}	
	return -1;
}

void CPlayer::AddRMB(int iValue)
{
	m_iRMB += iValue;
}

void CPlayer::AddGold(int iValue)
{
	m_iGold += iValue;
}

void CPlayer::AddOil(int iValue)
{
	m_iOil += iValue;
}

bool CPlayer::DecRMB(int iValue)
{
	if( m_iRMB < iValue )
		return false;

	m_iRMB -= iValue;
	return true;
}

bool CPlayer::DecGold(int iValue)
{
	if( m_iGold < iValue )
		return false;

	m_iGold -= iValue;

	_DecGoldFromStore(iValue);
	return true;
}

bool CPlayer::DecOil(int iValue)
{
	if( m_iOil < iValue )
		return false;

	m_iOil -= iValue;

	_DecOilFromStore(iValue);
	return true;
}

void CPlayer::AddResource(int iGold,int iOil)
{
	AddGold(iGold);
	AddOil(iOil);
}

bool CPlayer::DecResource(int iGold,int iOil)
{
	if( DecGold(iGold) == false )
	{
		return false;
	}

	if( DecOil(iOil) == false )
	{
		return false;
	}

	return true;

}

bool CPlayer::AddBuild(int iUniID,int iID,int iLevel,int iGridX,int iGridY)
{
	//检查town的限制
	if( CheckTowmLimit(iID) == false)
	{
		return false;
	}	

	int iGoldNeed = CCsvReaderMgr::Instance()->GetGoldCost(iID,iLevel);;
	int iOilNeed  = CCsvReaderMgr::Instance()->GetOilCost(iID,iLevel);

	bool bRet = DecResource(iGoldNeed,iOilNeed);
	if(bRet == false)
	{
		// may be Plug-in so I will kick the player off
		return false;
	}

	if (_InsertToBuilder(iUniID,iID,iLevel+1) == false )
	{
		ReturnResource(iGoldNeed,iOilNeed);
		return false;
	}

	BuildingData data;
	data.m_nUniID = iUniID;
	data.m_nID    = iID;
	data.m_nLevel = iLevel;
	data.m_nGridX = iGridX;
	data.m_nGridY = iGridY;
	data.m_bIsInLevelUp = true;
	m_oBuildings.push_back(data);

	CMysql::Instance()->SavePlayer(m_iAccount);


	return false;
}

void CPlayer::EraseBuild(int iUniID)
{
	if(IsBuildExist(iUniID) == false)
	{
		//none this build so not erases
		return;
	}

	// try to get the gold and oil resent to player by ID and level 

	_EraseBuild(iUniID);
	CMysql::Instance()->SavePlayer(m_iAccount);
	
}
	
void CPlayer::MoveBuild(int iUniID,int iGridX,int iGridY)
{
	// check grid block to  make sure if can move if not client may be trick
	


	_MoveBuild(iUniID,iGridX,iGridY);
	CMysql::Instance()->SavePlayer(m_iAccount);

}
bool CPlayer::FinishUpgradeBuildRMB(int iUniID,int iRMB)
{
	// now we use rmb value from client 
	// later we will caculate in server and check if client is tricking

	//int iRmbServerCaculate = RmbServerCaculate(iUniID)
	//if(iRmbServerCaculate - iRMB >= 100)
	//{
	//	printf("client is tricking \n");
	//	return false;
	//}

	if( DecRMB(m_iRMB) == false  )
	{
		printf("client is tricking \n");
		return false;
	}

	std::map<int,CBuilder*>::iterator itr = m_mapBuilder.begin();
	for(;itr!=m_mapBuilder.end();itr++)
	{
		CBuilder* pBuilder = itr->second;
		if(pBuilder)
		{
			int iUniIDinLevelUp = pBuilder->GetUniIDInLevelUp();
			if(iUniIDinLevelUp == iUniID )
			{
				pBuilder->ReqFinishUpgradeBuildRMB();	
				return true;
			}
		}
	}

	return false;

}

bool CPlayer::UpgradeBuild(int iUniID)
{
	// try to get the gold and oil cost by ID and level 

	if ( _UpgradeBuild(iUniID) == false )
		return false;

	CMysql::Instance()->SavePlayer(m_iAccount);

	return true;
}


void CPlayer::CollectBuildResource(int iUniID,int iGold,int iOil)
{
	//get gold and oil can be collect from mine with uniID 
	//and then check gold and oil is right or wrong and then
	//if right add to player if wrong  the client is tricking
	
	//if right 

	// caculate howmuch gold and oil can be get from the mine uniID
	// 每次收集金油 都遍历所有建筑 并重新计算mine中的矿油储量 似乎是比较耗的
	// 但我打算目前就先这么实现 以后是可以考虑优化 
	// 优化的方式就是每个mine配置一个storetime 但是这样会使得逻辑复杂化 所以暂时不考虑这么做 
	// 以后确有必要 也许可以修改
	CaculateResourceMineStore();	
	
	//把UniID中的金油转存到大厅 目前先不考虑storage 仅仅考虑存入大厅 而且也暂时不考虑大厅的存储上限制
	iGold = 0;
	iOil  = 0;
	//经过处理 iGold和iOil存储成功存入storage的数值
	if ( RestoreResourceFromMineToStorage(iUniID,iGold,iOil) == false)
		return;

	//由此函数可见 storage中的数据 只是表明存储量 如修改到大厅（或者未来包括仓库）的存储量 需要显式地调用addresource接口
	//以修改m_iGold 和 m_iOil的数据
	AddResource(iGold,iOil);

	CMysql::Instance()->SavePlayer(m_iAccount);

	CAckBuildResourceCollectProtocol ptc; //= new CAckSearchOpponentProtocol();
	ptc.m_iUniID	= iUniID;
	ptc.m_iGold		= iGold;
	ptc.m_iOil		= iOil;

	CProtocolLogic * pProtocol = CProtocolLogic::GetProtocolLogic();
	pProtocol->SetSendMsg(m_dwConnID,ptc);
	//CClientLink::Instance()->Send(m_dwConnID, ptc);

}

bool CPlayer::DecSearchGold()
{
	int iSearchGold = CCsvReaderMgr::Instance()->GetSearchGold(m_iTownLevel);
	if(iSearchGold==-1)
		return false;
	
	return  DecGold(iSearchGold);
}


void CPlayer::HarryResource(int iGold,int iOil)
{
	// later we not only add resource but also dec resource 
	_TryRestoreToStorage(iGold,iOil);	

	AddResource(iGold,iOil);

	CMysql::Instance()->SavePlayer(m_iAccount);

	CAckHarryResourceProtocol ptc; 
	ptc.m_iGold		= iGold;
	ptc.m_iOil		= iOil;
	CProtocolLogic * pProtocol = CProtocolLogic::GetProtocolLogic();
	pProtocol->SetSendMsg(m_dwConnID,ptc);
	//CClientLink::Instance()->Send(m_dwConnID, ptc);

}

void CPlayer::ReqOutputSoldier(int iUniID,int iSoldierID)
{
	CCamp * pCamp = GetCamp(iUniID);
	if(pCamp)
	{
		pCamp->ReqOutputSoldier(iSoldierID);
	}
}

void CPlayer::ReqCreateSoldier(int iUniID,int iSoldierID)
{
	CCamp * pCamp = GetCamp(iUniID);
	if(pCamp)
	{
		if( pCamp->ReqCreateSoldier(iSoldierID)==true)
		{
			SavePlayer();
		}
	}
}
void CPlayer::ReqCancelSoldier(int iUniID,int iSID)
{
	CCamp * pCamp = GetCamp(iUniID);
	if(pCamp)
	{
		if( pCamp->ReqCancelCreateSoldier(iSID)==true)
		{
			SavePlayer();
		}
	}
}

bool CPlayer::AddToSoldierList(int iSoldierID)
{
	int iSoldierPopulation = CCsvReaderMgr::Instance()->GetSoldierPopulaiton(iSoldierID);
	if(iSoldierPopulation == -1)
	{
		printf("Bug CPlayer::AddToSoldierList can not find soldier population SoldierID=%u\n",iSoldierID);
		return false;
	}

	if (iSoldierPopulation + m_iSoldierCurrentPopulation > m_iPopulation)
	{
		return false;
	}
	else
	{
		m_iSoldierCurrentPopulation	+= iSoldierPopulation;
		_AddtoSoldierMap(iSoldierID);
		_AddToPopStation(iSoldierID,iSoldierPopulation);

		return true;
	}
	return true;
}

void CPlayer::_AddToPopStation(int iSoldierID,int iSoldierPopulation)
{
	std::map<int,CPopStation*>::iterator it = m_mapUniID2PopuStation.begin();
	for( ; it!=m_mapUniID2PopuStation.end() ;it++ )
	{
		CPopStation * pStatiaon = it->second;
		if(pStatiaon!=NULL)
		{
			if(pStatiaon->TryAddToPopStation(iSoldierID,iSoldierPopulation) == true)
			{
				return;
			}
		}
	}

	printf("Bug CPlayer::_AddToPopStation failed SoldierID=%u\n",iSoldierID);
	return;
}

void CPlayer::_RemoveFromPopStation(int iSoldierID,int iSoldierPopulation)
{
	std::map<int,CPopStation*>::iterator it = m_mapUniID2PopuStation.begin();
	for( ; it!=m_mapUniID2PopuStation.end() ;it++ )
	{
		CPopStation * pStatiaon = it->second;
		if(pStatiaon!=NULL)
		{
			if(pStatiaon->TryRemoveFromPopStation(iSoldierID,iSoldierPopulation) == true)
			{
				return;
			}
		}
	}

	printf("Bug CPlayer::_RemoveFromPopStation failed SoldierID=%u\n",iSoldierID);
	return;

}

void CPlayer::OnPutSoldier(int iSID)
{
	RemoveFromSoldierList(iSID);
	SavePlayer();
}
void CPlayer::OnDisconnected(std::string strName)
{
	CMysql::Instance()->SetPlayerOnlineState(strName,false);
}
void CPlayer::RemoveFromSoldierList(int iSoldierID)
{
	int iSoldierPopulation = CCsvReaderMgr::Instance()->GetSoldierPopulaiton(iSoldierID);
	if(iSoldierPopulation == -1)
	{
		printf("Bug CPlayer::RemoveFromSoldierList can not find soldier population SoldierID=%u\n",iSoldierID);
		return;
	}
		
	m_iSoldierCurrentPopulation	-= iSoldierPopulation;
	if(m_iSoldierCurrentPopulation < 0)
	{
		m_iSoldierCurrentPopulation = 0;
	}

	std::map<int,int>::iterator it = m_mapSoldierList.find(iSoldierID);
	if(it!=m_mapSoldierList.end())
	{
		it->second--;
	}
	else
	{
		printf("Bug CPlayer::RemoveFromSoldierList can not find soldier in map SoldierID=%u\n",iSoldierID);
		return;		
	}

	_RemoveFromPopStation(iSoldierID,iSoldierPopulation);
	

}
	
void CPlayer::ReqUpgradeSoldier(int iSoldierID)
{
	if(m_lab)
	{
		m_lab->ReqUpgradeSoldier(iSoldierID);
	}
	else
	{
		printf("error no lab but receice msg to upgrade SoldieriD = %i\n",iSoldierID);
	}
}


void CPlayer::OnBuildAddSuccess(int iUniID,int iID)
{
	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		if((*it).m_nUniID == iUniID)
		{
			(*it).m_nLevel = 1;
			(*it).m_bIsInLevelUp = false;
			//_AckClientBuildUpgradeSuccess(iUniID,iID,1);
		}
	}

	if(iID == ECampID)
	{
		CCamp *pCamp = new CCamp(iUniID,1,this);
		m_mapUniID2Camp[iUniID] = pCamp;
	}

	if(iID == EPopStationID)
	{
		int iPopulationUpgrade = GetPopulationUpgrade(1);
		m_iPopulation += iPopulationUpgrade;
		_SyncPopulation();
		CPopStation *pPopStation = new CPopStation(iUniID,1,iPopulationUpgrade,this);
		m_mapUniID2PopuStation[iUniID] = pPopStation;

	}

	if(iID == ELabID)
	{
		if(m_lab == NULL)
		{
			m_lab = new CLab(this,1);
		}
	}

}

void CPlayer::_OnBuildUpGradeSuccess(int iUniID,int iID,int iLevel)
{
	if(iID == ETownID)
	{
		IncreaseTownLevel();
	}

	if(iID == ECampID)
	{
		//handle camp
		CCamp *pCamp = GetCamp(iUniID);
		if(pCamp)
		{
			pCamp->LevelUp(iLevel);
		}

	}

	if(iID == EPopStationID)
	{
		// handle population
		int iPopulationUpgrade = GetPopulationUpgrade(iLevel);
		m_iPopulation += iPopulationUpgrade;
		_SyncPopulation();
	}

	if(iID == ELabID)
	{
		//handle lab
		m_lab->OnUpgradeLab(iLevel);
	}
}

bool  CPlayer::IsBuildExist(int iUniID)
{
	int iCount = GetBuildCount();
	for(int i=0;i<iCount;i++)
	{
		if(m_oBuildings[i].m_nUniID == iUniID)
		{
			return true;
		}
	}

	return false;
}

int   CPlayer::GetBuildCount()
{
	return m_oBuildings.size();
}

int CPlayer::GetBuildCount(int iID)
{
	int iCount = 0;
	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		if((*it).m_nID == iID)
		{
			iCount++;
		}
	}

	return iCount;
}

void  CPlayer::_EraseBuild(int iUniID)
{
	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		if((*it).m_nUniID == iUniID)
		{
			m_oBuildings.erase(it);
			return;
		}
	}

}

void  CPlayer::_MoveBuild(int iUniID,int iGridX,int iGridY)
{
	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		if((*it).m_nUniID == iUniID)
		{
			(*it).m_nGridX = iGridX;
			(*it).m_nGridY = iGridY;
			return ;
		}
	}
	
	printf("CPlayer::_MoveBuild can not find build uniID = %d",iUniID);
}
	
bool  CPlayer::_UpgradeBuild(int iUniID)
{
	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		if((*it).m_nUniID == iUniID)
		{
			int iID		= (*it).m_nID;
			int iLevel	= (*it).m_nLevel;
	
			bool bIsInLevelUp = (*it).m_bIsInLevelUp;
			if(bIsInLevelUp)
			{
				printf("CPlayer::_UpgradeBuild Error Is Already In Level Up UniID = %d",iUniID);
				return false;
			}
			if(iLevel >= CCsvReaderMgr::Instance()->GetMaxLevel(iID) )
			{
				printf("Upgrade failed because level max");
				return false;
			}
			if(iID!=1)
			{
				if(iLevel >= GetTownLevel())
					return false;	// can not upgrade level higher than town
			}

			int iGoldNeed = CCsvReaderMgr::Instance()->GetGoldCost(iID,iLevel+1);
			int iOilNeed  = CCsvReaderMgr::Instance()->GetOilCost(iID,iLevel+1);

			bool bRet = DecResource(iGoldNeed,iOilNeed);
			if(bRet == false)
			{
				// may be Plug-in so I will kick the player off
				return false;
			}
			if (_InsertToBuilder(iUniID,iID,iLevel+1) == false )
			{
				ReturnResource(iGoldNeed,iOilNeed);
				return false;
			}
			return true;
		}
	}

	printf("CPlayer::_UpgradeBuild can not find build uniID = %d",iUniID);
	return false;
}

bool CPlayer::_InsertToBuilder(int iUniID,int iID,int iLevelToUp)
{
	std::map<int,CBuilder*>::iterator it = m_mapBuilder.begin();
	for ( ; it!=m_mapBuilder.end() ;it++ )
	{
		CBuilder* pBuilder = it->second;
		if(pBuilder)
		{
			if( pBuilder->ReqLevelUp(iUniID,iID,iLevelToUp) == true)
			{
				return  true;
			}
		}
	}

	return false;

}

void CPlayer::ReturnResource(int iGoldNeed,int iOilNeed)
{
	AddResource(iGoldNeed,iOilNeed);
}

void CPlayer::OnUpgradeSuccess(int iUniID)
{
	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		if((*it).m_nUniID == iUniID)
		{
			int iID		= (*it).m_nID;
			int iLevel	= (*it).m_nLevel;
			(*it).m_nLevel += 1;
			(*it).m_bIsInLevelUp = false;
			_OnBuildUpGradeSuccess(iUniID,iID,(*it).m_nLevel);
			return;		
			//_AckClientBuildUpgradeSuccess(iUniID,iID,(*it).m_nLevel);
		}
	}

	printf("CPlayer::OnUpgradeSuccess can not find build uniID = %d",iUniID);
	return;
}

//void CPlayer::_AckClientBuildUpgradeSuccess(int iUniID,int iID,int iLevel)
//{
//	CAckBuildLevelUpEndProtocol ptc;
//	ptc.m_iUniID = iUniID;
//	ptc.m_iLevel = iLevel;
//	Send(ptc);
//}

void CPlayer::CaculateResourceMineStore()
{
	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		if((*it).m_nID == 2)	// 金矿
		{
			int iGoldValueAdd = GetGoldValueAdd((*it).m_nID,(*it).m_nLevel,m_StoreTime);
			int iOilValueAdd = 0;
			int iGoldStoreMax = CCsvReaderMgr::Instance()->GetGoldStore((*it).m_nID,(*it).m_nLevel);
			int iOilStoreMax  = CCsvReaderMgr::Instance()->GetOilStore((*it).m_nID,(*it).m_nLevel);
			AddtoStorgeData((*it).m_nUniID,iGoldValueAdd,iOilValueAdd,iGoldStoreMax,iOilStoreMax);
		}

		if((*it).m_nID == 3)	// 油矿
		{
			int iGoldValueAdd = 0;
			int iOilValueAdd = GetOilValueAdd((*it).m_nID,(*it).m_nLevel,m_StoreTime);
			int iGoldStoreMax = CCsvReaderMgr::Instance()->GetGoldStore((*it).m_nID,(*it).m_nLevel);
			int iOilStoreMax  = CCsvReaderMgr::Instance()->GetOilStore((*it).m_nID,(*it).m_nLevel);
			AddtoStorgeData((*it).m_nUniID,iGoldValueAdd,iOilValueAdd,iGoldStoreMax,iOilStoreMax);
		}
	}

	ResetStoreTime();
}


int CPlayer::GetGoldValueAdd(int iID,int iLevel,__time64_t lastStorTime )
{
	int iGoldOutPut =  CCsvReaderMgr::Instance()->GetGoldOutput(iID,iLevel);
	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime(); 
	iGoldOutPut *= (int)(now-lastStorTime);

	iGoldOutPut /= (60*60);

	if(iGoldOutPut <=0)
		iGoldOutPut=1;

	return iGoldOutPut;

}

int	CPlayer::GetOilValueAdd(int iID,int iLevel,__time64_t lastStorTime)
{
	int iOilOutPut =  CCsvReaderMgr::Instance()->GetOilOutput(iID,iLevel);
	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime(); 
	iOilOutPut *= (int)(now-lastStorTime);

	iOilOutPut /= (60*60);

	if(iOilOutPut <=0)
		iOilOutPut=1;

	return iOilOutPut;
}

void CPlayer::AddtoStorgeData(int iUniID,int & iGold, int & iOil,int iGoldMax, int iOilMax)
{
	// 输入需存数值
	// 返回成功存入数值
	std::vector<StorageData>::iterator it;
	for (it=m_oStorageData.begin();it!=m_oStorageData.end();it++)
	{
		if( (*it).m_nUniID == iUniID )
		{

			if((*it).m_nGoldStore + iGold > iGoldMax)
			{
				// 溢出最大值 
				iGold = iGoldMax - (*it).m_nGoldStore;
				
			}

			if((*it).m_nOilStore + iOil > iOilMax)
			{
				// 溢出最大值 
				iOil = iOilMax - (*it).m_nOilStore;

			}

			(*it).m_nGoldStore += iGold;
			(*it).m_nOilStore += iOil;
			return;
		}
	}

	// can not find this uni id so create it
	StorageData data;
	data.m_nUniID = iUniID;
	data.m_nGoldStore = iGold;
	data.m_nOilStore  = iOil;

	if(data.m_nGoldStore > iGoldMax)
	{
		data.m_nGoldStore = iGoldMax;
		iGold			  = iGoldMax;
	}
	if(data.m_nOilStore > iOilMax)
	{
		data.m_nOilStore = iOilMax;
		iOil			 = iOilMax;
	}
	m_oStorageData.push_back(data);

}

void CPlayer::ResetStoreTime()
{
	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime();
	m_StoreTime = now;
}

bool CPlayer::RestoreResourceFromMineToStorage(int iUniID,int & iGold,int & iOil)
{

	//要确保是金矿或者油矿才能采集 否则不可采集 比如大厅 金库 油库 都是有储量的 但是不能被采集 否则就循环了啊
	if ( _IsResourceMine(iUniID)  == false   )
	{
		return false;
	}

	if ( GetResourceStoredInMine(iUniID,iGold,iOil) == false )
	{
		return false;
	}

	//输入 现存有数值 返回 成功存入的数值
	_TryRestoreToStorage(iGold,iOil);	

	// 现存有的数值减去已经成功存入的数值
	std::vector<StorageData>::iterator it;
	for (it=m_oStorageData.begin();it!=m_oStorageData.end();it++)
	{
		if( (*it).m_nUniID == iUniID )
		{
			(*it).m_nGoldStore -= iGold;
			(*it).m_nOilStore  -= iOil;
			return true;
		}
	}
	return false;
}

bool CPlayer::GetResourceStoredInMine(int iUniID,int & iGold,int & iOil)
{
	std::vector<StorageData>::iterator it;
	for (it=m_oStorageData.begin();it!=m_oStorageData.end();it++)
	{
		if( (*it).m_nUniID == iUniID )
		{
			iGold  = (*it).m_nGoldStore;
			iOil   = (*it).m_nOilStore;	
			return true;
		}
	}
	return false;
}

void CPlayer::_TryRestoreToStorage(int & iGold ,int & iOil)
{
	// 输入 需存数值
	// 返回 成功存入数值


	//记录成功存入的数值
	int iGoldStoredSuccessTotal = 0;
	int iOilStoredSuccessTotal  = 0;

	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		if((*it).m_nID == 1  || (*it).m_nID == 4 || (*it).m_nID == 5) // 尝试存入	
		{
			// 方法1
			// 每次找到可存储建筑（大厅 金库 油库）之后 
			// 首先计算出该建筑所能存储的上限 以及已存的数量 既得到还可存入的量
			// 得到当前还待存的量 如其大于可存入量 则全部存入之后 还需继续下一个的寻找
			// 如果小于可存入量 则 全部存入之后 即可退出 而且所有都存储完毕

			// 方法2
			// 每次找到可存储建筑
			// 首先计算可存储上限
			// 然后调用接口 把当前所需要存储的数量 以及 此上限 传入接口
			// 接口返回成功存储的量 （因接口自身知道自己本身已经存有多少 所以他自己可以判断）
			// 根据接口返回的成功存储的量 可以用当前需要存储的量 减去接口成功存储的量 记得到下面还需要继续存储的量 已经更新已经成功存储的量

			//现使用方法2

			int iGoldStoreMax = CCsvReaderMgr::Instance()->GetGoldStore((*it).m_nID,(*it).m_nLevel);
			int iOilStoreMax  = CCsvReaderMgr::Instance()->GetOilStore((*it).m_nID,(*it).m_nLevel);
			
			int iWantToStoreGold = iGold;
			int iWantToStoreOil	 = iOil;

			AddtoStorgeData( (*it).m_nUniID ,iGold,iOil,iGoldStoreMax,iOilStoreMax  );
			//得到成功存入数值
			iGoldStoredSuccessTotal += iGold;
			iOilStoredSuccessTotal += iOil;

			// 将iGold从成功存入的数值转换为剩余的待存数值
			iGold = iWantToStoreGold - iGold;
			iOil  = iWantToStoreOil  - iOil;

			if(iGold == 0 && iOil == 0)
			{
				//如果全部存储完毕 则返回
				iGold = iGoldStoredSuccessTotal;
				iOil  = iOilStoredSuccessTotal;
				return;
			}
			else
			{
				continue;
			}

		}


	}

	//如遍历完毕 都没有完全存储完毕 
	iGold = iGoldStoredSuccessTotal;
	iOil  = iOilStoredSuccessTotal;
	return;	

}

void CPlayer::_AddResourceToTown(int iGold,int iOil)
{
	std::vector<StorageData>::iterator it;
	for (it=m_oStorageData.begin();it!=m_oStorageData.end();it++)
	{
		// 由于注册时把大厅注册为了UniID==1 而且不可删除 就直接存入此处即可
		// 未来如需考虑存到各个storage中的话 就需要考虑扩展此函数
		if( (*it).m_nUniID == 1 )
		{
			(*it).m_nGoldStore += iGold;
			(*it).m_nOilStore += iOil;
			return;
		}
	}

}

bool CPlayer::_IsResourceMine(int iUniID)
{
	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		if (it->m_nUniID == iUniID)
		{
			if(it->m_nID == 2 || it->m_nID == 3 )
			{
				return true;
			}
			return false;
		}
	}

	return false;
}

	
void CPlayer::InitBuildFactor()
{
	// init m_iTownLevel m_iPopulation campVec and lab

	m_iTownLevel = 0;
	m_iPopulation = 0;

	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		int iUniID = it->m_nUniID;
		int iID	   = it->m_nID;
		int iLevel = it->m_nLevel;

		if (iID == ETownID)
		{
			InitTownLevel(iLevel);
		}

		if(iID == ECampID)
		{
			// handle camp
			CCamp *pCamp = new CCamp(iUniID,iLevel,this);
			m_mapUniID2Camp[iUniID] = pCamp;

		}

		if(iID == EPopStationID)	// maybe later change id to type to calculate population
		{
			_AddPopulation(iLevel);
			int iPopuation = CCsvReaderMgr::Instance()->GetPopulationOutPut(EPopStationID,iLevel);
			CPopStation *pPupuStation = new CPopStation(iUniID,iLevel,iPopuation,this);
			m_mapUniID2PopuStation[iUniID] = pPupuStation;

		}

		if(iID == ELabID)
		{
			if(m_lab == NULL)
			{
				m_lab = new CLab(this,iLevel);
			}

		}

		if(iID == EBuilderID)
		{
			CBuilder *pBuilder = new CBuilder(iUniID,this);
			m_mapBuilder[iUniID] = pBuilder;
		}
	}
}

void CPlayer::InitLabData(const LabData & labdata)
{
	if(m_lab && labdata.m_bUse == 1)
	{	
		m_lab->InitData(labdata);	
	}
}

void CPlayer::InitBuilderData(const std::vector<CBuilderData>& vecBuilderData)
{
	std::vector<CBuilderData>::const_iterator it = vecBuilderData.begin();
	for( ; it!=vecBuilderData.end() ; it++ )
	{
		int iUniID			=	(*it).m_iUniID;
		int iUniIDInLevelUp =	(*it).m_iUniIDInLevelUp;
		int iLevel			=	(*it).m_iLevelInLevelUp;
		__time64_t timeEnd	=	(*it).m_TimeEnd;

		int iIDInLevelUp	=	GetBuildID(iUniIDInLevelUp);
		
		std::map<int,CBuilder*>::iterator itBuilder = m_mapBuilder.find(iUniID);
		if(itBuilder==m_mapBuilder.end())
		{
			printf("Fatal Error no this builder\n");
		}
		else
		{
			CBuilder *pBuilder = itBuilder->second;
			pBuilder->InitBuilderData(iUniID,iUniIDInLevelUp,iIDInLevelUp,iLevel,timeEnd);	
		}
	}
}

void CPlayer::InitCampData(const std::map<int,CampData>& mapCampData)
{
	std::map<int,CampData>::const_iterator it = mapCampData.begin();
	for( ; it!=mapCampData.end() ; it++ )
	{
		int			iUniID		= it->first;
		bool		bIsCreating	= it->second.m_bInCreating;
		__time64_t	timeEnd		= it->second.m_TimeEndCreating;
		int	iSoldierIDCreated	= it->second.m_iSoldierIDCreated; 
		int	iPopulationInWaiting= it->second.m_iPopulationInWaiting;
		const std::vector<int>& vecSoldierList = it->second.m_vecSoldierWaitID;
		const std::map<int,int>& mapSoldierWaitNum = it->second.m_mapSoldierWaitNum;


		std::map<int,CCamp*>::iterator itCamp = m_mapUniID2Camp.find(iUniID);
		if(itCamp==m_mapUniID2Camp.end())
		{
			printf("Fatal Error no this Camp UniID= %d \n",iUniID);
		}
		else
		{
			CCamp *pCamp = itCamp->second;
			pCamp->InitCampData(bIsCreating,timeEnd,iSoldierIDCreated,iPopulationInWaiting,vecSoldierList,mapSoldierWaitNum);	
		}
	}
	
	//
	while(true)
	{
		//初始化camp之后 要看这时候有哪些兵是下线的这段时间造出来的
		//GetFirstEndCamp 其中 如果不是creating状态的 不要 因为证明下线前已经停止制造了（比如因为没有待造队列 或者外部人口无法将已经create的output出去）
		//iSoldierIDCreated 不是-1的不要 因为证明这时候已经造好了一个 却无法output（否则在下线前应该已经output了）
		//摒弃这两种情况之后 获得timeend 并比较哪个比较小一点 取得最小的 如果这时候没有符合条件的 或者最小的timeend已经大于now的时间 则返回null 
		CCamp *pCamp = 	GetFirstEndCamp();
		if(pCamp == NULL)
		{
			return;
		}
		else
		{
			pCamp->TryToCompleteTrainAndOutputOnLogin();
		}
	}


}

void CPlayer::InitPopuStationData(const std::map<int,CPopStationData>& mapPopStationData)
{
	// 
	std::map<int,int>::iterator itS = m_mapSoldierList.begin();
	for ( ; itS!=m_mapSoldierList.end() ;itS++ )
	{
		int iSID = itS->first;
		int iCount = itS->second;
		int iSoldierPopulation = CCsvReaderMgr::Instance()->GetSoldierPopulaiton(iSID);

		for(int i = 0; i<iCount ; i++)
		{
			_AddToPopStation(iSID,iSoldierPopulation);
		}
	}

	return;

	std::map<int,CPopStationData>::const_iterator it = mapPopStationData.begin();
	for( ; it!=mapPopStationData.end() ; it++ )
	{
		int			iUniID			= it->first;
		//int			iCurPopulation	= it->second.m_iCurPopulation;
		//const std::map<int,int>& mapSoldierID2Count = it->second.m_mapID2Count;

		std::map<int,CPopStation*>::iterator itPopStation = m_mapUniID2PopuStation.find(iUniID);
		if(itPopStation==m_mapUniID2PopuStation.end())
		{
			printf("Fatal Error no this PopStation UniID= %d \n",iUniID);
		}
		else
		{
			CPopStation *pPopStation = itPopStation->second;
			pPopStation->InitData(it->second);	
		}
	}
}
void CPlayer::InitWarReport(const std::vector<CPlayerWarReportData> &vPlayerWarReport)
{
	m_pPlayerWarReport = new CPlayerWarReport(this);
	if(m_pPlayerWarReport)
	{
		m_pPlayerWarReport->SetWarReport(vPlayerWarReport);
	}

}
CCamp * CPlayer::GetFirstEndCamp()
{
	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime();
	std::map<int,CCamp*>::iterator itCamp = m_mapUniID2Camp.begin();

	__time64_t timeendFirst = now;
	CCamp *pCampEndFirst    = NULL;

	for( ; itCamp!=m_mapUniID2Camp.end() ;itCamp++ )
	{
		CCamp *pCamp = itCamp->second;
		if(pCamp->GetIsCreating()==false)
			continue;
		if(pCamp->GetSoldierCreated()!=-1)
			continue;

		__time64_t timeend = pCamp->GetTimeEndCreating();
		if(timeend < timeendFirst)
		{
			pCampEndFirst = pCamp;
			timeendFirst  = timeend;
		}
	}
	return pCampEndFirst;

}


void CPlayer::_AddPopulation(int iLevel)
{
	int iPopuation = CCsvReaderMgr::Instance()->GetPopulationOutPut(EPopStationID,iLevel);

	m_iPopulation += iPopuation;
}

int CPlayer::GetPopulationUpgrade(int iLevel)
{
	int iPopuation = CCsvReaderMgr::Instance()->GetPopulationOutPut(7,iLevel);
	if(iLevel == 1)
		return iPopuation;
	else
	{
		int iPopuationLast = CCsvReaderMgr::Instance()->GetPopulationOutPut(7,iLevel-1);
		return iPopuation-iPopuationLast;
	}


}

void CPlayer::InitTownLevel(int iLevel)
{
	m_iTownLevel = iLevel;	
}

int CPlayer::GetTownLevel()
{
	return m_iTownLevel;
}

void CPlayer::IncreaseTownLevel()
{
	m_iTownLevel++;
}

bool CPlayer::CheckTowmLimit(int iID)
{
	int iBuildCount = GetBuildCount(iID);

	int iTownLevel = GetTownLevel();
	int iCountLimit = CCsvReaderMgr::Instance()->GetBuildLimitCount(iTownLevel,iID);
	if(iBuildCount>=iCountLimit)
		return false; //build of this id attach the limit 

	return true;

}

bool CPlayer::_DecGoldFromStore(int iValue)
{
	// 尝试从大厅或者金库中减去金币 
	// 理论上 在decgold中执行成功才会掉此处 所以理论上应该不至于出现store中出现不足钱的情况
	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		if((*it).m_nID == 1  || (*it).m_nID == 4)
		{
			int iWantoDecGold = iValue;
			// 输入想要减去的金钱 返回成功减去的金钱
			_TryDecGoldFromStore((*it).m_nUniID ,iValue); 
			
			iValue = iWantoDecGold - iValue;
			if(iValue <= 0)
			{
				return true;
			}
		}
	}
	// 并没有完全成功地减去 理论上 这是个bug
	//DEBUG
	printf("Fatal Error we can not dec all gold  \n ");
	return false;
}

bool CPlayer::_DecOilFromStore(int iValue)
{
	std::vector<BuildingData>::iterator it = m_oBuildings.begin();
	for (;it!=m_oBuildings.end();it++)
	{
		if((*it).m_nID == 1  || (*it).m_nID == 5)
		{
			int iWantoDecOil = iValue;
			// 输入想要减去的金钱 返回成功减去的金钱
			_TryDecOilFromStore((*it).m_nUniID ,iValue); 

			iValue = iWantoDecOil - iValue;
			if(iValue <= 0)
			{
				return true;
			}
		}
	}
	// 并没有完全成功地减去 理论上 这是个bug
	//DEBUG
	printf("Fatal Error we can not dec all oil  \n ");
	return false;
}

void CPlayer::_TryDecGoldFromStore(int iUniID, int iValue )
{
	std::vector<StorageData>::iterator it;
	for (it=m_oStorageData.begin();it!=m_oStorageData.end();it++)
	{
		if( (*it).m_nUniID == iUniID )
		{
			// 所存储的金额不足够 那成功减去的金钱就变为现存的最大值
			if((*it).m_nGoldStore  <  iValue)
			{
				iValue = (*it).m_nGoldStore;
			}
			(*it).m_nGoldStore -= iValue;
			return;
		}
	}
	// 居然没找到对应的uniID
	iValue = 0;//成功减去的金额为0
	printf("Fatal Error in _TryDecGoldFromStore we can not find uniID = %u in storage data\n ",iUniID);

}

void CPlayer::_TryDecOilFromStore(int iUniID, int iValue )
{
	std::vector<StorageData>::iterator it;
	for (it=m_oStorageData.begin();it!=m_oStorageData.end();it++)
	{
		if( (*it).m_nUniID == iUniID )
		{
			if((*it).m_nOilStore  <  iValue)
			{
				iValue = (*it).m_nOilStore;
			}
			(*it).m_nOilStore -= iValue;
			return;
		}
	}
	iValue = 0;
	printf("Fatal Error in _TryDecOilFromStore we can not find uniID = %u in storage data\n ",iUniID);
}


void CPlayer::CaculateCampToSoldierList()
{

}

CCamp *	CPlayer::GetCamp(int iUniID)
{
	std::map< int,CCamp*>::iterator it = m_mapUniID2Camp.find(iUniID);
	if (it!=m_mapUniID2Camp.end())
	{
		return it->second;
	}
	return NULL;

}

void CPlayer::_AddtoSoldierMap(int iSoldierID)
{
	std::map<int,int>::iterator it = m_mapSoldierList.find(iSoldierID);
	if(it!=m_mapSoldierList.end())
	{
		it->second++;
	}
	else
	{
		m_mapSoldierList[iSoldierID] = 1;
	}
}

void CPlayer::_SyncPopulation()
{	
	CSyncPopulationProtocol ptc;
	ptc.m_iPopulation = m_iPopulation;
	Send(ptc);
}