#include "ExportModule.h"
CExportModule * CExportModule::m_pHandle = NULL;
int			g_iExportHandleCount;			/// ¼ÆÊýÆ÷
CExportModule::CExportModule()
{
	m_hDebugHandle = NULL;
	m_hDebugHandle = CreateDebugHandle();

	m_hODBCHandle = NULL;
	

	m_hTcpHandle = NULL;
	
}
CExportModule::~CExportModule()
{
	if(m_hDebugHandle != NULL)
	{
		m_hDebugHandle->ReleaseDebugModule();
	}

	if(m_hODBCHandle != NULL)
	{
		m_hODBCHandle->ReleaseODBCHandle();
	}

	if(m_hTcpHandle != NULL)
	{
		m_hTcpHandle->ReleaseTcpModuleFrameworkHandle();
	}
}

CExportModule * CExportModule::GetExportModule()
{
	if (m_pHandle == NULL)
	{
		m_pHandle = new CExportModule;
		if (m_pHandle != NULL)
		{
			g_iExportHandleCount = 0;
			return m_pHandle;
		}
		else
			return NULL;
	}
	else
	{
		g_iExportHandleCount++;
		return m_pHandle;
	}
}
void CExportModule::ReleaseExportModule()
{
	if (m_pHandle != NULL && g_iExportHandleCount == 0)
	{
		delete m_pHandle;
		m_pHandle = NULL;
	}
	else
	{
		g_iExportHandleCount--;
	}
}
void CExportModule::SetDebugOn(bool bIsWindow)
{
	if(m_hDebugHandle != NULL)
	{
		m_hDebugHandle->SetOn(bIsWindow);
	}
}

