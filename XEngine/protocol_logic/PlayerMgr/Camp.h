#ifndef _Camp_H__
#define _Camp_H__

#include "../protocol/Coder_Encoder/pch.h"

#include <vector>
#include <map>

class CPlayer; 

class CCamp
{
public:
	CCamp (int iUniID , int iLevel,CPlayer *pPlayer);
	~CCamp();

	void    InitCampData(bool bIsCreating,__time64_t timeEnd,int  iSoldierIDCreated,int iPopulationInWaiting,const std::vector<int>& vecSoldierList,const std::map<int,int>& mapSoldierWaitNum );	

	void	ProcessCamp();

	bool	ReqCreateSoldier(int iSoldierID);
	bool	ReqCancelCreateSoldier(int iSoldierID);
	bool	ReqOutputSoldier(int iSoldierID);

	void	LevelUp(int iLevel);

	bool	IsUnLock(int iSoldierID);

	int			GetUniID();
	int			GetLevel();
	bool		GetIsCreating();
	__time64_t	GetTimeEndCreating();
	int			GetSoldierCreated();
	int			GetPopulationInWaiting();
	int			GetPopulaitonMax();
	const std::vector<int> & GetVecSoldierWaitingID();
	const std::map<int,int> & GetMapSoldierWaitingNum();

	void	TryToCompleteTrainAndOutputOnLogin();

private:
	void	_InsertToWaitingList(int iSoldierID);	
	void	_BeginCreate(__time64_t tBeginTime);
	void	_OnSuccessCreateOneSoldier(int iSoldierID);
	void	_AckCreateSoldier(int iSoldierID);
	void	_AckOutputSoldier(int iSoldierID,int iUniID,int iCurSoldierPopulation);

	bool	CheckPopulation(int iSoldierID);
	int		GetOilCost(int iSoldierID);
	void	ReturnPopulation(int iSoldierID);

	bool	_HasCreatedSoldier();
	bool	_AddToPlayerSoldierList();

private:
	int			m_iUniID;
	int			m_iLevel;
	CPlayer *	m_Player;

	bool		m_bInCreating;
	__time64_t	m_TimeBeginCreating;
	__time64_t	m_TimeEndCreating;

	int			m_iSoldierIDCreated;

	int			m_iPopulationInWaiting;
	int			m_iMaxPopulationWaiting;	//等待队列的人口上限

	std::vector<int>  m_vecSoldierWaitID;
	std::map<int,int> m_mapSoldierWaitNum;




};

#endif