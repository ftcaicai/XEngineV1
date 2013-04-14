#include "DebugModule.h"
CDebugModule * CDebugModule::m_pHandle = NULL;
int			g_iDebugHandleCount;			/// 计数器
CDebugModule::CDebugModule()
{

}
CDebugModule::~CDebugModule()
{

}
CDebugModule * CDebugModule::GetDebugModule(void)
{
	if (m_pHandle == NULL)
	{
		m_pHandle = new CDebugModule;
		if (m_pHandle != NULL)
		{
			g_iDebugHandleCount = 0;
			return m_pHandle;
		}
		else
			return NULL;
	}
	else
	{
		g_iDebugHandleCount++;
		return m_pHandle;
	}

}
void CDebugModule::ReleaseDebugModule()
{
	if (m_pHandle != NULL && g_iDebugHandleCount == 0)
	{
		delete m_pHandle;
		m_pHandle = NULL;
	}
	else
	{
		g_iDebugHandleCount--;
	}
}
void CDebugModule::SetOn(void)
{	
	m_iInfoState = DS_ON;
#ifdef _WINDOWS
	AllocConsole(); 
	freopen("CONOUT$","w+t",stdout); 
	freopen("CONIN$","r+t",stdin); 
#endif

}
void CDebugModule::SetOff(void)
{
	m_iInfoState = DS_OFF;
#ifdef _WINDOWS
	FreeConsole();
#endif
}
int CDebugModule::GetState(void)
{
	return m_iInfoState;
}
void CDebugModule::AddMsgInfo(int iMsg)
{
	if (m_iInfoState == DS_ON)
	{
		m_vInfoList.push_back(new CSaveInfo(g_stInfo[iMsg].m_iType,g_stInfo[iMsg].m_iLevel,iMsg,g_stInfo[iMsg].m_cMessage));
	}
}
void CDebugModule::AddErrorInfo(int iErrorCode)
{
	if (m_iInfoState == DS_ON)
	{
		m_vInfoList.push_back(new CSaveInfo(iErrorCode));
	}
}
void CDebugModule::AddStringInfo(const char * cValue)
{
	if (m_iInfoState == DS_ON)
	{
		m_vInfoList.push_back(new CSaveInfo(cValue));
	}
}
int CDebugModule::GetInfoCount(void)
{
	if (m_iInfoState == DS_ON)
		return (int)m_vInfoList.size();
	else
		return 0;
}
int CDebugModule::GetType(int nNode)
{
	if (m_iInfoState == DS_ON)
	{
		if(nNode >= 0 && nNode < (int) m_vInfoList.size())
		{
			return m_vInfoList[nNode]->GetType();
		}
		else
			return -1;
	}
	else
		return -1;
	
}
int CDebugModule::GetMsgCode(int nNode)
{
	if (m_iInfoState == DS_ON)
	{
		if(nNode >= 0 && nNode <(int)m_vInfoList.size())
		{
			return m_vInfoList[nNode]->GetMsgCode();
		}
		else
			return -1;
	}
	else
		return -1;
}
int CDebugModule::GetLevel(int nNode)
{
	if (m_iInfoState == DS_ON)
	{
		if(nNode >= 0 && nNode <(int)m_vInfoList.size())
		{
			return m_vInfoList[nNode]->GetLevel();
		}
		else
			return -1;
	}
	else
		return -1;
}
int CDebugModule::GetErrorCode(int nNode)
{
	if (m_iInfoState == DS_ON)
	{
		if(nNode >= 0 && nNode <(int)m_vInfoList.size())
		{
			return m_vInfoList[nNode]->GetErrorCode();
		}
		else
			return -1;
	}
	else
		return -1;
}
const char * CDebugModule::GetMsg(int nNode)
{
	if (m_iInfoState == DS_ON)
	{
		if(nNode >= 0 && nNode <(int)m_vInfoList.size())
		{
			return m_vInfoList[nNode]->GetMsg();
		}
		else
			return NULL;
	}
	else
		return NULL;
}
bool CDebugModule::DelInfo(int nNode)
{
	if (m_iInfoState == DS_ON)
	{
		if(nNode >= 0 && nNode <(int)m_vInfoList.size())
		{
			delete m_vInfoList[nNode];
			m_vInfoList.erase(m_vInfoList.begin() + nNode);
			return true;
		}
		else
			return false;
	}
	else 
		return false;
}
void CDebugModule::ClearInfo(void)
{
	if (m_iInfoState == DS_ON)
	{
		for (int nCount = 0; nCount < (int)m_vInfoList.size(); ++nCount)
		{
			delete m_vInfoList[nCount];
		}
		m_vInfoList.clear();
	}
}
void CDebugModule::SaveInfo(const char * cSaveName)
{
	FILE *			fp = NULL;
	string			sMsg;
	fp = fopen(cSaveName,"w+");
	if ( fp != NULL)
	{
		for (int nCount = 0; nCount < (int)m_vInfoList.size(); ++nCount)
		{
			sMsg.clear();
			sMsg = this->GetMsg(nCount);
			fprintf(fp,"第%d条 : %s",nCount,sMsg.c_str());
			fprintf(fp,"\n");
		}
		fclose(fp);
	}
}
CDebugModuleInterface * CreateDebugHandle()
{
	CDebugModuleInterface * hHandle;
	hHandle = static_cast<CDebugModuleInterface *>(CDebugModule::GetDebugModule());
	if(hHandle != NULL)
	{
		return hHandle;
	}
	else
		return NULL;

}