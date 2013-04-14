#include "CMysql.h"

#include <sstream>
#include <atltime.h>
#include<vector>
#include<map>

//#include <ctime>
//#include <cstdlib>
//#include "windows.h"
//#include <Windows.h>
//#include <afx.h>

#include "../protocol/buildingdata.h"
#include "../PlayerMgr/PlayerMgr.h"
#include "../PlayerMgr/Player.h"
#include "../PlayerMgr/PlayerWarReport.h"

#include "../HarryMgr/HarryMgr.h"

///To save login player struct
typedef struct LoginPlayer
{
	int								iAccountID;
	std::string						strPassWd;
	std::string						strName;
	int								iRMB;
	int								iGold;
	int								iOil;
	std::string						strStorgeData;
	__time64_t						storeTime;
	std::vector<BuildingData>		vecBuildings;
	std::vector<StorageData>		vecStorageData;
	LabData							labdata;
	SoldierData						soldierData;
	std::vector<CBuilderData>		vecBuilderData;
	std::map<int,CampData>			mapCampData;
	std::map<int,CPopStationData>	mapPopStationData;
	__time64_t						ProtectTime;
}stLoginPlayer;
//To save war report struct
typedef struct WarReport
{
	int								iIndex;
	std::string						sSufferer_Name;
	int								iAttacker_ID;
	std::string						sAttacker_Name;
	int								iAttacker_Level;
	std::string						sAttacker_League;
	std::string						sAttacker_Soilders;
	int								iRob_Money;
	int								iRob_Oil;
	int								iAttacker_Exp;
	int								iSufferer_Exp;
	int								iIsWin;
	int								iIsTop;
	__time64_t						WarTime;
	int								iIsNewReport;

}stWarReport;
typedef struct SearchTarget
{
	int								iAccountID;
	std::string						strName;
	int								iTownHallLevel;
	bool							bIsOnline;
	bool							bIsProtect;
	__time64_t						ProtectTime;
	bool							bIsBeAttack;
	__time64_t						BeAttackTime;
}stSearchTarget;
//To search player by name
std::map<int,int>									g_mAccountData;
//To search player by account ID
std::map<std::string,int>							g_mAccount;
//To save all table login info
std::vector<stLoginPlayer *>						g_vLoginPlayer;

//To save all War Report
std::vector<stWarReport *>							g_vWarReport;
//To search War Report
std::map<std::string,std::vector<int>>				g_mWarReport;
//To save search Target
std::vector<stSearchTarget *>						g_vSearchTarget;
//To search Target by Level
std::map<int,std::vector<int>>						g_mSearchTargetByLevel;
//To search Target by Name
std::map<std::string,int>							g_mSearchTargetByName;
//To search Target by AccountID						
std::map<int,int>									g_mSearchTargetByAccount;
INSTANCE_SINGLETON(CMysql)



CMysql::CMysql()
{ 
}

CMysql::~CMysql()
{ 
}

bool CMysql::Init()
{
	res_set = NULL;
	field = NULL;

	//mysql_library_init(0,NULL,NULL);
	mysql_init(&m_sqlCon);
	//if(!mysql_real_connect(&m_sqlCon,"localhost","root","123456","lybns",3306,NULL,0))//192.168.3.10
	try {
		if(!mysql_real_connect(&m_sqlCon,"192.168.3.10","root","123456","lybns",3306,NULL,0))//192.168.3.10
		{
			printf("MYSQL CONNECT ERROR\n");
			return false;
		}
	}
	catch(...){ 
		printf("MYSQL CONNECT ERROR\n");
		return false;
	}
	printf("MYSQL CONNECT Success\n");


	std::stringstream ss;
	ss << "CREATE TABLE IF NOT EXISTS MyAccount (AccountID INTEGER UNSIGNED NOT NULL, Password VARCHAR(128) NOT NULL,Name VARCHAR(128) NOT NULL,RMB  INTEGER UNSIGNED NOT NULL, PRIMARY KEY (AccountID));";

	if ( mysql_query(&m_sqlCon, ss.str().c_str())  )
	{
		// if error accur  but now i do not understand
	}
	InitAccount();
	InitWarReport();


	return true;

}

void CMysql::UnInit()
{

}


