#include "CsvReaderMgr.h"

INSTANCE_SINGLETON(CCsvReaderMgr)

#include <iostream>   
#include <string>   
#include <fstream>   


#include <sstream>
#include <vector>

CCsvReaderMgr::CCsvReaderMgr()
{ 
}

CCsvReaderMgr::~CCsvReaderMgr()
{ 
}

bool CCsvReaderMgr::Init()
{
	ReadCSVBuild();
	ReadCSVBuildLevel();
	ReadCSVTown();

	ReadSoldier();
	ReadSoldierLevelUp();
	return true;
}

void CCsvReaderMgr::UnInit()
{

}

void CCsvReaderMgr::ReadCSVBuild()
{
	std::ifstream file ( "CSV/building.csv" ); 
	std::string line = "";
	int iLineID = 0;
	//vector<string> all_words;
	while (getline(file, line))
	{
		iLineID++;
		if(iLineID>1)
		{
			int iID;
			SBuildData data;	//	m_strName;m_strType;m_iArea;m_iIsCanSell;m_iPricePercent;m_iMaxLevel;

			std::stringstream strstr(line);
			std::string word = "";
			int iColumnID = 0;
			while (getline(strstr,word, ',')) 
			{
				//all_words.push_back(word);
				iColumnID++;
				switch(iColumnID)
				{
				case 1:
					iID = atoi(word.c_str());
					break;

				case 2:
					data.m_strName = word;
					break;
				case 3:
					data.m_strType = word;
					break;
				case 4:
					data.m_iArea	= atoi(word.c_str());
					break;
				case 5:
					data.m_iIsCanSell = atoi(word.c_str());
					break;
				case 6:
					data.m_iPricePercent = atoi(word.c_str());
					break;
				case 7:
					data.m_iMaxLevel = atoi(word.c_str());
					break;
				case 8:
					//data.m_iMaxLevel = atoi(word.c_str());
					break;
				default:
					//debug 
					printf("error building.csv no this column colunmID = %d\n",iColumnID);
					break;	
				}
			}	
			m_mapBuildData[iID] = data;
		}
	}

}

void CCsvReaderMgr::ReadCSVBuildLevel()
{
	std::ifstream file ( "CSV/buildinglevel.csv" ); 
	std::string line = "";
	int iLineID = 0;
	//vector<string> all_words;
	while (getline(file, line))
	{
		iLineID++;
		if(iLineID>1)
		{
			int iID;
			int iLevel;
			SBuildLevelData data;	
			//1ID,2level,3Model,4Upgrade_model,5Death_model, 5
			//6Gold_cost,7Oil_cost,8Hp,9Building_time,10Search_gold,10
			//11out_gold,12out_oil,13out_med,14out_food, 14
			//15store_gold,16store_oil,17store_med,18soldier_number,19borrow,
			//20attack,21attack_type,22attack_target,23max_range,24min_range,skill_1,skill_2,script,icon


			//int		m_iStoreGold;
			//int		m_iStoreOil;
			//int		m_iStoreMed;

			std::stringstream strstr(line);
			std::string word = "";
			int iColumnID = 0;
			while (getline(strstr,word, ',')) 
			{
				//all_words.push_back(word);
				iColumnID++;
				switch(iColumnID)
				{
				case 1:
					iID = atoi(word.c_str());
					break;
				case 2:
					iLevel = atoi(word.c_str());
					break;
				case 3: // model 
					break;
				case 4:	// up model
					break;
				case 5: // death model
					break;
				case 6:	
					data.m_iGoldCost = atoi(word.c_str());
					break;
				case 7:
					data.m_iOilCost = atoi(word.c_str());
					break;
				case 8:	//hp
					break;
				case 9://building time
					data.m_iBuildingTime = atoi(word.c_str());
					break;
				case 10:// search gold
					break;
				case 11:
					data.m_iOutGold = atoi(word.c_str());
					break;
				case 12:
					data.m_iOutOil = atoi(word.c_str());
					break;
				case 13:
					data.m_iOutMed = atoi(word.c_str());
					break;
				case 14:
					data.m_iOutputPopulation = atoi(word.c_str());
					break;
				case 15:
					data.m_iStoreGold = atoi(word.c_str());
					break;
				case 16:
					data.m_iStoreOil = atoi(word.c_str());
					break;
				case 17:
					data.m_iStoreMed = atoi(word.c_str());
					break;
				case 18:
					data.m_iCampMaxPopulation = atoi(word.c_str());
					break;
				default:
					//debug 
					//printf("error no this column\n");
					break;	
				}
			}	

			std::map<int,SBuildData>::iterator it = m_mapBuildData.find(iID);
			if(it==m_mapBuildData.end())
			{
				//debug
				printf("Error no this build id = %u",iID);
			}
			m_mapBuildData[iID].InsertLevelData(iLevel,data);
		}
	}
}

