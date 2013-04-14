#ifndef __DEBUG_MODULE_INFO_STRUCT_H__
#define __DEBUG_MODULE_INFO_STRUCT_H__
#include "System_Header.h"
#include "DebugModuleInterface.h"
typedef struct InfoList
{
	int		m_iType;
	int		m_iMsg;
	int		m_iLevel;
	char	m_cMessage[MAXCHARSIZE];
}stList;

extern stList g_stInfo[]; 
#endif