bool CMysql::RegisterPlayer( std::string strPassWord,std::string strName)
{
	//check if player name already exist but now i can not think too much so just 	
	if( CheckPlayerNameExisted(strName) == true )
	{
		printf("Name conflict\n");
		return false;
	}

	int iAccountNum = GetAccountNum();
	if(iAccountNum <0)
	{
		printf(" CMysql::RegisterPlayer GetAccountNum Error\n");
		return false;
	}
	iAccountNum++;

	//1大厅 2 金矿 1油矿 1工人房 1炮塔
	std::vector<BuildingData>	m_oBuildings;
	BuildingData build0;
	build0.m_nUniID = 1;
	build0.m_nID = ETownID;
	build0.m_nLevel = 1;
	build0.m_nGridX = 0;
	build0.m_nGridY = 0;
	build0.m_bIsInLevelUp = false;

	BuildingData build1;
	build1.m_nUniID = 2;
	build1.m_nID = EGoldMine;
	build1.m_nLevel = 1;
	build1.m_nGridX = 3;
	build1.m_nGridY = 3;
	build1.m_bIsInLevelUp = false;

	BuildingData build2;
	build2.m_nUniID = 3;
	build2.m_nID = EOilMine;
	build2.m_nLevel = 1;
	build2.m_nGridX = 5;
	build2.m_nGridY = 5;
	build2.m_bIsInLevelUp = false;

	BuildingData build3;
	build3.m_nUniID = 4;
	build3.m_nID = EGoldMine;
	build3.m_nLevel = 1;
	build3.m_nGridX = 10;
	build3.m_nGridY = 0;
	build3.m_bIsInLevelUp = false;

	BuildingData build4;
	build4.m_nUniID = 5;
	build4.m_nID = EBuilderID;
	build4.m_nLevel = 1;
	build4.m_nGridX = 10;
	build4.m_nGridY = 10;
	build4.m_bIsInLevelUp = false;

	BuildingData build5;
	build5.m_nUniID = 6;
	build5.m_nID = ECannonID;
	build5.m_nLevel = 1;
	build5.m_nGridX = 30;
	build5.m_nGridY = 10;
	build5.m_bIsInLevelUp = false;

	BuildingData build6;
	build6.m_nUniID = 7;
	build6.m_nID = ECampID;
	build6.m_nLevel = 1;
	build6.m_nGridX = 30;
	build6.m_nGridY = 20;
	build6.m_bIsInLevelUp = false;

	BuildingData build7;
	build7.m_nUniID = 8;
	build7.m_nID = EPopStationID;
	build7.m_nLevel = 1;
	build7.m_nGridX = 30;
	build7.m_nGridY = 30;
	build7.m_bIsInLevelUp = false;

	m_oBuildings.push_back(build0);
	m_oBuildings.push_back(build1);
	m_oBuildings.push_back(build2);
	m_oBuildings.push_back(build3);
	m_oBuildings.push_back(build4);
	m_oBuildings.push_back(build5);
	m_oBuildings.push_back(build6);
	m_oBuildings.push_back(build7);

	ByteStream bsBuildVec;
	bsBuildVec << m_oBuildings;
	std::string strBuildVec;
	MySqlEscapeString(bsBuildVec,strBuildVec);

	//大厅中有1000块初始金币
	std::vector<StorageData>	m_oStorages;
	StorageData store;
	store.m_nUniID = 1;
	store.m_nGoldStore = 1000;
	store.m_nOilStore  = 1000;
	m_oStorages.push_back(store);

	ByteStream bsStore;
	bsStore << m_oStorages;
	std::string strStore;
	MySqlEscapeString(bsStore,strStore);

	//初始化labdata为无用
	LabData labdata;
	labdata.m_bUse  = 0;
	ByteStream bsLabdata;
	bsLabdata << labdata;
	std::string strLabData;
	MySqlEscapeString(bsLabdata,strLabData);

	//大厅中有100块初始金币
	std::vector<CBuilderData>	m_vecBuilderData;
	m_vecBuilderData.clear();
	ByteStream bsBuilderData;
	bsBuilderData << m_vecBuilderData;
	std::string strBuilderData;
	MySqlEscapeString(bsBuilderData,strBuilderData);

	//初始化空的soldier
	SoldierData sdata;
	ByteStream bsSoldierdata;
	bsSoldierdata << sdata;
	std::string strSoldierData;
	MySqlEscapeString(bsSoldierdata,strSoldierData);

	//初始化空的camp
	std::map<int,CampData> m_mapCampData;
	m_mapCampData.clear();
	ByteStream bsCampdata;
	bsCampdata << m_mapCampData;
	std::string strCampData;
	MySqlEscapeString(bsCampdata,strCampData);

	//初始化空的popS
	std::map<int,CPopStationData> m_mapPopData;
	m_mapPopData.clear();
	ByteStream bsPopdata;
	bsPopdata << m_mapPopData;
	std::string strPopData;
	MySqlEscapeString(bsPopdata,strPopData);

	int iRMBInit	= 0;
	int iGoldInit	= 1000;		// this gold is total value of store  not include gold in mine
	int iOilInit	= 1000;

	CTime tm;
	tm=CTime::GetCurrentTime();

	int iYear	= tm.GetYear();
	int iMonth	= tm.GetMonth();
	int iday	= tm.GetDay();
	int iHour	= tm.GetHour();
	int iMinute = tm.GetMinute();
	int iSecond	= tm.GetSecond();
	int iDayOfWeek = tm.GetDayOfWeek();
	__time64_t storeTime = tm.GetTime(); 
	//std::string str=tm.Format("现在时间是%Y年%m月%d日 %X");

	std::stringstream ss;
	ss << "insert into MyAccount(AccountID, Password, Name, RMB,Gold,Oil,StoreTime,BuildingInfo,LabData,BuilderData,SoldierData,CampData,PopData,StorageData) VALUES(" 
		<< iAccountNum << " , " 
		<< " '" << strPassWord.c_str() << "', " 
		<< " '" << strName.c_str()     << "', " 
		<< iRMBInit    << " , " 
		<< iGoldInit   << " , "
		<< iOilInit    << " , "
		<< storeTime    << " , "
		<< " '" << strBuildVec.c_str()		<< "' , "  
		<< " '" << strLabData.c_str()		<< "' , "  
		<< " '" << strBuilderData.c_str()	<< "' , "  
		<< " '" << strSoldierData.c_str()	<< "' , "  
		<< " '" << strCampData.c_str()		<< "' , "  
		<< " '" << strPopData.c_str()		<< "' , "  
		<< " '" << strStore.c_str()			<< "' "  
		<< "); " ; 

	//INSERT INTO `lybns`.`myaccount` (`AccountID`, `Password`, `Name`, `RMB`, `BuildingInfo`, `testStringInfo`, `Gold`, `Oil`) VALUES (1, '1', '1', 1, '1', '1', 1, 1);

	int iRet = mysql_query(&m_sqlCon, ss.str().c_str()) ;
	printf("test : %s\n", ss.str().c_str());


	if ( iRet == 0  )
	{
		printf(" RegisterPlayer success PlayerName = %s\n",strName.c_str() );
		return true;
	}	
	else
	{
		printf(" RegisterPlayer failed PlayerName = %s\n",strName.c_str() );
		return false;
	}

	return true;
}

void CMysql::MySqlEscapeString(ByteStream& bs,std::string & str)
{
	int iSize = bs.Size();
	str.clear();
	str.assign(bs.Data(),iSize);
	int ilength = str.length();
	char* pTemp = new char[ilength*2+1];
	mysql_real_escape_string(&m_sqlCon, pTemp, str.c_str(), ilength);
	str.assign(pTemp);
	delete pTemp;

}

bool CMysql::CheckPlayerNameExisted(std::string strName)
{
	std::stringstream ss;
	ss << "SELECT * FROM MyAccount WHERE Name='" <<strName.c_str() << "'"; 
	if ( mysql_query(&m_sqlCon, ss.str().c_str()) !=0 )
	{
		printf("mysql_query ERROR  CMysql::CheckPlayerNameExisted");
	}	

	if(res_set!=NULL)
	{	
		mysql_free_result(res_set);
		res_set = NULL;
	}

	res_set = mysql_store_result(&m_sqlCon); 

	if(res_set == NULL)
	{
		printf("CMysql Fatal Error res_set == NULL ");
		return false;
	}
	else
	{
		my_ulonglong rowNum = mysql_num_rows(res_set);
		if(rowNum > 0)
			return true;
		else
			return false;
	}

	


}


int  CMysql::GetAccountNum()
{
	std::stringstream ss;
	ss << "SELECT * FROM MyAccount" ; 
	if ( mysql_query(&m_sqlCon, ss.str().c_str()) !=0  )
	{
		printf("mysql_query ERROR  CMysql::GetAccountNum");
	}	

	if(res_set!=NULL)
	{	
		mysql_free_result(res_set);   
		res_set = NULL;
	}
	res_set = mysql_store_result(&m_sqlCon); 
	if(res_set == NULL)
	{
		printf("Fatal Error CMysql::GetAccountNum()  we Get Nothing");
		return -1;
	}

	my_ulonglong rowNum = mysql_num_rows(res_set);
	return (int)rowNum;
}