void CCsvReaderMgr::ReadCSVTown()
{
	std::ifstream file ( "CSV/town.csv" ); 
	std::string line = "";
	int iLineID = 0;
	while (getline(file, line))
	{
		iLineID++;
		if(iLineID>1)
		{
			int iLevel;
			STownData data;	//	m_strName;m_strType;m_iArea;m_iIsCanSell;m_iPricePercent;m_iMaxLevel;

			std::stringstream strstr(line);
			std::string word = "";
			int iColumnID = 0;
			while (getline(strstr,word, ',')) 
			{
				//all_words.push_back(word);
				iColumnID++;
				switch(iColumnID)
				{
				case 1:
					iLevel = atoi(word.c_str());
					break;

				case 2:
					data.m_iSearchGold = atoi(word.c_str());
					break;
				default:
					//從3開始是townlimit 依次寫入vec
					data.m_vecBuildLimitCount.push_back(atoi(word.c_str()));
					break;	
				}
			}	
			m_mapTownData[iLevel] = data;
		}
	}

}

void CCsvReaderMgr::ReadSoldier()
{
	std::ifstream file ( "CSV/soldier.csv" ); 
	std::string line = "";
	int iLineID = 0;
	while (getline(file, line))
	{
		iLineID++;
		if(iLineID>1)
		{
			int iID;
			SSoldierData data;
			//int			m_iID;
			//std::string m_strName;
			//int			m_iMaxLevel;
			//int			m_iPopulation;
			//int			m_iTrainTime;
			//int			m_iGoldBase;
			//int			m_iOilBase;
			//int			m_iGoldGrowth;
			//int			m_iOilGrowth;	

			//1ID,2Name,3Level,4Attack,5Hp,6Speed,7Att_speed,8Range,9population, 
			//10Train_time,11Cost_gold,12Cost_oil,13Att_growth,14Hp_growth,15Gold_growth,16Oil_growth,
			//17Model,18Death_model,19Icon,20Tips,21Skill_cure,22Skill_att,23Move_type,24Focus_build,25Focus_times,26AI

			std::stringstream strstr(line);
			std::string word = "";
			int iColumnID = 0;
			while (getline(strstr,word, ',')) 
			{
				iColumnID++;
				switch(iColumnID)
				{
				case 1:
					iID = atoi(word.c_str());
					data.m_iID	=	iID;
					break;
				case 2:
					data.m_strName = word;
					break;
				case 3:
					data.m_iMaxLevel	= atoi(word.c_str());
					break;
				case 9:
					data.m_iPopulation	= atoi(word.c_str());
					break;
				case 10:
					data.m_iTrainTime	= atoi(word.c_str());
					break;
				case 11:
					data.m_iGoldBase	= atoi(word.c_str());
					break;
				case 12:
					data.m_iOilBase		= atoi(word.c_str());
					break;
				case 15:
					data.m_iGoldGrowth	= atoi(word.c_str());
					break;
				case 16:
					data.m_iOilGrowth	= atoi(word.c_str());
				default:
					//debug 
					//printf("error soldier.csv no this column colunmID = %d\n",iColumnID);
					break;	
				}
			}	
			m_mapSoldierData[iID] = data;
		}
	}
}

