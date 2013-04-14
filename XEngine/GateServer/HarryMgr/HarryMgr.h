#ifndef _HarryMgr_H__
#define _HarryMgr_H__

#include "../protocol/Coder_Encoder/pch.h"
#include "../protocol/buildingdata.h"

class CHarryMgr
{
	CHarryMgr();
	~CHarryMgr();
	DECLARE_SINGLETON(CHarryMgr)

public:
	bool Init();
	void SearchOpponentForPlayer(UINT32 dwConnID);
	void OnFindOpponent(UINT32 dwConnID, int iAccountID,std::string strName, const std::vector<BuildingData> & vecBuildings,const std::vector<StorageData> & vecStorageDatas);
};

#endif