#ifndef _MySql_H__
#define _MySql_H__


#include "../protocol/Coder_Encoder/pch.h"

#include "winsock.h"
#include "include/mysql.h"



//#pragma comment(lib, "libmysql.lib") 


class ByteStream;
class CPlayerWarReportData;
class StorageData;

class CMysql
{
	CMysql();
	~CMysql();
	DECLARE_SINGLETON(CMysql)
public:
	bool Init();
	void UnInit();

public:
	bool RegisterPlayer( std::string  strPassWord,std::string strName);
	bool LoginCheck(UINT32 dwConnID, std::string strPassWord,std::string strName);
	std::vector<CPlayerWarReportData> GetWarReport(std::string strName);
	bool UpdateWarReport(std::vector<CPlayerWarReportData> vPlayerWarReport);
	bool SetWarReportTop(int iIndex,int iIsTop);

	bool SavePlayer( int iAccount);

	bool SearchOpponent(UINT32 dwConnID);

	bool SetPlayerOnlineState(std::string strName,bool bIsOnline);
	bool GetPlayerOnlineState(std::string strName);
	bool SetPlayerProtectTime(int iAccountID,__time64_t ProtectTime);
	__time64_t GetPlyaerProtectTime(std::string strName);
	bool UpdateTargetData(int nAccountID,const std::vector<StorageData> &vecStorageData);
	

private:
	bool CheckPlayerNameExisted(std::string m_strName);
	int  GetAccountNum();

	void MySqlEscapeString(ByteStream& bs,std::string & str);

private:
	bool InitAccount(void);
	bool UpdateAccount(UINT32 dwConnID, std::string strPassWord,std::string strName);
	bool InitWarReport(void);
	bool CreateDB();
	bool DropDB();
	bool CreateTables();
	bool AddWarReportRecord(std::vector<CPlayerWarReportData> vPlayerWarReport);
	void AddTownHallRecord(int iAccountID,std::string	strName,int	iTownHallLevel,bool	bIsOnline,bool bIsProtect,bool bIsBeAttack,__time64_t ProtectTime,__time64_t BeAttackTime);
	int FindTarget(int iTownHallLevel);
	std::vector<int> SearchTarget(int iTownHallLevel);
	bool UpdatePlayerProtectTimeToDB(int iAccountID,__time64_t ProtectTime);
	bool CheckProtectTime(int iAccountID,__time64_t Time);
	bool CheckBeAttackTime(__time64_t Time);
	


private:
	MYSQL m_sqlCon;

	MYSQL_RES * res_set; 
	MYSQL_ROW row; 
	MYSQL_FIELD * field;

	



	

};

#endif