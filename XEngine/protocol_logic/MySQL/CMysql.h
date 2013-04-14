#ifndef _MySql_H__
#define _MySql_H__

#include "../protocol/Coder_Encoder/pch.h"

#include "winsock.h"
#include "include/mysql.h"
//#pragma comment(lib, "libmysql.lib") 

class ByteStream;

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

	bool SavePlayer( int iAccount);

	bool SearchOpponent(UINT32 dwConnID);

private:
	bool CheckPlayerNameExisted(std::string m_strName);
	int  GetAccountNum();

	void MySqlEscapeString(ByteStream& bs,std::string & str);

private:
	bool CreateDB();
	bool DropDB();
	bool CreateTables();

private:
	MYSQL m_sqlCon;

	MYSQL_RES * res_set; 
	MYSQL_ROW row; 
	MYSQL_FIELD * field; 

};

#endif