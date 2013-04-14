#include "Save_Info.h"
CSaveInfo::CSaveInfo(int iType,int iLevel,int iMsg,const char * cMessage)
{
	m_iType = iType;
	m_iMsg = iMsg;
	m_iLevel = iLevel;
	m_iErrorCode = -1;
	strcpy(m_cMessage,cMessage);
}
CSaveInfo::CSaveInfo(int iErrorCode)
{
	m_iType = -1;
	m_iMsg = -1;
	m_iLevel = -1;
	m_iErrorCode = iErrorCode;
	strcpy(m_cMessage,"\0");
}
CSaveInfo::CSaveInfo(const char * cMessage)
{
	m_iType = -1;
	m_iMsg = -1;
	m_iLevel = -1;
	m_iErrorCode = -1;
	strcpy(m_cMessage,cMessage);
}
int CSaveInfo::GetType()
{
	return m_iType;
}
int CSaveInfo::GetMsgCode()
{
	return m_iMsg;
}
int CSaveInfo::GetLevel()
{
	return m_iLevel;
}
const char * CSaveInfo::GetMsg()
{
	return m_cMessage;
}
int CSaveInfo::GetErrorCode()
{
	return m_iErrorCode;
}