void CCsvReaderMgr::ReadSoldierLevelUp()
{
	std::ifstream file ( "CSV/soldierLevelUp.csv" ); 
	std::string line = "";
	int iLineID = 0;
	while (getline(file, line))
	{
		iLineID++;
		if(iLineID>1)
		{
			int iSoldierID;		
			int iLevel;
			SSoldierLevelUpCost costdata;
		
			std::stringstream strstr(line);
			std::string word = "";
			int iColumnID = 0;
			while (getline(strstr,word, ',')) 
			{
				iColumnID++;
				switch(iColumnID)
				{
				case 1:
					iSoldierID = atoi(word.c_str());
					break;

				case 2:
					iLevel = atoi(word.c_str());
					break;

				case 3:
					costdata.iOilCost = atoi(word.c_str());
					break;

				case 4:
					costdata.iTimeCost = atoi(word.c_str());
					break;
				}
			}	
			std::map<int,SSoldierLevelUpData>::iterator it = m_mapSoldierID2SoldierLevelUpData.find(iSoldierID);
			if(it==m_mapSoldierID2SoldierLevelUpData.end())
			{
				SSoldierLevelUpData data;
				data.m_mapLevel2CostData[iLevel] = costdata;
				m_mapSoldierID2SoldierLevelUpData[iSoldierID] = data;
			}
			else
			{
				it->second.m_mapLevel2CostData[iLevel] = costdata;
			}

		}
	}
}

void SBuildData::InsertLevelData(int iLevel, const SBuildLevelData & data )
{
	m_mapBuildLevelData[iLevel] = data;
}

int	 SBuildData::GetGoldCost(int iLevel)
{
	std::map<int,SBuildLevelData>::iterator it = m_mapBuildLevelData.find(iLevel);
	if(it == m_mapBuildLevelData.end())
		return 0;
	else
		return it->second.m_iGoldCost;

}

int  SBuildData::GetOilCost(int iLevel)
{
	std::map<int,SBuildLevelData>::iterator it = m_mapBuildLevelData.find(iLevel);
	if(it == m_mapBuildLevelData.end())
		return 0;
	else
		return it->second.m_iOilCost;
}

int SBuildData::GetBuildTime(int iLevel)
{
	std::map<int,SBuildLevelData>::iterator it = m_mapBuildLevelData.find(iLevel);
	if(it == m_mapBuildLevelData.end())
		return 0;
	else
		return it->second.m_iBuildingTime;
}

int SBuildData::GetGoldOutput(int iLevel)
{
	std::map<int,SBuildLevelData>::iterator it = m_mapBuildLevelData.find(iLevel);
	if(it == m_mapBuildLevelData.end())
		return 0;
	else
		return it->second.m_iOutGold;
}

int SBuildData::GetOilOutput(int iLevel)
{
	std::map<int,SBuildLevelData>::iterator it = m_mapBuildLevelData.find(iLevel);
	if(it == m_mapBuildLevelData.end())
		return 0;
	else
		return it->second.m_iOutOil;
}

int SBuildData::GetGoldStore(int iLevel)
{
	std::map<int,SBuildLevelData>::iterator it = m_mapBuildLevelData.find(iLevel);
	if(it == m_mapBuildLevelData.end())
		return 0;
	else
		return it->second.m_iStoreGold;	
}

int SBuildData::GetOilStore(int iLevel)
{
	std::map<int,SBuildLevelData>::iterator it = m_mapBuildLevelData.find(iLevel);
	if(it == m_mapBuildLevelData.end())
		return 0;
	else
		return it->second.m_iStoreOil;
}

int SBuildData::GetOutputPopulation(int iLevel)
{
	std::map<int,SBuildLevelData>::iterator it = m_mapBuildLevelData.find(iLevel);
	if(it == m_mapBuildLevelData.end())
		return 0;
	else
		return it->second.m_iOutputPopulation;
}

