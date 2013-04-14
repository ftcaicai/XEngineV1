#ifndef _Builder_H_
#define _Builder_H_

#include "../protocol/Coder_Encoder/pch.h"

#include <vector>
#include <map>

class CPlayer; 

class CBuilder
{
public:
	CBuilder (int iUniID ,CPlayer *pPlayer);
	~CBuilder();
	
	void	InitBuilderData(int iUniID,int iUniIDInLevelUp,int iIDInLevelUp,int iLevel,__time64_t timeEnd);

	void	ProcessBuilder();

	bool	ReqLevelUp(int iUniID, int iID,int iLevel);
	bool	ReqCancelLevelUp(int iUniID);

	int			GetUniID();
	int			GetUniIDInLevelUp();
	int			GetLevelInLevelUp();
	__time64_t	GetTimeEndCreating();



private:
	bool	IsInUpgrade();
	void	OnSuccessLevelUp();

	void	_AckBuildLevelUpBegin();
	void 	_AckBuildLevelUpEnd();

private:
	int			m_iUniID;
	CPlayer *	m_Player;

	int			m_iUniIDToUp;
	int			m_iIDToUp;
	int			m_iLevelToUp;

	__time64_t	m_TimeBegin; // 此数据并不存入数据库 因此并不准确 而程序中也未使用 仅仅是为以后可能的扩展留有接口
	__time64_t	m_TimeEnd;


};

#endif