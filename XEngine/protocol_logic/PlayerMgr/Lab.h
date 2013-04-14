#ifndef _Lab_H__
#define _Lab_H__

#include "../protocol/Coder_Encoder/pch.h"

#include <map>

class CPlayer;
class LabData;

class CLab
{
public:
	CLab (CPlayer * pPlayer,int iLevel);
	~CLab();
	void InitData(const LabData & labdata);
	void GetLabData(LabData & data);

	void ProcessLab();
	bool ReqUpgradeSoldier(int iSoldierID);

	void OnUpgradeLab(int iLevel);

	int	 GetSoldierLevel(int iSoldierID);


private:
	bool IsInUpgrade();
	int  GetInUpgradeSoldierID();

	void _UpGradeSoldier(int iSoldierID,int iTimeCost,int iOilCost);
	void AckUpgradeSoldierSuccessBegin(int iOilCost);
	void AckUpgradeSoldierSuccessEnd(int iSoldierID);

	bool IsUnLock(int iSoldierID);

private:
	int			m_iSoldierIDInUpgrade;
	__time64_t	m_TimeBeginUpgrade;
	__time64_t	m_TimeEndUpgrade;

	std::map<int,int> m_mapSoldierID2Level;

	CPlayer *	m_Player;
	int			m_iLevel;

};

#endif