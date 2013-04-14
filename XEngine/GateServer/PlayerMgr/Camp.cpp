#include "Camp.h"
#include "../CSVData/CsvReaderMgr.h"

#include "Player.h"
#include <atltime.h>

#include "../protocol/PtcSoldier/PtcReqCreateSoldier.h"
#include "../protocol/PtcSoldier/PtcReqOutputSoldier.h"

CCamp::CCamp (int iUniID , int iLevel,CPlayer *pPlayer)
{
	m_iUniID = iUniID;
	m_iLevel = iLevel;
	m_Player = pPlayer;

	m_bInCreating = false;
	m_iPopulationInWaiting = 0;

	m_iSoldierIDCreated = -1;

	m_iMaxPopulationWaiting	= CCsvReaderMgr::Instance()->GetCampMaxPopulation(6,iLevel);

	 
}

CCamp::~CCamp()
{ 
}

void CCamp::InitCampData(bool bIsCreating,__time64_t timeEnd,int  iSoldierIDCreated,int iPopulationInWaiting,
						 const std::vector<int>& vecSoldierList,const std::map<int,int>& mapSoldierWaitNum )
{
	m_bInCreating = bIsCreating;
	m_TimeEndCreating = timeEnd;
	m_iSoldierIDCreated = iSoldierIDCreated;
	m_iPopulationInWaiting = iPopulationInWaiting;
	m_vecSoldierWaitID = vecSoldierList;
	m_mapSoldierWaitNum = mapSoldierWaitNum;
}

void CCamp::ProcessCamp()
{

	if(m_bInCreating == false)
	{
		if( _HasCreatedSoldier() == true )
		{
			if(	_AddToPlayerSoldierList() == true)
			{
				_BeginCreate(0);	
			}
		}
		return;
	}

	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime();

	if(now > m_TimeEndCreating)
	{
		int iSoldierIDInCreating = m_vecSoldierWaitID[0];
		std::map<int,int>::iterator it =  m_mapSoldierWaitNum.find(iSoldierIDInCreating);
		if(it != m_mapSoldierWaitNum.end())
		{
			it->second--;
			if(it->second <= 0)
			{
				m_mapSoldierWaitNum.erase(it);
				m_vecSoldierWaitID.erase(m_vecSoldierWaitID.begin());
				// here i think vector erase is not Efficient maybe i'd like change vector to list

			}
		}	
		else
		{
			printf("error CCamp::ProcessCamp id=%d",iSoldierIDInCreating);
		}
		
		m_iSoldierIDCreated = iSoldierIDInCreating;
		_OnSuccessCreateOneSoldier(iSoldierIDInCreating);

	}

}

void CCamp::TryToCompleteTrainAndOutputOnLogin()
{
	if(m_vecSoldierWaitID.size()==0)
	{
		m_bInCreating = false;
		return;
	}

	int iSoldierIDInCreating = m_vecSoldierWaitID[0];
	std::map<int,int>::iterator it =  m_mapSoldierWaitNum.find(iSoldierIDInCreating);
	if(it != m_mapSoldierWaitNum.end())
	{
		it->second--;
		if(it->second <= 0)
		{
			m_mapSoldierWaitNum.erase(it);
			m_vecSoldierWaitID.erase(m_vecSoldierWaitID.begin());
			// here i think vector erase is not Efficient maybe i'd like change vector to list

		}
	}	
	else
	{
		printf("error CCamp::ProcessCamp id=%d",iSoldierIDInCreating);
	}

	m_iSoldierIDCreated = iSoldierIDInCreating;

	m_bInCreating = false;

	if(	_AddToPlayerSoldierList() == true)
	{
		_BeginCreate(m_TimeEndCreating);	
	}

}

bool CCamp::ReqCreateSoldier(int iSoldierID)
{
	if(IsUnLock(iSoldierID) == false)
	{	
		//尚未解锁
		return false;
	}
	
	if( CheckPopulation(iSoldierID) == false )
	{
		//人口不足
		return false;
	}

	
	//int iOilCost  = CCsvReaderMgr::Instance()->GetSoldierUpgradeOilCost(iSoldierID,iLevel+1); 
	int iOilCost = GetOilCost(iSoldierID);

	if( m_Player &&
		m_Player->DecOil(iOilCost) == true)
	{
		_InsertToWaitingList(iSoldierID);
		CAckSoldierInCreatingProtocol ptc;
		ptc.m_iUniID = m_iUniID;
		ptc.m_iSoldierID = iSoldierID;
		ptc.m_iOilCost = iOilCost;
		if(m_Player)
		{
			m_Player->Send(ptc);
		}


		_BeginCreate(0);

		return true;
	}
	else
	{
		ReturnPopulation(iSoldierID);
		printf("error ReqCreateSoldier player have not enough oil soldierid=%u, need oil = %d ",iSoldierID,iOilCost);
		return false;
	}

}