bool CMysql::LoginCheck( UINT32 dwConnID, std::string strPassWord,std::string strName)
{
	std::map<std::string,int>::iterator it = g_mAccount.find(strName);
	if(it != g_mAccount.end())
	{
		if(g_vLoginPlayer[it->second]->strPassWd == strPassWord)
		{
			
			CPlayerMgr::Instance()->OnLogin(dwConnID,g_vLoginPlayer[it->second]->iAccountID,g_vLoginPlayer[it->second]->strName,g_vLoginPlayer[it->second]->iRMB, g_vLoginPlayer[it->second]->iGold, 
				g_vLoginPlayer[it->second]->iOil , g_vLoginPlayer[it->second]->vecBuildings,g_vLoginPlayer[it->second]->vecStorageData,g_vLoginPlayer[it->second]->storeTime,g_vLoginPlayer[it->second]->labdata,
				g_vLoginPlayer[it->second]->soldierData,g_vLoginPlayer[it->second]->vecBuilderData,g_vLoginPlayer[it->second]->mapCampData,g_vLoginPlayer[it->second]->mapPopStationData,GetWarReport(strName),g_vLoginPlayer[it->second]->ProtectTime);

				return SetPlayerOnlineState(strName,true);
		}
		else
		{
			//printf("CMysql::LoginCheck Fatal Error rowNum == %d",rowNum);
			return false;
		}
	}
	else
	{
		return UpdateAccount(dwConnID,strPassWord,strName);
	}
}
std::vector<CPlayerWarReportData> CMysql::GetWarReport(std::string strName)
{
	CPlayerWarReportData 							pPlayerWarReportData;
	std::vector<CPlayerWarReportData>				vPlayerWarReportData;
	std::stringstream								ss;
	vPlayerWarReportData.clear();
	std::map<std::string,std::vector<int>>::iterator it = g_mWarReport.find(strName);
	if(it != g_mWarReport.end())
	{
		for(int nCount = 0; nCount < (int)it->second.size(); ++nCount)
		{
			pPlayerWarReportData.iIndex = g_vWarReport[it->second[nCount]]->iIndex;
			pPlayerWarReportData.sSufferer_Name = g_vWarReport[it->second[nCount]]->sSufferer_Name;
			pPlayerWarReportData.iAttacker_ID = g_vWarReport[it->second[nCount]]->iAttacker_ID;
			pPlayerWarReportData.sAttacker_Name = g_vWarReport[it->second[nCount]]->sAttacker_Name;
			pPlayerWarReportData.iAttacker_Level = g_vWarReport[it->second[nCount]]->iAttacker_Level;
			pPlayerWarReportData.sAttacker_League = g_vWarReport[it->second[nCount]]->sAttacker_League;
			pPlayerWarReportData.sAttacker_Soilders = g_vWarReport[it->second[nCount]]->sAttacker_Soilders;
			pPlayerWarReportData.iRob_Money = g_vWarReport[it->second[nCount]]->iRob_Money;
			pPlayerWarReportData.iRob_Oil = g_vWarReport[it->second[nCount]]->iRob_Oil;
			pPlayerWarReportData.iAttacker_Exp = g_vWarReport[it->second[nCount]]->iAttacker_Exp;
			pPlayerWarReportData.iSufferer_Exp = g_vWarReport[it->second[nCount]]->iSufferer_Exp;
			pPlayerWarReportData.iIsWin = g_vWarReport[it->second[nCount]]->iIsWin;
			pPlayerWarReportData.iIsTop = g_vWarReport[it->second[nCount]]->iIsTop;
			pPlayerWarReportData.WarTime = g_vWarReport[it->second[nCount]]->WarTime;
			pPlayerWarReportData.iIsNewReport = g_vWarReport[it->second[nCount]]->iIsNewReport;
			ss.rdbuf()->str("");
			ss << "update warreport set "<<"NewReport ="<<0<< " where index_ID ="<< g_vWarReport[it->second[nCount]]->iIndex<<";"; 
			int iRet = mysql_query(&m_sqlCon, ss.str().c_str()) ;
			printf("test : %s\n", ss.str().c_str());

			if ( iRet == 0  )
			{
				printf(" Update War Report new report flag success PlayerName = %s\n",g_vWarReport[it->second[nCount]]->sSufferer_Name.c_str() );
				vPlayerWarReportData.push_back(pPlayerWarReportData);
			}	
			else
			{
				printf("Update War Report new report flag failed PlayerName = %s\n",g_vWarReport[it->second[nCount]]->sSufferer_Name.c_str() );
			}
			
		}
		
		return vPlayerWarReportData;
	}
	else
		return vPlayerWarReportData;
}
bool CMysql::UpdateWarReport(std::vector<CPlayerWarReportData> vPlayerWarReport)
{
	std::stringstream								ss;
	std::string										sIndex;
	char											cTemp[10];
	std::string										sTemp;
	std::vector<int>								vWarRecord;
	std::map<std::string,std::vector<int>>::iterator it = g_mWarReport.find(vPlayerWarReport[0].sSufferer_Name);
	if(it != g_mWarReport.end())
	{
		//check the record size is more than 50
		if((int)it->second.size() == 50)
		{
			ss.clear();
			itoa(g_vWarReport[it->second[0]]->iIndex,cTemp,10);
			sIndex = cTemp;
			ss << "DELETE FROM warreport WHERE 'index='"<< sIndex.c_str() ; 
			int iRet = mysql_query(&m_sqlCon, ss.str().c_str()) ;
			printf("test : %s\n", ss.str().c_str());

			if ( iRet == 0  )
			{
				printf(" Delete War Report Record success PlayerName = %s\n",vPlayerWarReport[0].sSufferer_Name.c_str() );
			}	
			else
			{
				printf("Delete War Report Record faild PlayerName = %s\n",vPlayerWarReport[0].sSufferer_Name.c_str() );
				return false;
			}

			delete g_vWarReport[it->second[0]];
			g_vWarReport.erase(g_vWarReport.begin() + it->second[0]);
			it->second.erase(it->second.begin() + 0);


			ss.clear();
			CTime tm;
			tm=CTime::GetCurrentTime();

			int iYear	= tm.GetYear();
			int iMonth	= tm.GetMonth();
			int iday	= tm.GetDay();
			int iHour	= tm.GetHour();
			int iMinute = tm.GetMinute();
			int iSecond	= tm.GetSecond();
			int iDayOfWeek = tm.GetDayOfWeek();
			__time64_t storeTime = tm.GetTime(); 
			ss<<"INSERT INTO warreport (Sufferer_Name, Attacker_ID, Attacker_Name, Attacker_Level, Attacker_League, Attacker_Soilders, Rob_Money, Rob_Oil, Attacker_Exp, Sufferer_Exp, IsWin, IsTop, Time) VALUES ("
			<<"'"<< vPlayerWarReport[0].sSufferer_Name.c_str() << "' , "
			<< vPlayerWarReport[0].iAttacker_ID<< " , "
			<<"'"<< vPlayerWarReport[0].sAttacker_Name.c_str()<< "' , "
			<< vPlayerWarReport[0].iAttacker_Level<<" , "
			<<"'"<< vPlayerWarReport[0].sAttacker_League.c_str()<< "' , "
			<<"'"<< vPlayerWarReport[0].sAttacker_Soilders.c_str()<< "' , "
			<< vPlayerWarReport[0].iRob_Money<< " , "
			<< vPlayerWarReport[0].iRob_Oil<< " , "
			<< vPlayerWarReport[0].iAttacker_Exp<< " , "
			<< vPlayerWarReport[0].iSufferer_Exp<< " , "
			<< vPlayerWarReport[0].iIsWin<< " , "
			<< vPlayerWarReport[0].iIsTop<< " , "
			<< storeTime<< " );";

			iRet = mysql_query(&m_sqlCon, ss.str().c_str()) ;
			printf("test : %s\n", ss.str().c_str());


			if(AddWarReportRecord(vPlayerWarReport) == true)
			{
				it->second.push_back((int)g_vWarReport.size() - 1);
				return true;
			}
			else
				return false;
			
		}
		else
		{
			if(AddWarReportRecord(vPlayerWarReport) == true)
			{
				it->second.push_back((int)g_vWarReport.size() - 1);
				return true;
			}
			else
				return false;
					
		}
	}
	else
	{
		if(AddWarReportRecord(vPlayerWarReport) == true)
		{
			vWarRecord.push_back((int)g_vWarReport.size() - 1);
			g_mWarReport.insert(make_pair(vPlayerWarReport[0].sSufferer_Name,vWarRecord));
			return true;
		}
		else
			return false;
	}
}
bool CMysql::SetWarReportTop(int iIndex,int iIsTop)
{
	std::stringstream ss;
	ss<<"update warreport set Istop ="<<iIsTop<<" where index_ID ="<<iIndex;
	int iRet = mysql_query(&m_sqlCon, ss.str().c_str()) ;
	if ( iRet == 0  )
	{
		printf(" Set war report top success index = %d\n",iIndex );
		return true;
	}	
	else
	{
		printf("  Set war report top failed index = %d\n",iIndex );
		return false;
	}


}
bool CMysql::UpdateAccount(UINT32 dwConnID, std::string strPassWord,std::string strName)
{
	std::stringstream ss;
	ss << "SELECT * FROM MyAccount WHERE Name='" <<strName.c_str() << "' AND Password='" << strPassWord.c_str() <<"'"  ; 
	if ( mysql_query(&m_sqlCon, ss.str().c_str()) !=0  )
	{
		printf("mysql_query ERROR  CMysql::LoginCheck");
	}	
	if(res_set!=NULL)
	{	
		mysql_free_result(res_set); 
		res_set = NULL;
	}
	res_set = mysql_store_result(&m_sqlCon); 
	if(res_set == NULL)
	{
		printf("CMysql Fatal Error res_set == NULL ");
		return false;
	}
	else
	{
		my_ulonglong rowNum = mysql_num_rows(res_set);
		if(rowNum == 1)
		{
			row = mysql_fetch_row(res_set);
			if(row == NULL)
			{
				printf("CMysql::LoginCheck Fatal Error row == %NULL");
				return false;
			}
			else
			{
				stLoginPlayer *				pstLogin = new stLoginPlayer;
				if(pstLogin)
				{
					std::string strAccountID	=	row[0];
					pstLogin-> iAccountID		=	atoi(strAccountID.c_str());
					pstLogin->strPassWd			=	row[1];
					pstLogin->strName			=	row[2];
					std::string strRMB			=	row[3];
					pstLogin->iRMB				=	atoi(strRMB.c_str());
					std::string strGold			=	row[6];
					pstLogin->iGold				=	atoi(strGold.c_str());
					std::string strOil			=	row[7];
					pstLogin->iOil				=	atoi(strOil.c_str());
					std::string strStoreTime	=	row[9];
					pstLogin->storeTime			=	atol(strStoreTime.c_str());

					ByteStream bs;
					bs.Assign(row[4],1000);
					bs >> pstLogin->vecBuildings;

					bs.Assign(row[8],1000);
					bs >> pstLogin->vecStorageData;

					bs.Assign(row[10],1000);
					bs >> pstLogin->labdata;

					bs.Assign(row[11],1000);
					bs >> pstLogin->soldierData;

					bs.Assign(row[12],1000);
					bs >> pstLogin->vecBuilderData;

					bs.Assign(row[13],1000);
					bs>> pstLogin->mapCampData;

					bs.Assign(row[14],1000);
					bs>> pstLogin->mapPopStationData;

					std::string	strProtectTime = row[16];
					pstLogin->ProtectTime = atol(strProtectTime.c_str());

					g_vLoginPlayer.push_back(pstLogin);
					g_mAccount.insert(std::make_pair(g_vLoginPlayer[(int)g_vLoginPlayer.size() - 1]->strName,(int)g_vLoginPlayer.size() - 1));
					g_mAccountData.insert(std::make_pair(g_vLoginPlayer[(int)g_vLoginPlayer.size() - 1]->iAccountID,(int)g_vLoginPlayer.size() - 1));
					AddTownHallRecord(g_vLoginPlayer[(int)g_vLoginPlayer.size() - 1]->iAccountID,g_vLoginPlayer[(int)g_vLoginPlayer.size() - 1]->strName,g_vLoginPlayer[(int)g_vLoginPlayer.size() - 1]->vecBuildings[0].m_nLevel,
						false,false,false,g_vLoginPlayer[(int)g_vLoginPlayer.size() - 1]->ProtectTime,0);

					CPlayerMgr::Instance()->OnLogin(dwConnID,pstLogin->iAccountID,pstLogin->strName,pstLogin->iRMB, pstLogin->iGold, 
						pstLogin->iOil , pstLogin->vecBuildings,pstLogin->vecStorageData,pstLogin->storeTime,pstLogin->labdata,
						pstLogin->soldierData,pstLogin->vecBuilderData,pstLogin->mapCampData,pstLogin->mapPopStationData,GetWarReport(strName),pstLogin->ProtectTime);
					printf("Add New Account in table!!\n");
					return true;
				}
			}
		}
		else
		{
			printf("CMysql::LoginCheck Fatal Error rowNum == %d",rowNum);
			return false;
		}
	}
	return false;
}

