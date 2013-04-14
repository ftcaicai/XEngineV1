#ifndef _Player_H__
#define _Player_H__

#include "../protocol/Coder_Encoder/pch.h"
#include "../protocol/buildingdata.h"
#include "PlayerWarReport.h"

enum EBuildID
{
	ETownID			= 1,
	EGoldMine		= 2,
	EOilMine		= 3,
	EGoldStore		= 4,
	EOilStore		= 5,
	ECampID			= 6,
	EPopStationID	= 7,
	ELabID			= 8,
	EBuilderID		= 11,
	ECannonID		= 13,


};



class CLab;
class CCamp;
class CProtocol;
class CBuilder;
class CPopStation;
class CPlayerWarReport;

class CPlayer
{
public:
	CPlayer ( UINT32 dwConnID,int iAccount,std::string strName,int iRMB ,int iGold ,int iOil, 
			const std::vector<BuildingData> & rVecBuilding,const std::vector<StorageData> & rVecStoreData, __time64_t storetime,const LabData & labdata,
			const SoldierData& soldierData,const std::vector<CBuilderData>& vecBuilderData,const std::map<int,CampData>& mapCampData,
			const std::map<int,CPopStationData>& mapPopStationData,const std::vector<CPlayerWarReportData> &vPlayerWarReport,__time64_t ProtectTime);
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
	__time64_t	GetProtectTime();
	const std::vector<BuildingData> & GetBuildingDataVec();
	const std::vector<StorageData> & GetStorageDataVec();
	void		SetStorageDataVec(const std::vector<StorageData> &vecStorageData,int nLostPrecent,int nAccountID);
	void		GetLabData(LabData & data);
	void		GetSoldierData(SoldierData & data);
	void		GetCampDataToClient(std::map<int,CCampDataToClient> & mapUniID2CampDataToClient);
	void		GetCampData(std::map<int,CampData> & mapCampData);
	void		GetBuilderDataToClient(std::vector<CBuilderDataToClient> & vecBuilderDataToClient);
	void		GetBuilderData( std::vector<CBuilderData> & vecBuilderData);
	void		GetPopuStationData(std::map<int,CPopStationData> & mapPopStationData);
	void		GetWarReportData(std::vector<CPlayerWarReportData> & vPlayerWarReport);
	void		UpdateWarReportData(std::vector<CPlayerWarReportData> & vPlayerWarReport);
	void		SetWarReportTop(int iIndex,int iIsTop);

	int			GetBuildID(int iUniID);

	void		InitLabData(const LabData & labdata);
	void		InitBuilderData(const std::vector<CBuilderData>& vecBuilderData);
	void		InitCampData(const std::map<int,CampData>& mapCampData);
	void		InitPopuStationData(const std::map<int,CPopStationData>& mapPopStationData);
	void		InitWarReport(const std::vector<CPlayerWarReportData> &vPlayerWarReport);

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
	bool		FinishUpgradeBuildRMB(int iUniID,int iRMB);
	void		EraseBuild(int iUniID);
	void		MoveBuild(int iUniID,int iGridX,int iGridY);
	void		OnBuildAddSuccess(int iUniID,int iID);
	void		OnUpgradeSuccess(int iUniID);
	void		CollectBuildResource(int iUniID,int iGold,int iOil);

	bool		DecSearchGold();
	void		HarryResource(int iGold,int iOil);

	void		ReqOutputSoldier(int iUniID,int iSoldierID);
	void		ReqCreateSoldier(int iUniID,int iSoldierID);
	void		ReqCancelSoldier(int iUniID,int iSID);
	void		ReqUpgradeSoldier(int iSoldierID);
	bool		AddToSoldierList(int iSoldierID);

	void		OnPutSoldier(int iSID);
	void		OnDisconnected(std::string strName);

private:
	void 		RemoveFromSoldierList(int iSoldierID);
	
	void		_AddToPopStation(int iSoldierID,int iSoldierPopulation);
	void		_RemoveFromPopStation(int iSoldierID,int iSoldierPopulation);


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

	__time64_t  m_ProtectTime;

	std::vector<BuildingData>	m_oBuildings;
	std::vector<StorageData>	m_oStorageData;

	std::map<int,CBuilder*>		m_mapBuilder;
	std::map<int,CCamp*>		m_mapUniID2Camp;
	std::map<int,CPopStation*>  m_mapUniID2PopuStation;
	CLab*						m_lab;

	std::map<int,int>			m_mapSoldierList;
	int							m_iSoldierCurrentPopulation;
	CPlayerWarReport*			m_pPlayerWarReport;

};

#endif