#ifndef _CsvReaderMgr_H__
#define _CsvReaderMgr_H__

#include "../protocol/Coder_Encoder/pch.h"
#include <map>
#include <string>

struct SBuildData;
struct SBuildLevelData;	
struct STownData;
struct SSoldierLevelUpData;
struct SSoldierLevelUpCost;
struct SSoldierData;

class CCsvReaderMgr
{
	CCsvReaderMgr();
	~CCsvReaderMgr();
	DECLARE_SINGLETON(CCsvReaderMgr)
public:
	bool Init();
	void UnInit();

public:
	bool IsStorage(int iID);
	std::string GetBuildType(int iID);

	int	 GetMaxLevel(int iID);
	int	 GetGoldCost(int iID, int iLevel);
	int  GetOilCost(int iID, int iLevel);
	int  GetBuildTime(int iID,int iLevel);
	int  GetGoldOutput(int iID, int iLevel);
	int  GetOilOutput(int iID, int iLevel);

	int  GetGoldStore(int iID, int iLevel);
	int  GetOilStore(int iID, int iLevel);

	int  GetPopulationOutPut(int iID, int iLevel);
	int  GetCampMaxPopulation(int iID, int iLevel);

	int	 GetSearchGold(int iTownLevel);
	int	 GetBuildLimitCount(int iTownLevel,int iID);

	int	 GetSoldierUpgradeOilCost(int iSoldierID,int iLevel);
	int  GetSoldierUpgradeTimeCost(int iSoldierID,int iLevel);

	int	 GetSoldierPopulaiton(int iSoldierID);
	int	 GetSoldierMaxLevel(int iSoldierID);
	int	 GetSoldierTrainTime(int iSoldierID);
	int	 GetSoldierGoldCost(int iSoldierID,int iLevel);
	int	 GetSoldierOilCost(int iSoldierID,int iLevel);

private:
	void ReadCSVBuild();
	void ReadCSVBuildLevel();
	void ReadCSVTown();
	void ReadSoldier();
	void ReadSoldierLevelUp();


	std::map<int,SBuildData> m_mapBuildData;

	std::map<int,STownData>  m_mapTownData;

	std::map<int,SSoldierLevelUpData> m_mapSoldierID2SoldierLevelUpData;

	std::map<int,SSoldierData>	m_mapSoldierData;


};

struct SBuildData
{
	std::string	m_strName;
	std::string	m_strType;
	int		m_iArea;
	int		m_iIsCanSell;
	int		m_iPricePercent;
	int		m_iMaxLevel;

	std::map<int,SBuildLevelData> m_mapBuildLevelData;

	void InsertLevelData(int iLevel, const SBuildLevelData & data );

	int	 GetGoldCost(int iLevel);
	int  GetOilCost(int iLevel);
	int  GetBuildTime(int iLevel);
	int  GetGoldOutput(int iLevel);
	int  GetOilOutput(int iLevel);
	int	 GetGoldStore(int iLevel);
	int	 GetOilStore(int iLevel);

	int  GetOutputPopulation(int iLevel);
	int	 GetCampMaxPopulation(int iLevel);

};

struct SBuildLevelData
{
	//ID,level,Model,Upgrade_model,Death_model,
	//Gold_cost,Oil_cost,Hp,Building_time,Search_gold,
	//out_gold,out_oil,out_med,out_food,
	//store_gold,store_oil,store_med,soldier_number,borrow,
	//attack,attack_type,attack_target,max_range,min_range,skill_1,skill_2,script,icon

	int		m_iGoldCost;
	int		m_iOilCost;
	int		m_iBuildingTime;
	int		m_iOutGold;
	int		m_iOutOil;
	int		m_iOutMed;
	int     m_iOutputPopulation;

	int		m_iStoreGold;
	int		m_iStoreOil;
	int		m_iStoreMed;

	int     m_iCampMaxPopulation;

};

struct STownData
{
	int			m_iSearchGold;
	std::vector<int> m_vecBuildLimitCount;

	int GetLimitCount(int iID);
};


struct SSoldierLevelUpData
{
	int GetUpgradeOilCost(int iLevel);
	int GetUpgradeTimeCost(int iLevel);

	std::map<int,SSoldierLevelUpCost> m_mapLevel2CostData;
};


struct SSoldierLevelUpCost
{
	int iOilCost;
	int iTimeCost;
};

struct SSoldierData 
{
	//1ID,2Name,3Level,4Attack,5Hp,6Speed,7Att_speed,8Range,9population, 
	//10Train_time,11Cost_gold,12Cost_oil,13Att_growth,14Hp_growth,15Gold_growth,16Oil_growth,
	//17Model,18Death_model,19Icon,20Tips,21Skill_cure,22Skill_att,23Move_type,24Focus_build,25Focus_times,26AI
	
	int			m_iID;
	std::string m_strName;
	int			m_iMaxLevel;
	int			m_iPopulation;
	int			m_iTrainTime;
	int			m_iGoldBase;
	int			m_iOilBase;
	int			m_iGoldGrowth;
	int			m_iOilGrowth;


};

#endif