bool CMysql::SavePlayer( int iAccount )
{
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByAccountID(iAccount);
	if(pPlayer == NULL)
	{
		return false;
	}
	
	std::string strPlayerName = pPlayer->GetName();
	int iGold = pPlayer->GetGold();
	int iOil  = pPlayer->GetOil();
	int iRMB  =	pPlayer->GetRMB();
	__time64_t  storeTime = pPlayer->GetStoreTime();

	ByteStream bsBuildVec;
	bsBuildVec << pPlayer->GetBuildingDataVec();
	std::string strBuildVec;
	MySqlEscapeString(bsBuildVec,strBuildVec);

	ByteStream bsStorageData;
	bsStorageData << pPlayer->GetStorageDataVec();
	std::string strStorageData;
	MySqlEscapeString(bsStorageData,strStorageData);

	ByteStream bsLabData;
	LabData labdata;
	pPlayer->GetLabData(labdata);
	bsLabData << labdata;
	std::string strLabData;
	MySqlEscapeString(bsLabData,strLabData);

	ByteStream bsSoldierData;
	SoldierData soldierdata;
	pPlayer->GetSoldierData(soldierdata);
	bsSoldierData << soldierdata;
	std::string strSoldierData;
	MySqlEscapeString(bsSoldierData,strSoldierData);

	ByteStream bsBuilderData;
	std::vector<CBuilderData> vecBuilderData;
	pPlayer->GetBuilderData(vecBuilderData);
	bsBuilderData << vecBuilderData;
	std::string strBuilderData;
	MySqlEscapeString(bsBuilderData,strBuilderData);

	ByteStream bsCampData;
	std::map<int,CampData> mapCampData;
	pPlayer->GetCampData(mapCampData);
	bsCampData << mapCampData;
	std::string strCampData;
	MySqlEscapeString(bsCampData,strCampData);

	ByteStream bsPopStationData;
	std::map<int,CPopStationData> mapPopStationData;
	pPlayer->GetPopuStationData(mapPopStationData);
	bsPopStationData << mapPopStationData;
	std::string strPopStationData;
	MySqlEscapeString(bsPopStationData,strPopStationData);

	std::map<int,int>::iterator it = g_mAccountData.find(iAccount);
	if(it != g_mAccountData.end())
	{
		g_vLoginPlayer[it->second]->iRMB = iRMB;
		g_vLoginPlayer[it->second]->iGold = iGold;
		g_vLoginPlayer[it->second]->iOil = iOil;
		g_vLoginPlayer[it->second]->storeTime = storeTime;
		g_vLoginPlayer[it->second]->vecBuildings = pPlayer->GetBuildingDataVec();
		g_vLoginPlayer[it->second]->vecStorageData = pPlayer->GetStorageDataVec();
		pPlayer->GetLabData(g_vLoginPlayer[it->second]->labdata);
		pPlayer->GetSoldierData(g_vLoginPlayer[it->second]->soldierData);
		pPlayer->GetBuilderData(g_vLoginPlayer[it->second]->vecBuilderData);
		pPlayer->GetCampData(g_vLoginPlayer[it->second]->mapCampData);
		pPlayer->GetPopuStationData(g_vLoginPlayer[it->second]->mapPopStationData);
	}
	std::stringstream ss;
	ss << "UPDATE myaccount SET " 
				<< " RMB="  << iRMB 
				<< " ,Gold=" << iGold 
				<< " ,Oil="	<< iOil
				<< " ,StoreTime="	<< storeTime
				<< " ,BuildingInfo='"	<< strBuildVec.c_str()		<< "'"
				<< " ,StorageData='"	<< strStorageData.c_str()	<< "'"
				<< " ,LabData='"		<< strLabData.c_str()		<< "'"
				<< " ,SoldierData='"	<< strSoldierData.c_str()	<< "'"
				<< " ,BuilderData='"	<< strBuilderData.c_str()	<< "'"
				<< " ,CampData='"		<< strCampData.c_str()		<< "'"
				<< " ,PopData='"		<< strPopStationData.c_str()<< "'"
				<< " WHERE AccountID = " << iAccount <<" ;"  ;   

	int iRet = mysql_query(&m_sqlCon, ss.str().c_str()) ;
	if ( iRet == 0  )
	{
		printf(" SavePlayer success PlayerName = %s\n",strPlayerName.c_str() );
		return true;
	}	
	else
	{
		printf(" SavePlayer failed PlayerName = %s\n",strPlayerName.c_str() );
		return false;
	}

	return true;
}