void CExportModule::SetDebugOff(void)
{
	if(m_hDebugHandle != NULL)
	{
		m_hDebugHandle->SetOff();
	}
}
int CExportModule::GetDebugState(void)
{
	if(m_hDebugHandle != NULL)
	{
		return m_hDebugHandle->GetState();
	}
	else
		return -1;
}
void CExportModule::AddDebugMsgInfo(int iMsg)
{
	if(m_hDebugHandle != NULL)
	{
		m_hDebugHandle->AddMsgInfo(iMsg);
	}
}
void CExportModule::AddDebugErrorInfo(int iErrorCode)
{
	if(m_hDebugHandle != NULL)
	{
		m_hDebugHandle->AddErrorInfo(iErrorCode);
	}
}
void CExportModule::AddDebugStringInfo(const char * cValue)
{
	if(m_hDebugHandle != NULL)
	{
		m_hDebugHandle->AddStringInfo(cValue);
	}
}
int CExportModule::GetDebugInfoCount(void)
{
	if(m_hDebugHandle != NULL)
	{
		return m_hDebugHandle->GetInfoCount();
	}
	else
		return -1;
}
int CExportModule::GetDebugType(int nNode)
{
	if(m_hDebugHandle != NULL)
	{
		return m_hDebugHandle->GetType(nNode);
	}
	else
		return -1;
}
int CExportModule::GetDebugMsgCode(int nNode)
{
	if(m_hDebugHandle != NULL)
	{
		return m_hDebugHandle->GetMsgCode(nNode);
	}
	else
		return -1;
}
int CExportModule::GetDebugLevel(int nNode)
{
	if(m_hDebugHandle != NULL)
	{
		return m_hDebugHandle->GetLevel(nNode);
	}
	else
		return -1;
}
int CExportModule::GetDebugErrorCode(int nNode)
{
	if(m_hDebugHandle != NULL)
	{
		return m_hDebugHandle->GetErrorCode(nNode);
	}
	else
		return -1;
}
const char * CExportModule::GetDebugMsg(int nNode)
{
	if(m_hDebugHandle != NULL)
	{
		return m_hDebugHandle->GetMsg(nNode);
	}
	else
		return NULL;
}
bool CExportModule::DelDebugInfo(int nNode)
{
	if(m_hDebugHandle != NULL)
	{
		return m_hDebugHandle->DelInfo(nNode);
	}
	else
		return false;
}
void CExportModule::ClearDebugInfo(void)
{
	if(m_hDebugHandle != NULL)
	{
		return m_hDebugHandle->ClearInfo();
	}
}
void CExportModule::SaveDebugInfo(const char * cSaveName)
{
	if(m_hDebugHandle != NULL)
	{
		return m_hDebugHandle->SaveInfo(cSaveName);
	}
}
string CExportModule::GetDosCommand()
{
	if(m_hDebugHandle != NULL)
	{
		return m_hDebugHandle->GetDosCommand();
	}
}
bool CExportModule::CreateODBCModule(void)
{
	m_hODBCHandle = CreateODBC();
	if(m_hODBCHandle != NULL)
	{
		return true;
	}
	else
		return false;
}
bool CExportModule::InitODBCLink(void)
{
	
	if(m_hODBCHandle != NULL)
	{
		return m_hODBCHandle->InitODBCLink();
	}
	else
		return false;
}
bool CExportModule::LinkDataBase(const char * cServerName,const char * cUserName,const char * cPassword)
{
	if(m_hODBCHandle != NULL)
	{
		return m_hODBCHandle->LinkDataBase(cServerName,cUserName,cPassword);
	}
	else
		return false;
}
int CExportModule::CreateExecHandle(void)
{
	if(m_hODBCHandle != NULL)
	{
		return m_hODBCHandle->CreateExecHandle();
	}
	else
		return -1;
}
bool CExportModule::BindDataBaseCol(int nExeHandleIndex,int nType,unsigned short usColumnNumber,void * pValue,int iBufferLen,int &iDataFlag)
{
	if(m_hODBCHandle != NULL)
	{
		return m_hODBCHandle->BindDataBaseCol(nExeHandleIndex,nType,usColumnNumber,pValue,iBufferLen,iDataFlag);
	}
	else
		return false;
}
bool CExportModule::ExecMultipleSQL(int nExeHandleIndex,vector<string> vCommand)
{
	if(m_hODBCHandle != NULL)
	{
		return m_hODBCHandle->ExecMultipleSQL(nExeHandleIndex,vCommand);
	}
	else
		return false;
}
bool CExportModule::ExecSQL(int nExeHandleIndex,const char * cCommand)
{
	if(m_hODBCHandle != NULL)
	{
		return m_hODBCHandle->ExecSQL(nExeHandleIndex,cCommand);
	}
	else
		return false;
}
bool CExportModule::ExecFetch(int nExeHandleIndex,int &iNoDataFlag)
{
	if(m_hODBCHandle != NULL)
	{
		return m_hODBCHandle->ExecFetch(nExeHandleIndex,iNoDataFlag);
	}
	else
		return false;
}
bool CExportModule::FreeExecHandle(int nExeHandleIndex)
{
	if(m_hODBCHandle != NULL)
	{
		return m_hODBCHandle->FreeExecHandle(nExeHandleIndex);
	}
	else
		return false;
}
bool CExportModule::CloseDataBase(void)
{
	if(m_hODBCHandle != NULL)
	{
		return m_hODBCHandle->CloseDataBase();
	}
	else
		return false;
}
bool CExportModule::CreateTcp(void)
{
	m_hTcpHandle = CreateTcpModule();
	if(m_hTcpHandle != NULL)
	{
		return true;
	}
	else
		return false;
}
bool CExportModule::InitServer(int nPort,int nListen)
{
	if(m_hTcpHandle != NULL)
	{
		return m_hTcpHandle->InitServer(nPort,nListen);
	}
	else
		return false;
}
bool CExportModule::SetSendFlashMsg(int nFlag)
{
	if(m_hTcpHandle != NULL)
	{
		return m_hTcpHandle->SetSendFlashMsg(nFlag);
	}
	else
		return false;
}
bool CExportModule::StartServer(void)
{
	if(m_hTcpHandle != NULL)
	{
		return m_hTcpHandle->StartServer();
	}
	else
		return false;
}
bool CExportModule::StopService(void)
{
	if(m_hTcpHandle != NULL)
	{
		return m_hTcpHandle->StopService();
	}
	else
		return false;
}
bool CExportModule::InitClient(void)
{
	if(m_hTcpHandle != NULL)
	{
		return m_hTcpHandle->InitClient();
	}
	else
		return false;
}
bool CExportModule::StartClient(void)
{
	if(m_hTcpHandle != NULL)
	{
		return m_hTcpHandle->StartClient();
	}
	else
		return false;
}
bool CExportModule::ConnectServer(const char * cIPAddr,int nPort)
{
	if(m_hTcpHandle != NULL)
	{
		return m_hTcpHandle->ConnectServer(cIPAddr,nPort);
	}
	else
		return false;
}
vector<GUIDKEY> CExportModule::GetClientID(void)
{
	vector<GUIDKEY>		vClientID;
	vClientID.clear();
	if(m_hTcpHandle != NULL)
	{
		vClientID = m_hTcpHandle->GetClientID();
	}
	return vClientID;
}
vector<GUIDKEY> CExportModule::GetServerID(void)
{
	vector<GUIDKEY>		vServerID;
	vServerID.clear();
	if(m_hTcpHandle != NULL)
	{
		vServerID = m_hTcpHandle->GetServerID();
	}
	return vServerID;
}
vector<string> CExportModule::RecvServerMsg(GUIDKEY nClientID)
{
	vector<string>		vMsg;
	vMsg.clear();
	if(m_hTcpHandle != NULL)
	{
		vMsg = m_hTcpHandle->RecvServerMsg(nClientID);
	}
	return vMsg;
}
void CExportModule::CallRecvServerMsg(GUIDKEY nClientID,NetCallBack pCallbackServer)
{
	if(m_hTcpHandle != NULL)
	{
		m_hTcpHandle->CallRecvServerMsg(nClientID,pCallbackServer);
	}
}
vector<string> CExportModule::RecvClientMsg(GUIDKEY nServerID)
{
	vector<string>		vMsg;
	vMsg.clear();
	if(m_hTcpHandle != NULL)
	{
		vMsg = m_hTcpHandle->RecvClientMsg(nServerID);
	}
	return vMsg;
}
void CExportModule::CallRecvClientMsg(GUIDKEY nServerID,NetCallBack pCallbackClient)
{
	if(m_hTcpHandle != NULL)
	{
		m_hTcpHandle->CallRecvClientMsg(nServerID,pCallbackClient);
	}
}
bool CExportModule::SendServerMsg(GUIDKEY nClientID,vector<string> vSend)
{
	if(m_hTcpHandle != NULL)
	{
		return m_hTcpHandle->SendServerMsg(nClientID,vSend);
	}
	else
		return false;
}
bool CExportModule::SendClientMsg(GUIDKEY nServerID,vector<string> vSend)
{
	if(m_hTcpHandle != NULL)
	{
		return m_hTcpHandle->SendClientMsg(nServerID,vSend);
	}
	else
		return false;
}
bool CExportModule::CloseServerClient(GUIDKEY nClientID)
{
	if(m_hTcpHandle != NULL)
	{
		return m_hTcpHandle->CloseServerClient(nClientID);
	}
	else
		return false;
}
bool CExportModule::CloseClient(GUIDKEY nServerID)
{
	if(m_hTcpHandle != NULL)
	{
		return m_hTcpHandle->CloseClient(nServerID);
	}
	else
		return false;
}
bool CExportModule::IsExistGUID(GUIDKEY nLinkID)
{
	if(m_hTcpHandle != NULL)
	{
		return m_hTcpHandle->IsExistGUID(nLinkID);
	}
	else
		return false;
}
CExportModuleInterface * CreateExportHandle()
{
	CExportModuleInterface * hHandle;
	hHandle = static_cast<CExportModuleInterface *>(CExportModule::GetExportModule());
	if(hHandle != NULL)
	{
		return hHandle;
	}
	else
		return NULL;

}