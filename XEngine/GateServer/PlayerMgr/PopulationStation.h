#ifndef _PopStation_H__
#define _PopStation_H__

#include "../protocol/Coder_Encoder/pch.h"

#include <map>

class CPlayer;
class CPopStationData;


class CPopStation
{
public:
	CPopStation (int iUniID , int iLevel, int iMaxPopulation ,CPlayer *pPlayer);
	~CPopStation();
	void InitData(const CPopStationData & data);
	void GetData(CPopStationData & data);

	bool TryAddToPopStation(int iSID,int iPopulation);
    bool TryRemoveFromPopStation(int iSID,int iPopulation);

	void OnUpgradePopStation(int iLevel);

private:
	void _InsertToMap(int iSID);
	bool _RemoveFromMap(int iSID);
	
private:
	int			m_iUniID;
	CPlayer *	m_Player;
	int			m_iLevel;
	int			m_iMaxPopulation;

	int				  m_iCurPopulaiton;
	std::map<int,int> m_mapSoldierID2Count;




};

#endif