bool CMysql::SearchOpponent(UINT32 dwConnID)
{
	std::vector<BuildingData>				vBuilding;
	int										nTarget;
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer == NULL)
	{
		printf("Fatal Error   CHarryMgr::SearchOpponentForPlayer  But Can not Find Player connID = %u",dwConnID);
		return false;
	}

	int iAccountID = pPlayer->GetAccountID();
	vBuilding = pPlayer->GetBuildingDataVec();
	nTarget = FindTarget(vBuilding[0].m_nLevel);

	std::stringstream ss;
	ss << "SELECT * FROM MyAccount WHERE Name='" <<g_vSearchTarget[nTarget]->strName.c_str()<<"'"<<";"; 
	//ss << "SELECT * FROM MyAccount WHERE AccountID not in (" << iAccountID << ") ORDER BY RAND()  LIMIT 1"  ; 
			// this line will be modify because we search not only by rand
	if ( mysql_query(&m_sqlCon, ss.str().c_str()) !=0  )
	{
		printf("mysql_query ERROR  CMysql::LoginCheck");
	}	
	if(res_set!=NULL)
	{	
		mysql_free_result(res_set); 
		res_set = NULL;
	}
	res_set = mysql_store_result(&m_sqlCon); 
	if(res_set == NULL)
	{
		printf("CMysql Fatal Error res_set == NULL ");
		return false;
	}
	else
	{
		my_ulonglong rowNum = mysql_num_rows(res_set);
		if(rowNum == 1)
		{
			row = mysql_fetch_row(res_set);
			if(row == NULL)
			{
				printf("CMysql::SearchOpponent Fatal Error row == %NULL");
				return false;
			}
			else
			{
				std::string strAccountID = row[0];
				std::string strPassWd	= row[1];
				std::string strName		= row[2];
				std::string strRMB		= row[3];

				std::string strGold		= row[6];
				std::string strOil		= row[7];
				std::string strStorage  = row[8]; 

				std::string strBuildInfo = row[4];

				int iAccountID	=  atoi(strAccountID.c_str());
				int iRMB		=  atoi(strRMB.c_str());
				int iGold		=  atoi(strGold.c_str());
				int iOil		=  atoi(strOil.c_str());

				ByteStream bs;
				std::vector<BuildingData>	vecBuildings;
				bs.Assign(row[4],1000);
				bs >> vecBuildings;

				std::vector<StorageData>	vecStorageDatas;// input store gold oil data to this list
				bs.Assign(row[8],1000);
				bs >> vecStorageDatas;

				//CPlayerMgr::Instance()->OnLogin(dwConnID,iAccountID,strName,iRMB, iGold, iOil , vecBuildings);
				CHarryMgr::Instance()->OnFindOpponent(dwConnID,iAccountID,strName,vecBuildings,vecStorageDatas);	
				
				return true;
			}
		}
		else
		{
			printf("CMysql::LoginCheck Fatal Error rowNum == %d",rowNum);
			return false;
		}
	}
	return true;	

	return true;
}
bool CMysql::SetPlayerOnlineState(std::string strName,bool bIsOnline)
{
	std::map<std::string,int>::iterator it = g_mSearchTargetByName.find(strName);
	if(it != g_mSearchTargetByName.end())
	{
		g_vSearchTarget[it->second]->bIsOnline = bIsOnline;
		return true;
	}
	else
		return false;
}
bool CMysql::GetPlayerOnlineState(std::string strName)
{
	std::map<std::string,int>::iterator it = g_mSearchTargetByName.find(strName);
	if(it != g_mSearchTargetByName.end())
	{
		return g_vSearchTarget[it->second]->bIsOnline;
	}
	else
		return false;
}
bool CMysql::SetPlayerProtectTime(int iAccountID,__time64_t ProtectTime)
{
	std::map<int,int>::iterator it = g_mSearchTargetByAccount.find(iAccountID);
	if(it != g_mSearchTargetByAccount.end())
	{
		g_vSearchTarget[it->second]->bIsProtect = true;
		g_vSearchTarget[it->second]->ProtectTime = ProtectTime;
		g_vSearchTarget[it->second]->bIsBeAttack = false;
		g_vSearchTarget[it->second]->BeAttackTime = 0;
		return UpdatePlayerProtectTimeToDB(iAccountID,ProtectTime);
		
	}
	else
		return false;
}
__time64_t CMysql::GetPlyaerProtectTime(std::string strName)
{
	std::map<std::string,int>::iterator it = g_mSearchTargetByName.find(strName);
	if(it != g_mSearchTargetByName.end())
	{
		return g_vSearchTarget[it->second]->ProtectTime;
	}
	else
		return false;
}
bool CMysql::InitAccount(void)
{
	std::stringstream ss;
	ss << "SELECT * FROM MyAccount "; 
	if ( mysql_query(&m_sqlCon, ss.str().c_str()) !=0  )
	{
		printf("mysql_query ERROR  CMysql::LoginCheck");
	}	
	if(res_set!=NULL)
	{	
		mysql_free_result(res_set); 
		res_set = NULL;
	}
	res_set = mysql_store_result(&m_sqlCon); 
	if(res_set == NULL)
	{
		printf("CMysql Fatal Error res_set == NULL ");
		return false;
	}
	else
	{

		my_ulonglong rowNum = mysql_num_rows(res_set);
		for(int nCount = 0; nCount < rowNum; ++nCount)
		{
			row = mysql_fetch_row(res_set);
			stLoginPlayer *				pstLogin = new stLoginPlayer;
			if(pstLogin)
			{
				std::string strAccountID	=	row[0];
				pstLogin-> iAccountID		=	atoi(strAccountID.c_str());
				pstLogin->strPassWd			=	row[1];
				pstLogin->strName			=	row[2];
				std::string strRMB			=	row[3];
				pstLogin->iRMB				=	atoi(strRMB.c_str());
				std::string strGold			=	row[6];
				pstLogin->iGold				=	atoi(strGold.c_str());
				std::string strOil			=	row[7];
				pstLogin->iOil				=	atoi(strOil.c_str());
				std::string strStoreTime	=	row[9];
				pstLogin->storeTime			=	atol(strStoreTime.c_str());

				ByteStream bs;
				bs.Assign(row[4],1000);
				bs >> pstLogin->vecBuildings;
				
				bs.Assign(row[8],1000);
				bs >> pstLogin->vecStorageData;

				bs.Assign(row[10],1000);
				bs >> pstLogin->labdata;

				bs.Assign(row[11],1000);
				bs >> pstLogin->soldierData;

				bs.Assign(row[12],1000);
				bs >> pstLogin->vecBuilderData;
				
				bs.Assign(row[13],1000);
				bs>> pstLogin->mapCampData;

				bs.Assign(row[14],1000);
				bs>> pstLogin->mapPopStationData;

				std::string strProtectTime = row[16];
				pstLogin->ProtectTime = atol(strProtectTime.c_str());


				g_vLoginPlayer.push_back(pstLogin);

			}
		}
		for(int nCount = 0; nCount < (int)g_vLoginPlayer.size(); ++nCount)
		{
			g_mAccountData.insert(std::make_pair(g_vLoginPlayer[nCount]->iAccountID,nCount));
			g_mAccount.insert(std::make_pair(g_vLoginPlayer[nCount]->strName,nCount));
			AddTownHallRecord(g_vLoginPlayer[nCount]->iAccountID,g_vLoginPlayer[nCount]->strName,g_vLoginPlayer[nCount]->vecBuildings[0].m_nLevel,false,false,false,g_vLoginPlayer[nCount]->ProtectTime,0);
			
		}
		printf("Load Login Account Complete\n");
		return true;
	}
}
bool CMysql::InitWarReport(void)
{
	std::string			sTemp;
	std::vector<int>	vWarRecord;
	std::stringstream ss;
	ByteStream bs;
	//ss<<"Select LAST_INSERT_ID();";
	ss << "SELECT * FROM warreport "; 
	if ( mysql_query(&m_sqlCon, ss.str().c_str()) !=0  )
	{
		printf("mysql_query ERROR  CMysql::LoginCheck");
	}	
	if(res_set!=NULL)
	{	
		mysql_free_result(res_set); 
		res_set = NULL;
	}
	res_set = mysql_store_result(&m_sqlCon); 
	if(res_set == NULL)
	{
		printf("CMysql Fatal Error res_set == NULL ");
		return false;
	}
	else
	{
		my_ulonglong rowNum = mysql_num_rows(res_set);
		for(int nCount = 0; nCount < rowNum; ++nCount)
		{
			row = mysql_fetch_row(res_set);
			stWarReport *					pWarReport = new stWarReport;
			if(pWarReport)
			{
				///Index;
				sTemp = row[0];
				pWarReport->iIndex = atoi(sTemp.c_str());
				///Sufferer_Name;
				pWarReport->sSufferer_Name = row[1];
				///Attacker_ID;
				sTemp = row[2];
				pWarReport->iAttacker_ID = atoi(sTemp.c_str());
				///Attacker_Name;
				pWarReport->sAttacker_Name = row[3];
				///Attacker_Level
				sTemp = row[4];
				pWarReport->iAttacker_Level = atoi(sTemp.c_str());
				///Attacker_league
				pWarReport->sAttacker_League = row[5];
				///Attacker_Soilders
				bs.Clear();
				bs.Assign(row[6],1000);
				bs >> pWarReport->sAttacker_Soilders;
				///Rob_Money
				sTemp = row[7];
				pWarReport->iRob_Money = atoi(sTemp.c_str());
				///Rob_oil
				sTemp = row[8];
				pWarReport->iRob_Oil = atoi(sTemp.c_str());
				///Attacker_Exp
				sTemp = row[9];
				pWarReport->iAttacker_Exp = atoi(sTemp.c_str());
				///Sufferer_Exp
				sTemp = row[10];
				pWarReport->iSufferer_Exp = atoi(sTemp.c_str());
				///IsWin
				sTemp = row[11];
				pWarReport->iIsWin = atoi(sTemp.c_str());
				///IsTop
				sTemp = row[12];
				pWarReport->iIsTop = atoi(sTemp.c_str());
				///Time
				sTemp = row[13];
				pWarReport->WarTime = atol(sTemp.c_str());
				///IsNewReport
				sTemp = row[14];
				pWarReport->iIsNewReport = atoi(sTemp.c_str());

				g_vWarReport.push_back(pWarReport);

			}
		}
		for(int nCount = 0; nCount < (int)g_vWarReport.size(); ++ nCount)
		{
			std::map<std::string,std::vector<int>>::iterator it = g_mWarReport.find(g_vWarReport[nCount]->sSufferer_Name);
			if(it != g_mWarReport.end())
			{
				it->second.push_back(nCount);
			}
			else
			{
				vWarRecord.push_back(nCount);
				g_mWarReport.insert(std::make_pair(g_vWarReport[nCount]->sSufferer_Name,vWarRecord));
				vWarRecord.clear();
			}
			
		}
		printf("Load War Report Complete\n");
		return true;
	}

}
bool CMysql::CreateDB()
{
	return false;
	//std::stringstream ss;
	//const ServerConfig& roCfg = CConfigure::Instance()->GetServerConfig();

	//ss << "CREATE DATABASE IF NOT EXISTS " << roCfg.m_strDataBase << " "
	//	<< "DEFAULT CHARACTER SET utf8;";

	//return m_poMysqlConn->Query(ss.str().c_str(), NULL);
}