int	SBuildData::GetCampMaxPopulation(int iLevel)
{
	std::map<int,SBuildLevelData>::iterator it = m_mapBuildLevelData.find(iLevel);
	if(it == m_mapBuildLevelData.end())
		return 0;
	else
		return it->second.m_iCampMaxPopulation;	
}

bool CCsvReaderMgr::IsStorage(int iID)
{
	std::string strType = GetBuildType(iID);
	if(strType == "town" || strType == "resource" || strType == "store")
	{
		// need use other api like  compare?
		return true;
	}

	return false;
}

std::string CCsvReaderMgr::GetBuildType(int iID)
{
	std::map<int,SBuildData>::iterator it = m_mapBuildData.find(iID);
	if(it!=m_mapBuildData.end())
		return it->second.m_strType;

	return NULL;
}

int	 CCsvReaderMgr::GetMaxLevel(int iID)
{
	std::map<int,SBuildData>::iterator it = m_mapBuildData.find(iID);
	if(it!=m_mapBuildData.end())
		return it->second.m_iMaxLevel;

	return 0;
}

int	CCsvReaderMgr::GetGoldCost(int iID, int iLevel)
{
	std::map<int,SBuildData>::iterator it = m_mapBuildData.find(iID);
	if(it!=m_mapBuildData.end())
		return it->second.GetGoldCost(iLevel);

	return 0;
}

int CCsvReaderMgr::GetOilCost(int iID, int iLevel)
{
	std::map<int,SBuildData>::iterator it = m_mapBuildData.find(iID);
	if(it!=m_mapBuildData.end())
		return it->second.GetOilCost(iLevel);

	return 0;
}

int CCsvReaderMgr::GetBuildTime(int iID,int iLevel)
{
	std::map<int,SBuildData>::iterator it = m_mapBuildData.find(iID);
	if(it!=m_mapBuildData.end())
		return it->second.GetBuildTime(iLevel);

	return 0;
}

int CCsvReaderMgr::GetGoldOutput(int iID, int iLevel)
{
	std::map<int,SBuildData>::iterator it = m_mapBuildData.find(iID);
	if(it!=m_mapBuildData.end())
		return it->second.GetGoldOutput(iLevel);

	return 0;
}

int CCsvReaderMgr::GetOilOutput(int iID, int iLevel)
{
	std::map<int,SBuildData>::iterator it = m_mapBuildData.find(iID);
	if(it!=m_mapBuildData.end())
		return it->second.GetOilOutput(iLevel);

	return 0;

}


int CCsvReaderMgr::GetGoldStore(int iID, int iLevel)
{
	std::map<int,SBuildData>::iterator it = m_mapBuildData.find(iID);
	if(it!=m_mapBuildData.end())
		return it->second.GetGoldStore(iLevel);

	return 0;	
}

int CCsvReaderMgr::GetOilStore(int iID, int iLevel)
{
	std::map<int,SBuildData>::iterator it = m_mapBuildData.find(iID);
	if(it!=m_mapBuildData.end())
		return it->second.GetOilStore(iLevel);

	return 0;		
}

int  CCsvReaderMgr::GetPopulationOutPut(int iID, int iLevel)
{
	std::map<int,SBuildData>::iterator it = m_mapBuildData.find(iID);
	if(it!=m_mapBuildData.end())
		return it->second.GetOutputPopulation(iLevel);

	return 0;	
}

int CCsvReaderMgr::GetCampMaxPopulation(int iID, int iLevel)
{
	std::map<int,SBuildData>::iterator it = m_mapBuildData.find(iID);
	if(it!=m_mapBuildData.end())
		return it->second.GetCampMaxPopulation(iLevel);

	return 0;	
}

int	CCsvReaderMgr::GetSearchGold(int iTownLevel)
{
	std::map<int,STownData>::iterator it = m_mapTownData.find(iTownLevel);
	if(it!=m_mapTownData.end())
		return it->second.m_iSearchGold;

	return -1;		
}

