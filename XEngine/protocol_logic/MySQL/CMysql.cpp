#include "CMysql.h"

#include <sstream>
#include <atltime.h>
//#include <ctime>
//#include <cstdlib>
//#include "windows.h"
//#include <Windows.h>
//#include <afx.h>

#include "../protocol/buildingdata.h"
#include "../PlayerMgr/PlayerMgr.h"
#include "../PlayerMgr/Player.h"

#include "../HarryMgr/HarryMgr.h"



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
	if(!mysql_real_connect(&m_sqlCon,"192.168.3.10","root","123456","lybns",3306,NULL,0))//192.168.3.10
	{
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

	//1大厅 2 金矿 1油矿
	std::vector<BuildingData>	m_oBuildings;
	BuildingData build0;
	build0.m_nUniID = 1;
	build0.m_nID = 1;
	build0.m_nLevel = 1;
	build0.m_nGridX = 0;
	build0.m_nGridY = 0;
	build0.m_bIsInLevelUp = false;

	BuildingData build1;
	build1.m_nUniID = 2;
	build1.m_nID = 2;
	build1.m_nLevel = 1;
	build1.m_nGridX = 3;
	build1.m_nGridY = 3;
	build1.m_bIsInLevelUp = false;

	BuildingData build2;
	build2.m_nUniID = 3;
	build2.m_nID = 3;
	build2.m_nLevel = 1;
	build2.m_nGridX = 5;
	build2.m_nGridY = 5;
	build2.m_bIsInLevelUp = false;

	BuildingData build3;
	build3.m_nUniID = 4;
	build3.m_nID = 2;
	build3.m_nLevel = 1;
	build3.m_nGridX = 10;
	build3.m_nGridY = 0;
	build3.m_bIsInLevelUp = false;

	BuildingData build4;
	build4.m_nUniID = 5;
	build4.m_nID = 11;
	build4.m_nLevel = 1;
	build4.m_nGridX = 10;
	build4.m_nGridY = 10;
	build4.m_bIsInLevelUp = false;

	m_oBuildings.push_back(build0);
	m_oBuildings.push_back(build1);
	m_oBuildings.push_back(build2);
	m_oBuildings.push_back(build3);
	m_oBuildings.push_back(build4);

	ByteStream bsBuildVec;
	bsBuildVec << m_oBuildings;
	std::string strBuildVec;
	MySqlEscapeString(bsBuildVec,strBuildVec);

	//大厅中有100块初始金币
	std::vector<StorageData>	m_oStorages;
	StorageData store;
	store.m_nUniID = 1;
	store.m_nGoldStore = 100;
	store.m_nOilStore  = 0;
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

	int iRMBInit	= 0;
	int iGoldInit	= 100;		// this gold is total value of store  not include gold in mine
	int iOilInit	= 0;

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
	ss << "insert into MyAccount(AccountID, Password, Name, RMB,Gold,Oil,StoreTime,BuildingInfo,LabData,BuilderData,SoldierData,CampData,StorageData) VALUES(" 
		<< iAccountNum << " , " 
		<< " '" << strPassWord << "', " 
		<< " '" << strName     << "', " 
		<< iRMBInit    << " , " 
		<< iGoldInit   << " , "
		<< iOilInit    << " , "
		<< storeTime    << " , "
		<< " '" << strBuildVec.c_str()		<< "' , "  
		<< " '" << strLabData.c_str()		<< "' , "  
		<< " '" << strBuilderData.c_str()	<< "' , "  
		<< " '" << strSoldierData.c_str()	<< "' , "  
		<< " '" << strCampData.c_str()		<< "' , "  
		<< " '" << strStore.c_str()			<< "' "  
		<< "); " ; 

	//INSERT INTO `lybns`.`myaccount` (`AccountID`, `Password`, `Name`, `RMB`, `BuildingInfo`, `testStringInfo`, `Gold`, `Oil`) VALUES (1, '1', '1', 1, '1', '1', 1, 1);

	int iRet = mysql_query(&m_sqlCon, ss.str().c_str()) ;
	//printf("test : %s", ss.str().c_str());

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
	ss << "SELECT * FROM MyAccount WHERE Name='" <<strName << "'"; 
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
	std::stringstream ss;
	ss << "SELECT * FROM MyAccount WHERE Name='" <<strName << "' AND Password='" << strPassWord <<"'"  ; 
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
				std::string strAccountID = row[0];
				std::string strPassWd	= row[1];
				std::string strName		= row[2];
				std::string strRMB		= row[3];

				std::string strGold		= row[6];
				std::string strOil		= row[7];
				std::string strStorgeData	= row[8];
				std::string strStoreTime	= row[9];
				std::string strLabData		= row[10];
				std::string strSoldierData  = row[11];
				std::string strBuilderData  = row[12];

				std::string strBuildInfo = row[4];


				//std::string strTestInfo = row[5];


				int iAccountID	=  atoi(strAccountID.c_str());
				int iRMB		=  atoi(strRMB.c_str());
				int iGold		=  atoi(strGold.c_str());
				int iOil		=  atoi(strOil.c_str());
				__time64_t storeTime = atol(strStoreTime.c_str());; 

				ByteStream bs;
				std::vector<BuildingData>	vecBuildings;
				bs.Assign(row[4],1000);
				bs >> vecBuildings;

				std::vector<StorageData>	vecStorageData;
				bs.Assign(row[8],1000);
				bs >> vecStorageData;

				LabData labdata;	
				bs.Assign(row[10],1000);
				bs >> labdata;

				SoldierData soldierData;
				bs.Assign(row[11],1000);
				bs >> soldierData;

				std::vector<CBuilderData>	vecBuilderData;
				bs.Assign(row[12],1000);
				bs >> vecBuilderData;

				std::map<int,CampData>		mapCampData;
				bs.Assign(row[13],1000);
				bs>> mapCampData;

				CPlayerMgr::Instance()->OnLogin(dwConnID,iAccountID,strName,iRMB, iGold, iOil , vecBuildings,vecStorageData,storeTime,labdata,soldierData,vecBuilderData,mapCampData);

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
	CPlayer *pPlayer = CPlayerMgr::Instance()->GetPlayerByConnID(dwConnID);
	if(pPlayer == NULL)
	{
		printf("Fatal Error   CHarryMgr::SearchOpponentForPlayer  But Can not Find Player connID = %u",dwConnID);
		return false;
	}

	int iAccountID = pPlayer->GetAccountID();

	std::stringstream ss;
	//ss << "SELECT * FROM MyAccount WHERE Name='" <<strName << "' AND Password='" << strPassWord <<"'"  ; 
	ss << "SELECT * FROM MyAccount WHERE AccountID not in (" << iAccountID << ") ORDER BY RAND()  LIMIT 1"  ; 
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