bool CMysql::DropDB()
{
	return false;
	//const ServerConfig& roCfg = CConfigure::Instance()->GetServerConfig();

	//std::stringstream ss;
	//ss << "DROP DATABASE " << roCfg.m_strDataBase << ";";

	//return m_poMysqlConn->Query(ss.str().c_str(), NULL);
}

bool CMysql::CreateTables()
{
	return false;
	
	//std::stringstream ss;
	//ss << "CREATE TABLE IF NOT EXISTS Account (AccountID INTEGER UNSIGNED NOT NULL, Password VARCHAR(128) NOT NULL, PRIMARY KEY (AccountID));";

	//if(!m_poMysqlConn->Query(ss.str().c_str(), NULL))
	//{
	//	return false;
	//}

	//ss.str("");
	//ss << "SELECT count(AccountID) FROM Account;";
	//IMysqlRecordSet* poRet = NULL;
	//if(!m_poMysqlConn->QueryWithRecord(ss.str().c_str(), &poRet))
	//{
	//	return false;
	//}

	//poRet->FetchRow();

	//char* pVal = NULL;
	//UINT32 dwLen = 0;
	//UINT32 dwRow = 0;
	//poRet->GetFieldValue(0, &pVal, dwLen);
	//StringUtil::Str2Uint32(pVal, dwRow);

	//if(dwRow == 0)
	//{ 
	//	for(INT32 i = 1; i < 15; ++i)
	//	{
	//		ss.str("");
	//		ss << "INSERT INTO Account VALUES(" << (i+1) << ", 123);";
	//		m_poMysqlConn->Query(ss.str().c_str(), NULL);
	//	}
	//}

	//return true;
}
bool CMysql::AddWarReportRecord(std::vector<CPlayerWarReportData> vPlayerWarReport)
{
	stWarReport *									pWarReport;
	std::stringstream ss;
	std::stringstream sloading;
	std::string		  sTemp;
	ss.clear();
	CTime tm;
	tm=CTime::GetCurrentTime();

	int iYear	= tm.GetYear();
	int iMonth	= tm.GetMonth();
	int iday	= tm.GetDay();
	int iHour	= tm.GetHour();
	int iMinute = tm.GetMinute();
	int iSecond	= tm.GetSecond();
	int iDayOfWeek = tm.GetDayOfWeek();
	__time64_t storeTime = tm.GetTime(); 

	ByteStream bsSoilders;
	bsSoilders << vPlayerWarReport[0].sAttacker_Soilders;
	std::string strSoilders;
	MySqlEscapeString(bsSoilders,strSoilders);


	ss<<"INSERT INTO warreport (Sufferer_Name, Attacker_ID, Attacker_Name, Attacker_Level, Attacker_League, Attacker_Soilders, Rob_Money, Rob_Oil, Attacker_Exp, Sufferer_Exp, IsWin, IsTop, Time) VALUES ("
		<<"'"<< vPlayerWarReport[0].sSufferer_Name.c_str() << "' , "
		<< vPlayerWarReport[0].iAttacker_ID<< " , "
		<<"'"<< vPlayerWarReport[0].sAttacker_Name.c_str()<< "' , "
		<< vPlayerWarReport[0].iAttacker_Level<<" , "
		<<"'"<< vPlayerWarReport[0].sAttacker_League.c_str()<< "' , "
		<<"'"<< strSoilders.c_str()<< "' , "
		<< vPlayerWarReport[0].iRob_Money<< " , "
		<< vPlayerWarReport[0].iRob_Oil<< " , "
		<< vPlayerWarReport[0].iAttacker_Exp<< " , "
		<< vPlayerWarReport[0].iSufferer_Exp<< " , "
		<< vPlayerWarReport[0].iIsWin<< " , "
		<< vPlayerWarReport[0].iIsTop<< " , "
		<< storeTime<< " );";

	int iRet = mysql_query(&m_sqlCon, ss.str().c_str()) ;
	printf("test : %s\n", ss.str().c_str());


	if ( iRet == 0  )
	{
		printf(" Add War Report Record success PlayerName = %s\n",vPlayerWarReport[0].sSufferer_Name.c_str() );
		sloading.clear();
		sloading<<"select * from warreport order by index_ID DESC limit 1";
		if ( mysql_query(&m_sqlCon, sloading.str().c_str()) !=0  )
		{
			printf("mysql_query ERROR  CMysql::AddWarReportRecord");
			return false;
		}	
		if(res_set!=NULL)
		{	
			mysql_free_result(res_set); 
			res_set = NULL;
		}
		res_set = mysql_store_result(&m_sqlCon); 
		if(res_set == NULL)
		{
			printf("CMysql Fatal Error res_set == NULL ");
			return false;
		}
		else
		{
			my_ulonglong rowNum = mysql_num_rows(res_set);
			if(rowNum == 1)
			{
				row = mysql_fetch_row(res_set);
				if(row == NULL)
				{
					printf("CMysql::UpdateWarReport Fatal Error row == %NULL");
					return false;
				}
				else
				{
					stWarReport *					pWarReport = new stWarReport;
					if(pWarReport)
					{
						///Index;
						sTemp = row[0];
						pWarReport->iIndex = atoi(sTemp.c_str());
						///Sufferer_Name;
						pWarReport->sSufferer_Name = row[1];
						///Attacker_ID;
						sTemp = row[2];
						pWarReport->iAttacker_ID = atoi(sTemp.c_str());
						///Attacker_Name;
						pWarReport->sAttacker_Name = row[3];
						///Attacker_Level
						sTemp = row[4];
						pWarReport->iAttacker_Level = atoi(sTemp.c_str());
						///Attacker_league
						pWarReport->sAttacker_League = row[5];
						///Attacker_Soilders
						pWarReport->sAttacker_Soilders = row[6];
						///Rob_Money
						sTemp = row[7];
						pWarReport->iRob_Money = atoi(sTemp.c_str());
						///Rob_oil
						sTemp = row[8];
						pWarReport->iRob_Oil = atoi(sTemp.c_str());
						///Attacker_Exp
						sTemp = row[9];
						pWarReport->iAttacker_Exp = atoi(sTemp.c_str());
						///Sufferer_Exp
						sTemp = row[10];
						pWarReport->iSufferer_Exp = atoi(sTemp.c_str());
						///IsWin
						sTemp = row[11];
						pWarReport->iIsWin = atoi(sTemp.c_str());
						///IsTop
						sTemp = row[12];
						pWarReport->iIsTop = atoi(sTemp.c_str());
						///Time
						sTemp = row[13];
						pWarReport->WarTime = atol(sTemp.c_str());

						g_vWarReport.push_back(pWarReport);
					}
					
				}
				printf(" Add War Report Record success PlayerName = %s\n",vPlayerWarReport[0].sSufferer_Name.c_str() );
				return true;
			}
		}	

	} 
	else
	{
		printf(" Add War Report Record faild PlayerName = %s\n",vPlayerWarReport[0].sSufferer_Name );
		return false;
	}
}
void CMysql::AddTownHallRecord(int iAccountID,std::string	strName,int	iTownHallLevel,bool	bIsOnline,bool bIsProtect,bool bIsBeAttack,__time64_t ProtectTime,__time64_t BeAttackTime)
{
	stSearchTarget *					pSearchTarget =  new stSearchTarget;
	std::vector<int>					vSearchIndex;
	if(pSearchTarget)
	{
		vSearchIndex.clear();
		pSearchTarget->iAccountID = iAccountID;
		pSearchTarget->strName = strName;
		pSearchTarget->iTownHallLevel = iTownHallLevel;
		pSearchTarget->bIsOnline = bIsOnline;
		pSearchTarget->bIsProtect = bIsProtect;
		pSearchTarget->bIsBeAttack = bIsBeAttack;
		pSearchTarget->ProtectTime = ProtectTime;
		pSearchTarget->BeAttackTime =BeAttackTime;

		g_vSearchTarget.push_back(pSearchTarget);

		std::map<int,std::vector<int>>::iterator it = g_mSearchTargetByLevel.find(iTownHallLevel);
		if(it != g_mSearchTargetByLevel.end())
		{
			it->second.push_back((int)g_vSearchTarget.size() - 1);
		}
		else
		{
			vSearchIndex.push_back((int)g_vSearchTarget.size() - 1);
			g_mSearchTargetByLevel.insert(std::make_pair(iTownHallLevel,vSearchIndex));
		}

		g_mSearchTargetByName.insert(std::make_pair(strName,(int)g_vSearchTarget.size() - 1));
		g_mSearchTargetByAccount.insert(std::make_pair(iAccountID,(int)g_vSearchTarget.size() - 1));
		
	}
	
}
int CMysql::FindTarget(int iTownHallLevel)
{
	std::vector<int>					vTarget;
	std::vector<int>					vTargetOK;
	int									nTownHallLevel = iTownHallLevel;
	int									nUpTownHallLevel = nTownHallLevel;
	int									nDownTownHallLevel = nTownHallLevel;
	vTarget.clear();
	vTargetOK.clear();
	while(true)
	{
		std::map<int,std::vector<int>>::iterator it = g_mSearchTargetByLevel.find(nTownHallLevel);
		if(it != g_mSearchTargetByLevel.end())
		{
			vTarget = it->second;
			for(int nCount = 0; nCount < (int)vTarget.size(); ++nCount)
			{
				g_vSearchTarget[vTarget[nCount]]->bIsProtect = CheckProtectTime(g_vSearchTarget[vTarget[nCount]]->iAccountID,g_vSearchTarget[vTarget[nCount]]->ProtectTime);
				g_vSearchTarget[vTarget[nCount]]->bIsBeAttack = CheckBeAttackTime(g_vSearchTarget[vTarget[nCount]]->BeAttackTime);
				///找到等级相同的，并且符合条件的，存入
				if(g_vSearchTarget[vTarget[nCount]]->bIsOnline == false && g_vSearchTarget[vTarget[nCount]]->bIsProtect == false &&  g_vSearchTarget[vTarget[nCount]]->bIsBeAttack == false)
				{
					vTargetOK.push_back(vTarget[nCount]);
				}
			}
		}
		else
		{
			vTargetOK = SearchTarget(iTownHallLevel);
		}
		if(vTargetOK.empty() == false)
		{
			break;
		}
		else
		{
			vTargetOK = SearchTarget(iTownHallLevel);
		}
	}
	int nTarget = rand()%(int)vTargetOK.size();
	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime();
	g_vSearchTarget[vTargetOK[nTarget]]->BeAttackTime = now + 4 * 60;
	g_vSearchTarget[vTargetOK[nTarget]]->bIsBeAttack = true;
	return vTargetOK[nTarget];

	
}
std::vector<int> CMysql::SearchTarget(int iTownHallLevel)
{
	std::vector<int>					vTarget;
	std::vector<int>					vTargetOK;
	int									nTownHallLevel = iTownHallLevel;
	int									nUpTownHallLevel = nTownHallLevel;
	int									nDownTownHallLevel = nTownHallLevel;
	std::map<int,std::vector<int>>::iterator it;
	while(true)
	{
		nUpTownHallLevel = nUpTownHallLevel + 1;
		nDownTownHallLevel = nDownTownHallLevel - 1;
		if(nDownTownHallLevel <=0 )
		{
			nDownTownHallLevel = 0;
		}
		it = g_mSearchTargetByLevel.find(nUpTownHallLevel);
		if(it != g_mSearchTargetByLevel.end())
		{
			vTarget = it->second;
			for(int nCount = 0; nCount < (int)vTarget.size(); ++nCount)
			{
				g_vSearchTarget[vTarget[nCount]]->bIsProtect = CheckProtectTime(g_vSearchTarget[vTarget[nCount]]->iAccountID,g_vSearchTarget[vTarget[nCount]]->ProtectTime);
				g_vSearchTarget[vTarget[nCount]]->bIsBeAttack = CheckBeAttackTime(g_vSearchTarget[vTarget[nCount]]->BeAttackTime);
				if(g_vSearchTarget[vTarget[nCount]]->bIsOnline == false && g_vSearchTarget[vTarget[nCount]]->bIsProtect == false && g_vSearchTarget[vTarget[nCount]]->bIsBeAttack == false)
				{
					vTargetOK.push_back(vTarget[nCount]);
				}
			}
		}
		it = g_mSearchTargetByLevel.find(nDownTownHallLevel);
		if(it != g_mSearchTargetByLevel.end())
		{
			vTarget = it->second;
			for(int nCount = 0; nCount < (int)vTarget.size(); ++nCount)
			{
				g_vSearchTarget[vTarget[nCount]]->bIsProtect = CheckProtectTime(g_vSearchTarget[vTarget[nCount]]->iAccountID,g_vSearchTarget[vTarget[nCount]]->ProtectTime);
				g_vSearchTarget[vTarget[nCount]]->bIsBeAttack = CheckBeAttackTime(g_vSearchTarget[vTarget[nCount]]->BeAttackTime);
				if(g_vSearchTarget[vTarget[nCount]]->bIsOnline == false && g_vSearchTarget[vTarget[nCount]]->bIsProtect == false && g_vSearchTarget[vTarget[nCount]]->bIsBeAttack == false)
				{
					vTargetOK.push_back(vTarget[nCount]);
				}
			}
		}
		if(vTarget.empty() == false)
		{
			break;
		}
	}
	return vTargetOK;
}
bool CMysql::UpdatePlayerProtectTimeToDB(int iAccountID,__time64_t ProtectTime)
{
	std::stringstream ss;
	ss << "UPDATE myaccount SET " <<"ProtectTime="<<ProtectTime<< " WHERE AccountID = " << iAccountID <<" ;"  ;   

	int iRet = mysql_query(&m_sqlCon, ss.str().c_str()) ;
	if ( iRet == 0  )
	{
		printf(" Update protect time success PlayerID = %d\n",iAccountID );
		return true;
	}	
	else
	{
		printf(" Update protect time failed PlayerID = %d\n",iAccountID );
		return false;
	}
}
bool CMysql::CheckProtectTime(int iAccountID,__time64_t Time)
{
	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime();
	if(now > Time)
	{
		UpdatePlayerProtectTimeToDB(iAccountID,0);
		return false;
	}
	else
	{
		return true;
	}
}
bool CMysql::CheckBeAttackTime(__time64_t Time)
{
	CTime tm;
	tm=CTime::GetCurrentTime();
	__time64_t now = tm.GetTime();
	if(now > Time)
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CMysql::UpdateTargetData(int nAccountID,const std::vector<StorageData> &vecStorageData)
{
	int iGold = 0;
	int iOil = 0;
	std::map<int,int>::iterator it = g_mAccountData.find(nAccountID);
	if(it != g_mAccountData.end())
	{
		for(int nCount = 0; nCount < (int)g_vLoginPlayer[it->second]->vecBuildings.size(); ++nCount)
		{
			if(g_vLoginPlayer[it->second]->vecBuildings[nCount].m_nID == EGoldStore)	// 金库
			{
				for(int nCount1 = 0; nCount1 < (int)vecStorageData.size(); ++nCount1)
				{
					if(vecStorageData[nCount1].m_nUniID == g_vLoginPlayer[it->second]->vecBuildings[nCount].m_nUniID)
					{
						iGold = iGold + vecStorageData[nCount1].m_nGoldStore;
					}
				}

			}

			if(g_vLoginPlayer[it->second]->vecBuildings[nCount].m_nID == EOilStore)	// 油库
			{
				for(int nCount1 = 0; nCount1 < (int)vecStorageData.size(); ++nCount1)
				{
					if(vecStorageData[nCount1].m_nUniID == g_vLoginPlayer[it->second]->vecBuildings[nCount].m_nUniID)
					{
						iOil = iOil + vecStorageData[nCount1].m_nOilStore;
					}
				}
			}
		}
		g_vLoginPlayer[it->second]->iGold = iGold;
		g_vLoginPlayer[it->second]->iOil = iOil;
		g_vLoginPlayer[it->second]->vecStorageData = vecStorageData;

		ByteStream bsStorageData;
		bsStorageData << g_vLoginPlayer[it->second]->vecStorageData;
		std::string strStorageData;
		MySqlEscapeString(bsStorageData,strStorageData);
		std::stringstream ss;
		ss << "UPDATE myaccount SET " 
			<<"Gold="<<iGold
			<<",Oil="<<iOil
			<<",StorageData='" <<strStorageData.c_str()<<"'" 
			<< " WHERE AccountID = " << nAccountID <<" ;"  ;   

		int iRet = mysql_query(&m_sqlCon, ss.str().c_str()) ;
		if ( iRet == 0  )
		{
			printf(" Update target player success PlayerID = %d\n",nAccountID );
			printf(" Target Gold = %d\n",iGold);
			printf(" Target Oil = %d\n",iOil);
			return true;
		}	
		else
		{
			printf(" Update target player failed PlayerID = %d\n",nAccountID );
			printf(" Target Gold = %d\n",iGold);
			printf(" Target Oil = %d\n",iOil);
			return false;
		}
	}
}