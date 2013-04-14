#include "DebugModule.h"
CDebugModule * CDebugModule::m_pHandle = NULL;
CRITICAL_SECTION 									g_hDebugMutex;
CDebugModule::CDebugModule(void)
{
	m_iShowCount = 0;
	InitializeCriticalSection(&g_hDebugMutex);
}
CDebugModule::~CDebugModule(void)
{
	m_iShowCount = 0;
	if(m_pHandle->m_iLoop != 0)
	{
		CloseAutoDisplay();	
	}
	
	
}
CDebugModule * CDebugModule::GetDebugModule()
{
	if (m_pHandle == NULL)
	{
		m_pHandle = new CDebugModule;
		if (m_pHandle != NULL)
		{
			m_pHandle->m_iHandleCount = 0;
			return m_pHandle;
		}
		else
			return NULL;
	}
	else
	{
		m_pHandle->m_iHandleCount++;
		return m_pHandle;
	}

}
void CDebugModule::ReleaseDebugModule()
{
	if (m_pHandle != NULL && m_pHandle->m_iHandleCount == 0)
	{
		delete m_pHandle;
		m_pHandle = NULL;
	}
	else
	{
		m_pHandle->m_iHandleCount--;
	}
}
CDebugModule * CDebugModule::GetDebugHandle()
{
	if(m_pHandle)
		return m_pHandle;
	else
		return NULL;
}
void CDebugModule::SetOn(bool bIsWindow)
{	
	m_iInfoState = DS_ON;
	if(bIsWindow)
	{
		AllocConsole();
		freopen("CONOUT$","w+t",stdout); 
		freopen("CONOUT$","r+t",stdin); 
	}
	AutoDisplay();
}
void CDebugModule::SetOff(void)
{
	m_iInfoState = DS_OFF;
	CloseAutoDisplay();

}
int CDebugModule::GetState(void)
{
	return m_iInfoState;
}
void CDebugModule::AddMsgInfo(int iMsg)
{
	{
		CDebugCriticalSection			m_hSection;
		if (m_iInfoState == DS_ON)
		{
			m_vInfoList.push_back(new CSaveInfo(g_stInfo[iMsg].m_iType,g_stInfo[iMsg].m_iLevel,iMsg,g_stInfo[iMsg].m_cMessage));
		}
	}
}
void CDebugModule::AddErrorInfo(int iErrorCode)
{
	{
		CDebugCriticalSection			m_hSection;
		if (m_iInfoState == DS_ON)
		{
			m_vInfoList.push_back(new CSaveInfo(iErrorCode));
		}
	}
}
void CDebugModule::AddStringInfo(const char * cValue)
{
	{
		CDebugCriticalSection			m_hSection;
		if (m_iInfoState == DS_ON)
		{
			m_vInfoList.push_back(new CSaveInfo(cValue));
		}
	}
}
int CDebugModule::GetInfoCount(void)
{
	{
		CDebugCriticalSection			m_hSection;
		if (m_iInfoState == DS_ON)
			return (int)m_vInfoList.size();
		else
			return 0;
	}
}
int CDebugModule::GetType(int nNode)
{
	{
		CDebugCriticalSection			m_hSection;
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
	
}
int CDebugModule::GetMsgCode(int nNode)
{
	{
		CDebugCriticalSection			m_hSection;
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
}
int CDebugModule::GetLevel(int nNode)
{
	{
		CDebugCriticalSection			m_hSection;
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
}
int CDebugModule::GetErrorCode(int nNode)
{
	{
		CDebugCriticalSection			m_hSection;
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
}
const char * CDebugModule::GetMsg(int nNode)
{
	{
		CDebugCriticalSection			m_hSection;
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
}
bool CDebugModule::DelInfo(int nNode)
{
	{
		CDebugCriticalSection			m_hSection;
		if (m_iInfoState == DS_ON)
		{
			if(nNode >= 0 && nNode <(int)m_vInfoList.size())
			{
				delete m_vInfoList[nNode];
				m_vInfoList.erase(m_vInfoList.begin() + nNode);
				m_pHandle->m_iShowCount--;
				return true;
			}
			else
				return false;
		}
		else 
			return false;
	}
}
void CDebugModule::ClearInfo(void)
{
	{
		CDebugCriticalSection			m_hSection;
		if (m_iInfoState == DS_ON)
		{
			for (int nCount = 0; nCount < (int)m_vInfoList.size(); ++nCount)
			{
				delete m_vInfoList[nCount];
			}
			m_vInfoList.clear();
			m_pHandle->m_iShowCount = 0;
		}
	}
}
void CDebugModule::SaveInfo(const char * cSaveName)
{
	{
		CDebugCriticalSection			m_hSection;
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
}

void CDebugModule::AutoDisplay(void)
{
	m_pHandle->m_iLoop = 1;
	m_stDebugThread.psa.nLength = sizeof(m_stDebugThread.psa);											
	m_stDebugThread.psa.lpSecurityDescriptor = NULL;
	m_stDebugThread.psa.bInheritHandle = TRUE;
	m_stDebugThread.m_iSleep = 16;
	m_stDebugThread.m_handle=(HANDLE)_beginthreadex(&m_stDebugThread.psa,0,DebugAutoDisplay,this,0,&m_stDebugThread.m_uID);
	if (m_stDebugThread.m_handle!=NULL)
	{
		printf("启动DEBUG调试线程\n");
		StartDosCommandInput();
	}
}
unsigned _stdcall CDebugModule::DebugAutoDisplay(void * Param)
{
	CDebugModule * ST=reinterpret_cast<CDebugModule *>(Param);
	if (ST!=NULL)
	{
		ST->AutoDisplayLoop();
		return 0;
	}
	else
	{
		return 1;
	}
}
unsigned int CDebugModule::AutoDisplayLoop(void)
{
	m_pHandle->m_iShowCount = 0;
	while (m_pHandle->m_iLoop == 1)
	{
		{
			CDebugCriticalSection			m_hSection;
			if(m_pHandle->m_iShowCount < (int)m_vInfoList.size())
			{
				if(m_vInfoList[m_pHandle->m_iShowCount]->GetErrorCode() != -1)
				{
					printf("系统错误代码:%d\n",m_vInfoList[m_pHandle->m_iShowCount]->GetErrorCode());
				}
				printf("内核提示:%s\n",m_vInfoList[m_pHandle->m_iShowCount]->GetMsg());
				m_pHandle->m_iShowCount++;
			}
		}
		Sleep(m_pHandle->m_stDebugThread.m_iSleep);
	}
	return 0;
}
void CDebugModule::CloseAutoDisplay(void)
{
	int						iState;
	unsigned long			ExitCode;
	m_pHandle->m_iLoop = 0;
	//等待线程结束
	iState = WaitForSingleObject(m_stDebugThread.m_handle,5000);
	if (iState == WAIT_OBJECT_0)
	{
		//得到线程退出代码
		GetExitCodeThread(m_stDebugThread.m_handle,&ExitCode);
		if (ExitCode == 0)
		{
			printf("DEBUG调试线程已关闭\n");
			DeleteCriticalSection(&g_hDebugMutex);
			CloseDosCommandInput();
		}
	}
}
void CDebugModule::StartDosCommandInput()
{
	m_stInpuThread.psa.nLength = sizeof(m_stInpuThread.psa);											
	m_stInpuThread.psa.lpSecurityDescriptor = NULL;
	m_stInpuThread.psa.bInheritHandle = TRUE;
	m_stInpuThread.m_iSleep = 16;
	m_pHandle->m_iInputLoop = 1;
	m_stInpuThread.m_handle=(HANDLE)_beginthreadex(&m_stInpuThread.psa,0,ThreadInput,NULL,0,&m_stInpuThread.m_uID);
	if (m_stInpuThread.m_handle!=NULL)
	{
		printf("XCore输入线程启动\n");
		printf("输入Exit退出\n");
	}
	else
	{
		printf("XCore输入线程启动失败\n");
	}
	
}
string CDebugModule::GetDosCommand()
{
	string				sCommand;
	if((int)m_pHandle->m_sCommand.size() > 0)
	{
		sCommand = m_pHandle->m_sCommand;
		m_pHandle->m_sCommand.clear();
		return sCommand;
	}
	else
	{
		sCommand = "\0";
		return sCommand;
	}
}
bool CDebugModule::CloseDosCommandInput()
{
	int						iState;
	unsigned long			ExitCode;
	m_pHandle->m_iInputLoop = 0;
	//等待线程结束
	iState = WaitForSingleObject(m_stInpuThread.m_handle,5000);
	if (iState == WAIT_OBJECT_0)
	{
		//得到线程退出代码
		GetExitCodeThread(m_stInpuThread.m_handle,&ExitCode);
		if (ExitCode == 0)
		{
			printf("输入线程已关闭\n");
#ifdef _WINDOWS
			fclose(stdout);
			fclose(stdin);
			FreeConsole();
#endif
			return true;
		}
		else
		{
#ifdef _WINDOWS
			fclose(stdout);
			fclose(stdin);
			FreeConsole();
#endif
			return false;
		}
	}
	else
		return false;
}
unsigned int CDebugModule::ThreadInput(void * Param)
{
	char				cInput[256];
	while(m_pHandle->m_iInputLoop == 1)
	{
		memset(cInput,'\0',255);
		fgets(cInput,255,stdin);
		m_pHandle->m_sCommand.clear();
		m_pHandle->m_sCommand.append(cInput);
		//if(m_pHandle->m_sCommand.empty() == false)
			//printf("%s",m_pHandle->m_sCommand.c_str());
		Sleep(m_pHandle->m_stInpuThread.m_iSleep);
	}
	m_pHandle->m_sCommand.clear();
	m_pHandle->m_sCommand.append("Exit\n");
	return 0;


}
CDebugCriticalSection::CDebugCriticalSection(void)
{
	EnterCriticalSection(&g_hDebugMutex);
};
CDebugCriticalSection::~CDebugCriticalSection(void)
{
	LeaveCriticalSection(&g_hDebugMutex);
};
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
CDebugModuleInterface * GetDebugHandle()
{
	CDebugModuleInterface * hHandle;
	hHandle = static_cast<CDebugModuleInterface *>(CDebugModule::GetDebugHandle());
	if(hHandle != NULL)
	{
		return hHandle;
	}
	else
		return NULL;
}