bool CCamp::ReqCancelCreateSoldier(int iSID)
{
	std::map<int,int>::iterator it =  m_mapSoldierWaitNum.find(iSID);
	if(it != m_mapSoldierWaitNum.end())
	{
		it->second--;
		if(it->second <= 0)
		{
			m_mapSoldierWaitNum.erase(it);

			if( m_vecSoldierWaitID.size()<=0 )
			{
				printf("BUG SID %d Exist in map but not in vector\n ");
				return true;
			}
			int iSIDCreating = m_vecSoldierWaitID[0];
			if( iSIDCreating != iSID )
			{
				// 要去掉的这一种类士兵 如果不是第一列 （正在建造的） 则直接简单去掉即可

				std::vector<int>::iterator it = m_vecSoldierWaitID.begin();
				for(;it!=m_vecSoldierWaitID.end();it++)
				{
					if( (*it) == iSID )
					{
						m_vecSoldierWaitID.erase(it);
						return true;	
					}
				}
			}			
			else
			{
				//如果是第一列 也即正在建造的士兵 那么要停止建造 并尝试继续建造下一列
				m_vecSoldierWaitID.erase(m_vecSoldierWaitID.begin());
				m_bInCreating = false;
				_BeginCreate(0);
				return true;
			}

		}
	}	
	else
	{
		// can  not find in wait map 
		// maybe is created so
		if (iSID == m_iSoldierIDCreated)
		{
			m_iSoldierIDCreated = -1;
			m_bInCreating = false;
			_BeginCreate(0);
		}

	}

	return true;
}

bool CCamp::ReqOutputSoldier(int iSoldierID)
{
	// 此接口取消 统一由服务器计算正在生产的士兵有没有产完

	// 暂时先由客户端向服务器发出申请获得士兵
	// 服务器暂时不予以验证（以后需要验证可以加） 直接从待生产队列中移除 兵加入到player的soldier队列中
	// 也即是已生产完毕的队列中
	std::map<int,int>::iterator it = m_mapSoldierWaitNum.find(iSoldierID);
	if(it!=m_mapSoldierWaitNum.end())
	{
		// in futuer maybe add count but now one by one
		if( it->second >= 1)
		{
			it->second -= 1;
			return true;
		}
		printf("so horrible bug server do not have this soldier but client req what happen? ");
		return false;
		// if it is 0 keep it zero
		// I think no problem

	}
	else
	{
		printf("so horrible bug server do not have this soldier but client req what happen? ");
		return false;
	}
}

void CCamp::LevelUp(int iLevel)
{
	m_iLevel = iLevel;
	//升级之后 重新计算人口上限和解锁士兵列表
	m_iMaxPopulationWaiting	= CCsvReaderMgr::Instance()->GetCampMaxPopulation(6,iLevel);
}

bool CCamp::IsUnLock(int iSoldierID)
{
	// 根据level来决定是否解锁 暂时认为都解锁了
	return true;
}


int CCamp::GetUniID()
{
	return m_iUniID;
}

int	CCamp::GetLevel()
{
	return m_iLevel;
}
	
bool CCamp::GetIsCreating()
{
	return m_bInCreating;
}


__time64_t CCamp::GetTimeEndCreating()
{
	return m_TimeEndCreating;
}

int CCamp::GetSoldierCreated()
{
	return m_iSoldierIDCreated;
}

int CCamp::GetPopulationInWaiting()
{
	return m_iPopulationInWaiting;
}

int CCamp::GetPopulaitonMax()
{
	return m_iMaxPopulationWaiting;
}

const std::vector<int> & CCamp::GetVecSoldierWaitingID()
{
	return m_vecSoldierWaitID;
}

const std::map<int,int> & CCamp::GetMapSoldierWaitingNum()
{
	return m_mapSoldierWaitNum;
}