int	CCsvReaderMgr::GetBuildLimitCount(int iTownLevel,int iID)
{
	std::map<int,STownData>::iterator it = m_mapTownData.find(iTownLevel);
	if(it!=m_mapTownData.end())
		return it->second.GetLimitCount(iID);

	return -1;		
}

int CCsvReaderMgr::GetSoldierUpgradeOilCost(int iSoldierID,int iLevel)
{
	std::map<int,SSoldierLevelUpData>::iterator it = m_mapSoldierID2SoldierLevelUpData.find(iSoldierID);
	if(it !=  m_mapSoldierID2SoldierLevelUpData.end())
	{
		return  it->second.GetUpgradeOilCost(iLevel);
	}
	return -1;
}

int CCsvReaderMgr::GetSoldierUpgradeTimeCost(int iSoldierID,int iLevel)
{
	std::map<int,SSoldierLevelUpData>::iterator it = m_mapSoldierID2SoldierLevelUpData.find(iSoldierID);
	if(it !=  m_mapSoldierID2SoldierLevelUpData.end())
	{
		return  it->second.GetUpgradeTimeCost(iLevel);
	}
	return -1;
}

int CCsvReaderMgr::GetSoldierPopulaiton(int iSoldierID)
{
	std::map<int,SSoldierData>::iterator it = m_mapSoldierData.find(iSoldierID);
	if(it !=  m_mapSoldierData.end())
	{
		return  it->second.m_iPopulation;
	}
	return -1;
}

int	CCsvReaderMgr::GetSoldierMaxLevel(int iSoldierID)
{
	std::map<int,SSoldierData>::iterator it = m_mapSoldierData.find(iSoldierID);
	if(it !=  m_mapSoldierData.end())
	{
		return  it->second.m_iMaxLevel;
	}
	return -1;
}

int CCsvReaderMgr::GetSoldierTrainTime(int iSoldierID)
{
	std::map<int,SSoldierData>::iterator it = m_mapSoldierData.find(iSoldierID);
	if(it !=  m_mapSoldierData.end())
	{
		return  it->second.m_iTrainTime;
	}
	return 0;
}

int CCsvReaderMgr::GetSoldierGoldCost(int iSoldierID,int iLevel)
{
	std::map<int,SSoldierData>::iterator it = m_mapSoldierData.find(iSoldierID);
	if(it !=  m_mapSoldierData.end())
	{
		int iGoldCostBase = it->second.m_iGoldBase;
		int iGoldCostGrowth = it->second.m_iGoldGrowth;
		int iGoldCost = iGoldCostBase+iGoldCostGrowth*(iLevel-1);
		return iGoldCost;
	}
	return -1;
}

int	CCsvReaderMgr::GetSoldierOilCost(int iSoldierID,int iLevel)
{
	std::map<int,SSoldierData>::iterator it = m_mapSoldierData.find(iSoldierID);
	if(it !=  m_mapSoldierData.end())
	{
		int iOilCostBase = it->second.m_iOilBase;
		int iOilCostGrowth = it->second.m_iOilGrowth;
		int iOilCost = iOilCostBase+iOilCostGrowth*(iLevel-1);
		return iOilCost;
	}
	return -1;
}


int SSoldierLevelUpData::GetUpgradeOilCost(int iLevel)
{
	std::map<int,SSoldierLevelUpCost>::iterator it = m_mapLevel2CostData.find(iLevel);
	if(it!= m_mapLevel2CostData.end() )
	{
		return it->second.iOilCost;
	}
	return -1;
}

int SSoldierLevelUpData::GetUpgradeTimeCost(int iLevel)
{
	std::map<int,SSoldierLevelUpCost>::iterator it = m_mapLevel2CostData.find(iLevel);
	if(it!= m_mapLevel2CostData.end() )
	{
		return it->second.iTimeCost;
	}
	return -1;
}

int STownData::GetLimitCount(int iID)
{
	if(iID > m_vecBuildLimitCount.size())
		return -1;
	else
		return m_vecBuildLimitCount[iID-1];
}