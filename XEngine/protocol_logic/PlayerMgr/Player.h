#ifndef _Player_H__
#define _Player_H__

#include "../protocol/Coder_Encoder/pch.h"
#include "../protocol/buildingdata.h"

class CLab;
class CCamp;
class CProtocol;
class CBuilder;

class CPlayer
{
public:
	CPlayer ( UINT32 dwConnID,int iAccount,std::string strName,int iRMB ,int iGold ,int iOil, 
			const std::vector<BuildingData> & rVecBuilding,const std::vector<StorageData> & rVecStoreData, __time64_t storetime,const LabData & labdata,
			const SoldierData& soldierData,const std::vector<CBuilderData>& vecBuilderData,const std::map<int,CampData>& mapCampData);
	~CPlayer();

public:
	void		ProcessPlayer();
	void		SavePlayer();
	bool		Send(const CProtocol& roPtc);


	UINT32		GetConnID();
	int			GetAccountID();
	std::string GetName();
	int			GetRMB();
	int			GetGold();
	int			GetOil();
	__time64_t  GetStoreTime();
	int			GetPopulation();
	int			GetSoldierLevel(int iSoldierID);
	int			GetCurSoldierPopulation();
	const std::vector<BuildingData> & GetBuildingDataVec();
	const std::vector<StorageData> & GetStorageDataVec();
	void		GetLabData(LabData & data);
	void		GetSoldierData(SoldierData & data);
	void		GetCampDataToClient(std::map<int,CCampDataToClient> & mapUniID2CampDataToClient);
	void		GetCampData(std::map<int,CampData> & mapCampData);
	void		GetBuilderDataToClient(std::vector<CBuilderDataToClient> & vecBuilderDataToClient);
	void		GetBuilderData( std::vector<CBuilderData> & vecBuilderData);

	int			GetBuildID(int iUniID);

	void		InitLabData(const LabData & labdata);
	void		InitBuilderData(const std::vector<CBuilderData>& vecBuilderData);
	void		InitCampData(const std::map<int,CampData>& mapCampData);
	void		InitSoldierListData();

	void		AddRMB(int iValue);
	void		AddGold(int iValue);
	void		AddOil(int iValue);
	bool		DecRMB(int iValue);
	bool		DecGold(int iValue);
	bool		DecOil(int iValue);
	void		AddResource(int iGold,int iOil);
	bool		DecResource(int iGold,int iOil);

	bool		AddBuild(int iUniID,int iID,int iLevel,int iGridX,int iGridY);
	bool		UpgradeBuild(int iUniID);
	void		EraseBuild(int iUniID);
	void		MoveBuild(int iUniID,int iGridX,int iGridY);
	void		OnBuildAddSuccess(int iUniID,int iID);
	void		OnUpgradeSuccess(int iUniID);
	void		CollectBuildResource(int iUniID,int iGold,int iOil);

	bool		DecSearchGold();
	void		HarryResource(int iGold,int iOil);

	void		ReqOutputSoldier(int iUniID,int iSoldierID);
	void		ReqCreateSoldier(int iUniID,int iSoldierID);
	void		ReqUpgradeSoldier(int iSoldierID);
	bool		AddToSoldierList(int iSoldierID);

	void		OnPutSoldier(int iSID);

private:
	void 		RemoveFromSoldierList(int iSoldierID);



private:

	void		_AutoSavePlayer();

	bool		_InsertToBuilder(int iUniID,int iID,int iLevelToUp);
	void		ReturnResource(int iGoldNeed,int iOilNeed);

	void		_OnBuildUpGradeSuccess(int iUniID,int iID,int iLevel);

	bool		IsBuildExist(int iUniID);
	int			GetBuildCount();
	int			GetBuildCount(int iID);


	void		_EraseBuild(int iUniID);
	void		_MoveBuild(int iUniID,int iGridX,int iGridY);
	bool		_UpgradeBuild(int iUniID);
//	void 		_AckClientBuildUpgradeSuccess(int iUniID,int iID,int iLevel);

	void		CaculateResourceMineStore();
	int			GetGoldValueAdd(int iID,int iLevel,__time64_t lastStorTime );
	int			GetOilValueAdd(int iID,int iLevel,__time64_t lastStorTime);
	void		AddtoStorgeData(int iUniID,int & iGold, int & iOil,int iGoldMax, int iOilMax);
	void		ResetStoreTime();

	bool		RestoreResourceFromMineToStorage(int iUniID,int & iGold,int & iOil);
	bool		GetResourceStoredInMine(int iUniID,int & iGold,int & iOil);
	void		_TryRestoreToStorage(int & iGold ,int & iOil);
	void		_AddResourceToTown(int iGold,int iOil);

	bool		_IsResourceMine(int iUniID);

	void		InitBuildFactor();

	void		InitTownLevel(int iLevel);	
	int			GetTownLevel();
	void		IncreaseTownLevel();
	bool		CheckTowmLimit(int iID);

	void		_AddPopulation(int iLevel);
	int			GetPopulationUpgrade(int iLevel);

	bool        _DecGoldFromStore(int iValue);
	bool        _DecOilFromStore(int iValue);

	void		_TryDecGoldFromStore(int iUniID, int iValue );
	void		_TryDecOilFromStore(int iUniID, int iValue );

	void		CaculateCampToSoldierList();

	CCamp *		GetCamp(int iUniID);

	void		_AddtoSoldierMap(int iSoldierID);

	void 		_SyncPopulation();

	CCamp * 	GetFirstEndCamp();

private:

	__time64_t  m_lastSaveTime;
	UINT32		m_dwConnID;
	int			m_iAccount;
	std::string m_strName;
	int			m_iRMB;

	int			m_iGold;
	int			m_iOil;

	__time64_t	m_StoreTime;
	int			m_iTownLevel; 
	int			m_iPopulation;

	std::vector<BuildingData>	m_oBuildings;
	std::vector<StorageData>	m_oStorageData;

	std::map<int,CBuilder*>		m_mapBuilder;
	std::map<int,CCamp*>		m_mapUniID2Camp;
	CLab*						m_lab;

	std::map<int,int>			m_mapSoldierList;
	int							m_iSoldierCurrentPopulation;

};

#endif