void CCamp::_InsertToWaitingList(int iSoldierID)
{
	std::map<int,int>::iterator it = m_mapSoldierWaitNum.find(iSoldierID);
	if(it!=m_mapSoldierWaitNum.end())
	{
		// in futuer maybe add count but now one by one
		it->second += 1;
	}
	else
	{
		m_mapSoldierWaitNum[iSoldierID] = 1;
		m_vecSoldierWaitID.push_back(iSoldierID);
	}
}

void CCamp::_BeginCreate(__time64_t tBeginTime)
{
	if(m_bInCreating == true)
	{
		return;
	}

	if(m_vecSoldierWaitID.size()==0)
	{
		return;
	}

	if (m_iSoldierIDCreated != -1)
	{
		return;
	}

	m_bInCreating = true;

	int iSoldierID = m_vecSoldierWaitID[0];
	int iTrainingTime = CCsvReaderMgr::Instance()->GetSoldierTrainTime(iSoldierID);

	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime();
	
	if(tBeginTime == 0)
	{
		m_TimeBeginCreating = now;
	}
	else
	{
		m_TimeBeginCreating = tBeginTime;
	}

	m_TimeEndCreating	= m_TimeBeginCreating+iTrainingTime;

	if(tBeginTime != 0)
	{
		return;
	}

	CAckSoldierBeginCreateProtocol ptc;
	ptc.m_iUniID = m_iUniID;
	ptc.m_TimeEnd = m_TimeEndCreating;
	if(m_Player)
	{
		m_Player->Send(ptc);
	}
}

void CCamp::_OnSuccessCreateOneSoldier(int iSoldierID)
{
	
	_AckCreateSoldier(iSoldierID);
	m_bInCreating = false;

	if(	_AddToPlayerSoldierList() == true)
	{
		_BeginCreate(0);	
	}
	else
	{
		//插入不成功 則證明外部人口滿 則此處保持阻塞狀態
	}

}

void CCamp::_AckCreateSoldier(int iSoldierID)
{
	if( m_Player )
	{
		CAckCreateSoldierProtocol ptc; 
		ptc.m_iUniID	 = m_iUniID;
		ptc.m_iSoldierID = iSoldierID;
		m_Player->Send(ptc);
	}
}

void CCamp::_AckOutputSoldier(int iSoldierID,int iUniID,int iCurSoldierPopulation)
{
	if( m_Player )
	{
		CAckOutputSoldierProtocol ptc; 
		ptc.m_iUniID	 = iUniID;
		ptc.m_iSoldierID = iSoldierID;
		ptc.m_iCurSoldierPopulaiton = iCurSoldierPopulation;
		m_Player->Send(ptc);
	}
}

bool CCamp::CheckPopulation(int iSoldierID)
{
	int iPopu = CCsvReaderMgr::Instance()->GetSoldierPopulaiton(iSoldierID);
	if(iPopu == -1)
	{
		printf("BUG CCamp::CheckPopulation\n");
		return false;
	}

	if (iPopu + m_iPopulationInWaiting > m_iMaxPopulationWaiting)
	{
		return false;
	}
	else
	{
		m_iPopulationInWaiting	+= iPopu;
		return true;
	}
}

int CCamp::GetOilCost(int iSoldierID)
{
	if( m_Player  )
	{
		int iSoldierLevel = m_Player->GetSoldierLevel(iSoldierID);
		int iOilCost = CCsvReaderMgr::Instance()->GetSoldierOilCost(iSoldierID,iSoldierLevel);
		return iOilCost;
	}
	return 0;
}

void CCamp::ReturnPopulation(int iSoldierID)
{
	int iPopu = CCsvReaderMgr::Instance()->GetSoldierPopulaiton(iSoldierID);
	if(iPopu == -1)
	{
		printf("BUG CCamp::ReturnPopulation\n");
		return;
	}
	m_iPopulationInWaiting -= iPopu;

}

bool CCamp::_HasCreatedSoldier()
{
	if(m_iSoldierIDCreated == -1)
		return false;

	return true;
}

bool CCamp::_AddToPlayerSoldierList()
{
	if(m_iSoldierIDCreated == -1)
	{
		return false;
	}

	if(m_Player)
	{
		if( m_Player->AddToSoldierList(m_iSoldierIDCreated)== true)
		{
			ReturnPopulation(m_iSoldierIDCreated);
			_AckOutputSoldier(m_iSoldierIDCreated,m_iUniID,m_Player->GetCurSoldierPopulation());
			m_iSoldierIDCreated = -1;
			m_Player->SavePlayer();
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}