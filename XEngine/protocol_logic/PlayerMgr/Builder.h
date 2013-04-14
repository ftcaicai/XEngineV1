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

	__time64_t	m_TimeBegin; // �����ݲ����������ݿ� ��˲���׼ȷ ��������Ҳδʹ�� ������Ϊ�Ժ���ܵ���չ���нӿ�
	__time64_t	m_TimeEnd;


};

#endif