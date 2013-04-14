#ifndef __SAVE_INFO_H__
#define __SAVE_INFO_H__
#include "Info_Struct.h"
class CSaveInfo
{
public:
	CSaveInfo(int iType,int iLevel,int iMsg,const char * cMessage);
	CSaveInfo(int iErrorCode);
	CSaveInfo(const char * cMessage);

	//��������
	int GetType();					
	//������Ϣ
	int GetMsgCode();
	//���صȼ�
	int GetLevel();
	//����ϵͳ�Ĵ������
	int GetErrorCode();
	//������ʾ
	const char * GetMsg();
private:
	int			m_iType;
	int			m_iMsg;
	int			m_iLevel;
	int			m_iErrorCode;
	char		m_cMessage[MAXCHARSIZE];
};
#endif