#ifndef _PlayerMgr_H__
#define _PlayerMgr_H__

#include "../protocol/Coder_Encoder/pch.h"
#include <map>

class CPlayer;
class BuildingData;
class StorageData;
class LabData;
class SoldierData;
class CBuilderData;
class CampData;


class CPlayerMgr
{
	CPlayerMgr();
	~CPlayerMgr();
	DECLARE_SINGLETON(CPlayerMgr)
public:
	bool Init();
	void UnInit();

public:
	bool OnLogin(UINT32 dwConnID,int  iAccountID, std::string strName, int iRMB,int iGold , int iOil ,
		const std::vector<BuildingData> & rVecBuilding,const std::vector<StorageData> & rVecStoreData, __time64_t storetime,const LabData & labdata,
		const SoldierData & soldierdata,const std::vector<CBuilderData>& vecBuilderData,const std::map<int,CampData>& mapCampData );

	CPlayer* GetPlayerByConnID(UINT32 dwConnID);
	CPlayer* GetPlayerByAccountID(int iAccount);
	CPlayer* GetPlayerByName(std::string strName);

	void ReleasePlayer(UINT32 dwConnID);

	void ProcessPlayerMgr();

private:
	void KickOffLine(int iAccountID);
	void KickOffLine( std::string strName );

private:
	std::map<UINT32 , CPlayer*> m_MapConnID2Player;
	std::map<int	, CPlayer*> m_MapAccountID2Player;
	std::map<std::string , CPlayer*> m_MapName2Player;
};

#endif