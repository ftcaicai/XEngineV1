#ifndef __SAVE_INFO_H__
#define __SAVE_INFO_H__
#include "Info_Struct.h"
class CSaveInfo
{
public:
	CSaveInfo(int iType,int iLevel,int iMsg,const char * cMessage);
	CSaveInfo(int iErrorCode);
	CSaveInfo(const char * cMessage);

	//返回类型
	int GetType();					
	//返回消息
	int GetMsgCode();
	//返回等级
	int GetLevel();
	//返回系统的错误代码
	int GetErrorCode();
	//返回提示
	const char * GetMsg();
private:
	int			m_iType;
	int			m_iMsg;
	int			m_iLevel;
	int			m_iErrorCode;
	char		m_cMessage[MAXCHARSIZE];
};
#endif