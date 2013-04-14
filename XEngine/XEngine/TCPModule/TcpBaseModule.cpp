#include "TcpBaseModule.h"
CTcpBaseModule * CTcpBaseModule::m_hTCPHandle =  NULL;
int								 g_iTCPHandleCount = 0;


CRITICAL_SECTION 									g_hTCPServerVectorMutex;
CRITICAL_SECTION 									g_hTCPClientVectorMutex;
CRITICAL_SECTION									g_hTCPServerIOCPVectorMutex;
CRITICAL_SECTION									g_hTCPClientIOCPVectorMutex;
CRITICAL_SECTION									g_hTCPServerThreadMutex;
CRITICAL_SECTION									g_hTCPClientThreadMutex;

CTcpBaseModule::CTcpBaseModule(void)
{
	m_hIOCPHandleServer = NULL;
	m_hIOCPHandleClient = NULL;
	m_hConnect = NULL;

	InitializeCriticalSection(&g_hTCPServerVectorMutex);
	InitializeCriticalSection(&g_hTCPClientVectorMutex);
	InitializeCriticalSection(&g_hTCPServerIOCPVectorMutex);
	InitializeCriticalSection(&g_hTCPClientIOCPVectorMutex);

	InitializeCriticalSection(&g_hTCPServerThreadMutex);
	InitializeCriticalSection(&g_hTCPClientThreadMutex);

	m_hDebugModule = NULL;
	m_hDebugModule = CreateDebugHandle();

}
CTcpBaseModule::~CTcpBaseModule(void)
{
	m_hIOCPHandleServer = NULL;
	m_hIOCPHandleClient = NULL;
	m_hConnect = NULL;

	DeleteCriticalSection(&g_hTCPServerVectorMutex);
	DeleteCriticalSection(&g_hTCPClientVectorMutex);
	DeleteCriticalSection(&g_hTCPServerIOCPVectorMutex);
	DeleteCriticalSection(&g_hTCPClientIOCPVectorMutex);

	DeleteCriticalSection(&g_hTCPServerThreadMutex);
	DeleteCriticalSection(&g_hTCPClientThreadMutex);

	if(m_hDebugModule != NULL)
	{
		m_hDebugModule->ReleaseDebugModule();
	}
}
CTcpBaseModule * CTcpBaseModule::GetTcpBaseModule(void)
{
	if (m_hTCPHandle == NULL)
	{
		m_hTCPHandle = new CTcpBaseModule;
		if (m_hTCPHandle != NULL)
		{
			g_iTCPHandleCount = 0;
			return m_hTCPHandle;
		}
		else
			return NULL;
	}
	else
	{
		g_iTCPHandleCount++;
		return m_hTCPHandle;
	}
}
void CTcpBaseModule::ReleaseTcpBaseModule(void)
{
	if (m_hTCPHandle != NULL && g_iTCPHandleCount == 0)
	{
		delete m_hTCPHandle;
		m_hTCPHandle = NULL;
	}
	else
	{
		g_iTCPHandleCount--;
	}
}
int CTcpBaseModule::FindClient(GUIDKEY nClientID)
{
	{
		CTcpServerIOCPVectorCriticalSection					m_hCSectionVector;
		map<GUIDKEY,int>::iterator					m_pClient;
		m_pClient = m_mClientMap.find(nClientID);
		if (m_pClient != m_mClientMap.end())
			return m_pClient->second;
		else
			return -1;
	}

}
void CTcpBaseModule::UpdataClient(void)
{
	{
		CTcpServerIOCPVectorCriticalSection						m_hCSectionVector;
		m_mClientMap.clear();
		for (int nCount = 0; nCount < (int)m_vClient.size(); ++nCount)
		{
			m_mClientMap.insert(make_pair(m_vClient[nCount]->pClientHandle->nGuid,nCount));
		}
	}
}
int CTcpBaseModule::FindServer(GUIDKEY nServerID)
{
	{
		CTcpClientIOCPVectorCriticalSection						m_hCSectionVector;
		map<GUIDKEY,int>::iterator					m_pServer;
		m_pServer = m_mServerMap.find(nServerID);
		if (m_pServer != m_mServerMap.end())
			return m_pServer->second;
		else
			return -1;
	}
}
void CTcpBaseModule::UpdataServer(void)
{
	{
		CTcpClientIOCPVectorCriticalSection						m_hCSectionVector;
		m_mServerMap.clear();
		for (int nCount = 0; nCount < (int)m_vServer.size(); ++nCount)
		{
			m_mServerMap.insert(make_pair(m_vServer[nCount]->pServerHandle->nGuid,nCount));
		}
	}
}
bool CTcpBaseModule::StartSocket(void)
{
	WSADATA		sWSAData;
	WORD		wVer;
	int			hr = 0;

	wVer = MAKEWORD(2,2);
	hr = WSAStartup(wVer,&sWSAData);
	if (hr == 0)
	{
		if (m_hDebugModule != NULL)
		{
			m_hDebugModule->AddMsgInfo(StartSocketSucceed);
		}
		return true;
	}
	else
	{
		if (m_hDebugModule != NULL)
		{
			hr = WSAGetLastError();
			m_hDebugModule->AddMsgInfo(StartSocketError);
			m_hDebugModule->AddErrorInfo(hr);
		}
		return false;
	}
}
bool CTcpBaseModule:: CreateSocketListen(int nListenType)
{
	int												hr = 0;
	if (nListenType == LT_TCPServer)
	{
		m_hListen = WSASocket(AF_INET, SOCK_STREAM, 0,NULL,0,WSA_FLAG_OVERLAPPED);
		if (m_hListen != NULL)
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(CreateSocketListenSucceed);
			}
			return true;
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				hr = WSAGetLastError();
				m_hDebugModule->AddMsgInfo(CreateSocketListenError);
				m_hDebugModule->AddErrorInfo(hr);
			}
			return false;
		}
	}
	else
	{
		m_hConnect = WSASocket(AF_INET, SOCK_STREAM, 0,NULL,0,WSA_FLAG_OVERLAPPED);
		if (m_hListen != NULL)
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(CreateSocketConnectSucceed);
			}
			return true;
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				hr = WSAGetLastError();
				m_hDebugModule->AddMsgInfo(CreateSocketConnectError);
				m_hDebugModule->AddErrorInfo(hr);
			}
			return false;
		}
	}

}
bool CTcpBaseModule::BindSocket(int Port)
{
	int												hr = 0;
	SOCKADDR_IN addr; 
	addr.sin_family = AF_INET; 
	addr.sin_port = ::htons( Port ); 
	addr.sin_addr.S_un.S_addr = INADDR_ANY; 
	hr = bind( m_hListen, (sockaddr *)&addr, sizeof( addr ) ); 
	if (hr == 0)
	{
		if (m_hDebugModule != NULL)
		{
			m_hDebugModule->AddMsgInfo(BindSocketListenSucceed);
		}
		return true;
	}
	else
	{
		if (m_hDebugModule != NULL)
		{
			hr = WSAGetLastError();
			m_hDebugModule->AddMsgInfo(BindSocketListenError);
			m_hDebugModule->AddErrorInfo(hr);
		}
		return false;
	}
	return true;
}
bool CTcpBaseModule::ListenSocket(int nListen)
{
	int												hr = 0;
	hr = listen( m_hListen,nListen);
	if (hr == 0)
	{
		if (m_hDebugModule != NULL)
		{
			m_hDebugModule->AddMsgInfo(SocketListenSucceed);
		}
		return true;
	}
	else
	{
		if (m_hDebugModule != NULL)
		{
			hr = WSAGetLastError();
			m_hDebugModule->AddMsgInfo(SocketListenError);
			m_hDebugModule->AddErrorInfo(hr);
		}
		return false;
	}
}
SOCKET CTcpBaseModule::LinkClient(SOCKADDR_IN * pAddr)
{
	SOCKET											hClient = NULL;
	SOCKADDR_IN 									addr;
	int												addrLen = sizeof(addr);
	int												hr = 0;
	hClient = WSAAccept( m_hListen,(SOCKADDR *)&addr,&addrLen,NULL,5000);
	if (hClient != NULL)
	{
		if (m_hDebugModule != NULL)
		{
			m_hDebugModule->AddMsgInfo(LinkClientSucceed);
		}
		pAddr = &addr;
		return hClient;
	}
	else
	{
		if (m_hDebugModule != NULL)
		{
			hr = WSAGetLastError();
			m_hDebugModule->AddMsgInfo(LinkClientError);
			m_hDebugModule->AddErrorInfo(hr);
		}
		return NULL;
	}
}
SOCKET CTcpBaseModule::ConnectOtherServer(const char * IPAddr,int Port)
{
	int												hr = 0;
	SOCKADDR_IN										Clientaddr;
	if (IPAddr!=NULL && Port>=0 && Port<=65535)
	{
		Clientaddr.sin_family=AF_INET;
		Clientaddr.sin_port=htons(Port);
		Clientaddr.sin_addr.s_addr=inet_addr(IPAddr);
		int option_value = TRUE;
		if ( 0 != setsockopt( m_hConnect,SOL_SOCKET,SO_REUSEADDR,( char * )&option_value,sizeof( option_value ) ) )
		{			
			if (m_hDebugModule != NULL)
			{
				hr = WSAGetLastError();
				m_hDebugModule->AddMsgInfo(ConnectOtherServerError);
				m_hDebugModule->AddMsgInfo(hr);
			}
			return NULL;
		}
		//10035
		linger no_linger;
		no_linger.l_onoff   = 1;
		no_linger.l_linger  = 0;
		if ( 0 != setsockopt( m_hConnect,SOL_SOCKET,SO_LINGER,( char * )&no_linger,sizeof( no_linger ) ) )
		{
			if (m_hDebugModule != NULL)
			{
				hr = WSAGetLastError();
				m_hDebugModule->AddMsgInfo(ConnectOtherServerError);
				m_hDebugModule->AddMsgInfo(hr);
				
			}
			return NULL;
		}
		hr = connect( m_hConnect,(SOCKADDR *)&Clientaddr,sizeof( Clientaddr ) );
		if ( hr == 0 )
		{
			option_value = 65535;
			if ( 0 != setsockopt( m_hConnect,SOL_SOCKET,SO_RCVBUF,( char* )&option_value,sizeof( option_value ) ) )
			{
				if (m_hDebugModule != NULL)
				{
					hr = WSAGetLastError();
					m_hDebugModule->AddMsgInfo(ConnectOtherServerError);
					m_hDebugModule->AddMsgInfo(hr);
				}
				return NULL;
			}
			option_value = 65535;
			if ( 0 != setsockopt( m_hConnect,SOL_SOCKET,SO_SNDBUF,( char* )&option_value,sizeof( option_value ) ) )
			{
				if (m_hDebugModule != NULL)
				{
					hr = WSAGetLastError();
					m_hDebugModule->AddMsgInfo(ConnectOtherServerError);
					m_hDebugModule->AddMsgInfo(hr);
				}
				return NULL;
			}
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(ConnectOtherServerSucceed);
			}
			return m_hConnect;
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				hr = WSAGetLastError();
				m_hDebugModule->AddMsgInfo(ConnectOtherServerError);
				m_hDebugModule->AddErrorInfo(hr);
			}
			return NULL;
		}
	}
	else
	{
		if (m_hDebugModule != NULL)
		{
			hr = WSAGetLastError();
			m_hDebugModule->AddMsgInfo(ConnectOtherServerError);
			m_hDebugModule->AddErrorInfo(hr);
		}
		return NULL;
	}
}

bool CTcpBaseModule::ReceiveMsg(GUIDKEY nLinkID,int nType)
{
	int												hr=0;
	WSABUF											wsabuf;
	DWORD											nRecvBytes = 0;
	DWORD											flag = 0;
	int												iLinkPos;

	if (nType == LT_TCPServer)
	{
		iLinkPos = this->FindClient(nLinkID);
		if (iLinkPos != -1)
		{
			{
				CTcpServerIOCPVectorCriticalSection						hCSectionVector;
				strncpy(m_vClient[iLinkPos]->pClientType->cRecvMsg,"\0",MAXNETSIZE);
				wsabuf.buf  = m_vClient[iLinkPos]->pClientType->cRecvMsg;
				wsabuf.len  = MAXNETSIZE;

				hr = WSARecv(m_vClient[iLinkPos]->pClientHandle->hSocket,&wsabuf, 1, &nRecvBytes, &flag, &m_vClient[iLinkPos]->pClientType->ol, NULL);
			}
			if (hr!=SOCKET_ERROR)
			{
				if (m_hDebugModule != NULL)
				{
					m_hDebugModule->AddMsgInfo(RecvMsgSucceed);
					
				}
				return true;
			}
			else
			{
				hr = WSAGetLastError();
				if (hr == WSA_IO_PENDING)
				{
					/*
					if (m_hDebugModule != NULL)
					{
						hr = WSAGetLastError();
						m_hDebugModule->AddMsgInfo(RecvMsgError);
						m_hDebugModule->AddErrorInfo(hr);
					}
					*/
					return true;
				}
				if (m_hDebugModule != NULL)
				{
					hr = WSAGetLastError();
					m_hDebugModule->AddMsgInfo(RecvMsgError);
					m_hDebugModule->AddErrorInfo(hr);
				}
				return false;
			}
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(InputValueError);
			}
			return false;
		}
	}
	else
	{
		iLinkPos = this->FindServer(nLinkID);
		if (iLinkPos != -1)
		{
			{
				CTcpClientIOCPVectorCriticalSection						hCSectionVector;
				strncpy(m_vServer[iLinkPos]->pServerType->cRecvMsg,"\0",MAXNETSIZE);
				wsabuf.buf  = m_vServer[iLinkPos]->pServerType->cRecvMsg;
				wsabuf.len  = MAXNETSIZE;

				hr = WSARecv(m_vServer[iLinkPos]->pServerHandle->hSocket,&wsabuf, 1, &nRecvBytes, &flag, &m_vServer[iLinkPos]->pServerType->ol, NULL);
			}
			if (hr!=SOCKET_ERROR)
			{
				if (m_hDebugModule != NULL)
				{
					m_hDebugModule->AddMsgInfo(RecvMsgSucceed);
				}
				return true;
			}
			else
			{
				hr = WSAGetLastError();
				if (hr == WSA_IO_PENDING)
				{
					if (m_hDebugModule != NULL)
					{
						hr = WSAGetLastError();
						m_hDebugModule->AddMsgInfo(RecvMsgError);
						m_hDebugModule->AddErrorInfo(hr);
					}
					return true;
				}
				if (m_hDebugModule != NULL)
				{
					hr = WSAGetLastError();
					m_hDebugModule->AddMsgInfo(RecvMsgError);
					m_hDebugModule->AddErrorInfo(hr);
				}
				return false;
			}
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(InputValueError);
			}
			return false;
		}
	}

}	
bool CTcpBaseModule::SendMsg(GUIDKEY nLinkID,vector<string> vSendMsg,int nType)
{
	int												hr=0;
	char											cSendBuffer[MAXNETSIZE] = {"\0"};
	WSABUF											SendBuf;
	ULONG											nSendBytes;
	int												iLinkPos;

	if (nType == LT_TCPServer)
	{
		iLinkPos = this->FindClient(nLinkID);
		if (iLinkPos != -1)
		{
			{
				CTcpServerIOCPVectorCriticalSection						hCSectionVector;
				for (int nCount = 0; nCount < (int)vSendMsg.size(); ++nCount)
				{
					memcpy(cSendBuffer,vSendMsg[nCount].c_str(),vSendMsg[nCount].size());
					SendBuf.buf = cSendBuffer;
					SendBuf.len =(int)vSendMsg[nCount].size();
					hr = WSASend(m_vClient[iLinkPos]->pClientHandle->hSocket,&SendBuf, 1,&nSendBytes,0,&m_vClient[iLinkPos]->pClientType->ol,NULL);
					if (hr != SOCKET_ERROR)
					{
						continue;
					}
					else
					{
						if (m_hDebugModule != NULL)
						{
							hr = WSAGetLastError();
							m_hDebugModule->AddMsgInfo(SendMsgError);
							m_hDebugModule->AddErrorInfo(hr);
						}
						return false;
					}
				}
			}
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(SendMsgSucceed);
			}
			return true;
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(InputValueError);
			}
			return false;
		}
	}
	else
	{
		iLinkPos = this->FindServer(nLinkID);
		if (iLinkPos != -1)
		{
			{
				CTcpClientIOCPVectorCriticalSection						hCSectionVector;
				for (int nCount = 0; nCount < (int)vSendMsg.size(); ++nCount)
				{
					memcpy(cSendBuffer,vSendMsg[nCount].c_str(),vSendMsg[nCount].size());
					SendBuf.buf = cSendBuffer;
					SendBuf.len =(int)vSendMsg[nCount].size();
					hr = WSASend(m_vServer[iLinkPos]->pServerHandle->hSocket,&SendBuf, 1,&nSendBytes,0,&m_vServer[iLinkPos]->pServerType->ol,NULL);
					if (hr != SOCKET_ERROR)
					{
						continue;
					}
					else
					{
						if (m_hDebugModule != NULL)
						{
							hr = WSAGetLastError();
							m_hDebugModule->AddMsgInfo(SendMsgError);
							m_hDebugModule->AddErrorInfo(hr);
						}
						return false;
					}
				}
			}
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(SendMsgSucceed);
			}
			return true;
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(InputValueError);
			}
			return false;
		}
	}


}
bool CTcpBaseModule::SendFlashMsg(GUIDKEY nLinkID)
{
	vector<string>									vParam;
	int												hr=0;
	char											cSendBuffer[MAXNETSIZE] = {"<cross-domain-policy><allow-access-from domain=\"*\" to-ports=\"*\"/></cross-domain-policy>\0"};
	WSABUF											SendBuf;
	ULONG											nSendBytes;
	char											cErrorCode[MAXCHARSIZE];
	int												m_iLinkPos;

	m_iLinkPos = this->FindClient(nLinkID);
	if (m_iLinkPos != -1)
	{
		{
			CTcpServerIOCPVectorCriticalSection						hCSectionVector;
			SendBuf.buf = cSendBuffer;
			SendBuf.len =(int)strlen(cSendBuffer) + 1;
			hr = WSASend(m_vClient[m_iLinkPos]->pClientHandle->hSocket,&SendBuf, 1,&nSendBytes,0,&m_vClient[m_iLinkPos]->pClientType->ol,NULL);
			if (hr != SOCKET_ERROR)
			{

			}
			else
			{
				if (m_hDebugModule != NULL)
				{
					hr = WSAGetLastError();
					m_hDebugModule->AddMsgInfo(SendMsgError);
					m_hDebugModule->AddErrorInfo(hr);
					
				}
				return false;
			}
		}
		if (m_hDebugModule != NULL)
		{
			m_hDebugModule->AddMsgInfo(SendMsgSucceed);
		}
		return true;
	}
	else
	{
		if (m_hDebugModule != NULL)
		{
			m_hDebugModule->AddMsgInfo(InputValueError);
		}
		return false;
	}
}
GUIDKEY CTcpBaseModule::GetMsgIOCPServer(int & nType,vector<string> &vMsg)
{
	stIOCPHandleServer *			pIOHandle;
	stIOCPIO	*					pIOData;
	unsigned long					nRecv = 0;
	int								iClientPos;
	GUIDKEY							nGuid = 0;
	bool							hr;
	string							sMsg;
	int								nCount;
	hr = GetQueuedCompletionStatus(m_hIOCPHandleServer,&nRecv,(LPDWORD)&pIOHandle,(LPOVERLAPPED *)&pIOData, WSA_INFINITE );
	if (hr == true)
	{
		if (nRecv == 0 && pIOHandle != NULL && pIOData != NULL)
		{
			int nErrorCode = GetLastError();
			nGuid = pIOHandle->nGuid;
			if (this->CloseSocket(pIOHandle->nGuid,LT_TCPServer,nErrorCode) == true)
			{
				nType = OP_CLOSE;
				return nGuid;
			}
			else
			{
				return -1;
			}
		}
		else if (nRecv == 0 && pIOHandle == NULL && pIOData == NULL)
		{
			nType = OP_STOP;
			return -1;
		}
		else
		{	
			iClientPos = this->FindClient(pIOHandle->nGuid);
			if (iClientPos != -1)
			{
				for ( nCount = 0; nCount < (int)nRecv; ++nCount)
				{
					if (pIOData->cRecvMsg[nCount] != 0)
						break;
				}
				if (nCount != nRecv)
				{
					{
						CTcpServerIOCPVectorCriticalSection						hCSectinVector;
						vMsg.clear();
						m_vClient[iClientPos]->pClientHandle = pIOHandle;
						m_vClient[iClientPos]->pClientType = pIOData;
						sMsg.append(pIOData->cRecvMsg,nRecv);
						for ( nCount = 0; nCount < MAXNETSIZE; ++nCount)
						{
							pIOData->cRecvMsg[nCount] = 0;
						}
						vMsg.push_back(sMsg);
						nType = OP_READ;
						sMsg.clear();
					}
					return pIOHandle->nGuid;
				}
				else
					return -1;
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		int nError = GetLastError();
		if(nError == 64)
		{
			nGuid = pIOHandle->nGuid;
			
			this->CloseSocket(pIOHandle->nGuid,LT_TCPServer,nError);
			nType = OP_CLOSE;
			return nGuid;
		}
		else
			return -1;
		
	}

}
GUIDKEY CTcpBaseModule::GetMsgIOCPClient(int & nType,vector<string> &vMsg)
{
	stIOCPHandleClient *			pIOHandle;
	stIOCPIO	*					pIOData;
	unsigned long					nRecv = 0;
	int								iServerPos;
	GUIDKEY							nGuid = 0;
	bool							hr;
	string							sMsg;
	int								nCount;
	hr = GetQueuedCompletionStatus(m_hIOCPHandleClient,&nRecv,(LPDWORD)&pIOHandle,(LPOVERLAPPED *)&pIOData, WSA_INFINITE );
	if (hr == true)
	{
		if (nRecv == 0  && pIOHandle != NULL && pIOData != NULL)
		{
			int nErrorCode = GetLastError();
			nGuid = pIOHandle->nGuid;
			if (this->CloseSocket(pIOHandle->nGuid,LT_TCPClient,nErrorCode) == true)
			{
				nType = OP_CLOSE;
				return nGuid;
			}
			else
			{
				return -1;
			}
		}
		else if (nRecv == 0 && pIOHandle == NULL && pIOData == NULL)
		{
			nType = OP_STOP;
			return -1;
		}
		else
		{	
			iServerPos = this->FindServer(pIOHandle->nGuid);
			if (iServerPos != -1)
			{
				for ( nCount = 0; nCount < (int)nRecv; ++nCount)
				{
					if (pIOData->cRecvMsg[nCount] != 0)
						break;
				}
				if (nCount != nRecv)
				{
					{
						CTcpClientIOCPVectorCriticalSection							hCSectionVector;
						vMsg.clear();
						m_vServer[iServerPos]->pServerHandle = pIOHandle;
						m_vServer[iServerPos]->pServerType = pIOData;
						sMsg.append(pIOData->cRecvMsg,nRecv);
						for ( nCount = 0; nCount < MAXNETSIZE; ++nCount)
						{
							pIOData->cRecvMsg[nCount] = 0;
						}
						vMsg.push_back(sMsg);
						nType = OP_READ;
						sMsg.clear();
					}
					return pIOHandle->nGuid;
				}
				else
					return -1;
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		int nError = GetLastError();
		if(nError == 64)
		{
			nGuid = pIOHandle->nGuid;
			this->CloseSocket(pIOHandle->nGuid,LT_TCPClient,nError);
			nType = OP_CLOSE;
			return nGuid;
		}
		else
			return -1;
	}

}
bool CTcpBaseModule::CloseSocket(GUIDKEY nLinkID,int nType,int nErrorCode)
{
	int												hr=0;
	int												iLinkPos = 0;
	if (nType == LT_TCPServer)
	{
		iLinkPos = this->FindClient(nLinkID);
		if (iLinkPos != -1)
		{
			{
				CTcpServerIOCPVectorCriticalSection						hCSectinVector;
				if(nErrorCode == 0)
				{
					if(shutdown(m_vClient[iLinkPos]->pClientHandle->hSocket,2) == 0)
						hr = closesocket(m_vClient[iLinkPos]->pClientHandle->hSocket);
				}
			}
			if (hr == 0)
			{
				{
					CTcpServerIOCPVectorCriticalSection						hCSectinVector;
					if (m_hDebugModule != NULL)
					{
						m_hDebugModule->AddMsgInfo(CloseSocketSucceed);
					}
					
					//CloseHandle(m_vClient[iLinkPos]->pClientHandle->hIOCPHandle);
					GlobalFree(m_vClient[iLinkPos]->pClientHandle);
					GlobalFree(m_vClient[iLinkPos]->pClientType);
					m_vClient.erase(m_vClient.begin() + iLinkPos);
				}
				this->UpdataClient();
				return true;
			}
			else
			{
				if (m_hDebugModule != NULL)
				{
					hr = WSAGetLastError();
					m_hDebugModule->AddMsgInfo(CloseSocketError);
					m_hDebugModule->AddErrorInfo(hr);
				}
				return false;
			}
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(InputValueError);
			}
			return false;
		}
	}
	else
	{
		iLinkPos = this->FindServer(nLinkID);
		if (iLinkPos != -1)
		{
			{
				CTcpClientIOCPVectorCriticalSection						hCSectinVector;
				if(nErrorCode == 0)
				{
					//if(shutdown(m_vServer[iLinkPos]->pServerHandle->hSocket,2) == 0)
						hr = closesocket(m_vServer[iLinkPos]->pServerHandle->hSocket);
				}
			}
			if (hr == 0)
			{
				{
					CTcpClientIOCPVectorCriticalSection						hCSectinVector;
					if (m_hDebugModule != NULL)
					{
						m_hDebugModule->AddMsgInfo(CloseSocketSucceed);
					}
					//CloseHandle(m_vServer[iLinkPos]->pServerHandle->hIOCPHandle);
					GlobalFree(m_vServer[iLinkPos]->pServerHandle);
					GlobalFree(m_vServer[iLinkPos]->pServerType);
					m_vServer.erase(m_vServer.begin() + iLinkPos);
				}
				this->UpdataServer();
				return true;
			}
			else
			{
				if (m_hDebugModule != NULL)
				{
					hr = WSAGetLastError();
					m_hDebugModule->AddMsgInfo(CloseSocketError);
					m_hDebugModule->AddErrorInfo(hr);
				}
				return false;
			}
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(InputValueError);
			}
			return false;
		}
	}

}	
GUIDKEY CTcpBaseModule::CreateIOCPServer(SOCKET hClient,SOCKADDR_IN * pAddr)
{
	int												hr=0;
	HANDLE											hClientHandle;
	SYSTEMTIME										stSystemTime;

	SYSTEM_INFO										stSysInfo;
	GetSystemInfo(&stSysInfo);
	if (hClient == NULL)
	{
		m_hIOCPHandleServer = CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, stSysInfo.dwNumberOfProcessors * 2 ); 
		if (m_hIOCPHandleServer != NULL)
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(CreateIOCPSucceed);
			}
			return -1;
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				hr = GetLastError();
				m_hDebugModule->AddMsgInfo(CreateIOCPError);
				m_hDebugModule->AddErrorInfo(hr);
			}
			return -1;
		}
	}
	else
	{
		stIOCPClientLinkHandle * pClient = new stIOCPClientLinkHandle;
		if (pClient != NULL)
		{
			stIOCPHandleServer * pIOCPHandle = ( stIOCPHandleServer *)::GlobalAlloc( GPTR, sizeof( stIOCPHandleServer ) );
			if (pIOCPHandle != NULL)
			{
				GetSystemTime(&stSystemTime);
				printf("%d\n",stSystemTime.wYear);
				pIOCPHandle->nGuid = stSystemTime.wHour * 10000000 + stSystemTime.wMinute * 100000 + stSystemTime.wSecond * 1000 + stSystemTime.wMilliseconds;
				pIOCPHandle->hSocket = hClient;
				memcpy(&pIOCPHandle->stAddr,&pAddr,sizeof(SOCKADDR_IN));
				hClientHandle = CreateIoCompletionPort((HANDLE)pIOCPHandle->hSocket,m_hIOCPHandleServer,(DWORD) pIOCPHandle,0);
				if (hClientHandle != NULL)
				{
					pIOCPHandle->hIOCPHandle = hClientHandle;
					stIOCPIO * pIOData = ( stIOCPIO *)::GlobalAlloc( GPTR, sizeof( stIOCPIO ) );
					if (pIOData != NULL)
					{
						ZeroMemory(&pIOData->ol,sizeof(OVERLAPPED));

						pClient->pClientHandle = pIOCPHandle;
						pClient->pClientType = pIOData;

						{
							CTcpServerIOCPVectorCriticalSection						hCSectinVector;
							m_vClient.push_back(pClient);
						}
						this->UpdataClient();

						if (m_hDebugModule != NULL)
						{
							m_hDebugModule->AddMsgInfo(CreateIOCPSucceed);
						}
						return pIOCPHandle->nGuid;
					}
					else
					{
						if (m_hDebugModule != NULL)
						{
							m_hDebugModule->AddMsgInfo(CreateValueError);
						}
						return -1;
					}
				}
				else
				{
					if (m_hDebugModule != NULL)
					{
						hr = GetLastError();
						m_hDebugModule->AddMsgInfo(CreateIOCPError);
						m_hDebugModule->AddErrorInfo(hr);
					}
					return -1;
				}
			}
			else
			{
				if (m_hDebugModule != NULL)
				{
					hr = GetLastError();
					m_hDebugModule->AddMsgInfo(CreateIOCPError);
					m_hDebugModule->AddErrorInfo(hr);
				}
				return -1;
			}
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(CreateValueError);
			}
			return -1;
		}
	}
}
GUIDKEY CTcpBaseModule::CreateIOCPClient(SOCKET hConnect)
{
	vector<string>									vParam;
	int												hr=0;
	char											cErrorCode[MAXCHARSIZE];
	HANDLE											hClientHandle;
	SYSTEMTIME										stSystemTime;

	SYSTEM_INFO										stSysInfo;
	if (hConnect == NULL)
	{
		GetSystemInfo(&stSysInfo);
		m_hIOCPHandleClient	= CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, stSysInfo.dwNumberOfProcessors * 2 ); 
		if (m_hIOCPHandleClient != NULL)
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(CreateIOCPSucceed);
			}
			return -1;
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				hr = GetLastError();
				m_hDebugModule->AddMsgInfo(CreateIOCPError);
				m_hDebugModule->AddErrorInfo(hr);
			}
			return -1;
		}
	}
	else
	{
		stIOCPServerLinkHandle * pConnect = new stIOCPServerLinkHandle;
		if (pConnect != NULL)
		{
			stIOCPHandleClient * pIOCPHandle = ( stIOCPHandleClient *)::GlobalAlloc( GPTR, sizeof( stIOCPHandleClient ) );
			if (pIOCPHandle != NULL)
			{
				GetSystemTime(&stSystemTime);
				printf("%d\n",stSystemTime.wYear);
				pIOCPHandle->nGuid = stSystemTime.wYear * 100000000000000 + stSystemTime.wMonth * 1000000000000 + stSystemTime.wDayOfWeek * 100000000000 + stSystemTime.wDay * 1000000000 + stSystemTime.wHour * 10000000 + stSystemTime.wMinute * 100000 + stSystemTime.wSecond * 1000 + stSystemTime.wMilliseconds;
				pIOCPHandle->hSocket = hConnect;
				hClientHandle = CreateIoCompletionPort((HANDLE)pIOCPHandle->hSocket,m_hIOCPHandleClient,(DWORD) pIOCPHandle,0);
				if (hClientHandle != NULL)
				{
					pIOCPHandle->hIOCPHandle = hClientHandle;
					stIOCPIO * pIOData = ( stIOCPIO *)::GlobalAlloc( GPTR, sizeof( stIOCPIO ) );
					if (pIOData != NULL)
					{
						ZeroMemory(&pIOData->ol,sizeof(OVERLAPPED));

						pConnect->pServerHandle= pIOCPHandle;
						pConnect->pServerType = pIOData;
						{
							CTcpClientIOCPVectorCriticalSection						hCSectinVector;
							m_vServer.push_back(pConnect);
						}
						this->UpdataServer();

						if (m_hDebugModule != NULL)
						{
							m_hDebugModule->AddMsgInfo(CreateIOCPSucceed);
						}
						return pIOCPHandle->nGuid;
					}
					else
					{
						if (m_hDebugModule != NULL)
						{
							m_hDebugModule->AddMsgInfo(CreateValueError);
						}
						return -1;
					}
				}
				else
				{
					if (m_hDebugModule != NULL)
					{
						hr = GetLastError();
						m_hDebugModule->AddMsgInfo(CreateIOCPError);
						m_hDebugModule->AddErrorInfo(hr);
					}
					return -1;
				}
			}
			else
			{
				if (m_hDebugModule != NULL)
				{
					hr = GetLastError();
					m_hDebugModule->AddMsgInfo(CreateIOCPError);
					m_hDebugModule->AddErrorInfo(hr);
				}
				return -1;
			}
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(CreateValueError);
			}
			return -1;
		}
	}
}
bool CTcpBaseModule::CloseListen(void)
{
	int												hr=0;
	if (m_hListen != NULL)
	{
		hr = closesocket( m_hListen );
		if (hr == 0)
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(CloseSocketSucceed);
			}
			m_hListen = NULL;
			return true;
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				hr = WSAGetLastError();
				m_hDebugModule->AddMsgInfo(CloseSocketError);
				m_hDebugModule->AddErrorInfo(hr);
			}
			return false;
		}
	}
	else
	{
		m_hListen = NULL;
		if (m_hDebugModule != NULL)
		{
			m_hDebugModule->AddMsgInfo(CloseSocketSucceed);
		}
		m_hListen = NULL;
		return true;
	}

}
bool CTcpBaseModule::CloseAllServerIOCP(void)
{
	int												hr=0;
	stIOCPHandleServer *							pIOHandleClient;
	stIOCPIO	*									pIOData;
	int												iClose = 0;
	if (m_hIOCPHandleServer != NULL)
	{
		for (int nCount = 0; nCount< (int)m_vClient.size(); ++nCount)
		{
			pIOHandleClient = m_vClient[nCount]->pClientHandle;
			pIOData = m_vClient[nCount]->pClientType;
			if (PostQueuedCompletionStatus(m_hIOCPHandleServer,(DWORD)iClose,(DWORD)pIOHandleClient,(LPOVERLAPPED)pIOData) == true)
			{
				if (m_hDebugModule == NULL)
				{
					m_hDebugModule->AddMsgInfo(ShoutdownIOCPSucceed);
					continue;
				}
				else
				{
					continue;
				}
				
			}
			else
			{
				if (m_hDebugModule != NULL)
				{
					hr = GetLastError();
					m_hDebugModule->AddMsgInfo(ShoutdownIOCPError);
					m_hDebugModule->AddErrorInfo(hr);
				}
				return false;
			}
		}
	}
	return true;

}
bool CTcpBaseModule::CloseAllClientIOCP(void)
{
	int												hr=0;
	stIOCPHandleClient *							pIOHandleServer;
	stIOCPIO	*									pIOData;
	int												iClose = 0;
	if (m_hIOCPHandleClient != NULL)
	{
		for (int nCount = 0; nCount< (int)m_vServer.size(); ++nCount)
		{
			pIOHandleServer = m_vServer[nCount]->pServerHandle;
			pIOData = m_vServer[nCount]->pServerType;
			if (PostQueuedCompletionStatus(m_hIOCPHandleClient,(DWORD)iClose,(DWORD)pIOHandleServer,(LPOVERLAPPED)pIOData) == true)
			{
				if (m_hDebugModule != NULL)
				{
					m_hDebugModule->AddMsgInfo(ShoutdownIOCPSucceed);
					continue;
				}
				else
				{
					continue;
				}

			}
			else
			{
				if (m_hDebugModule != NULL)
				{
					hr = GetLastError();
					m_hDebugModule->AddMsgInfo(ShoutdownIOCPError);
					m_hDebugModule->AddErrorInfo(hr);
				}
				return false;
			}
		}
	}
	else
	{
		if (m_hDebugModule != NULL)
		{
			m_hDebugModule->AddMsgInfo(InputValueError);
		}
		return false;
	}
	return true;
}
bool CTcpBaseModule::StopServerIOCPHandle(void)
{
	if (m_hIOCPHandleServer != NULL)
	{
		if (PostQueuedCompletionStatus(m_hIOCPHandleServer,(DWORD)0,(DWORD)NULL,(LPOVERLAPPED)NULL) != true)
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(StopIOCPError);
			}
			return false;
		}
	}
	if (m_hDebugModule != NULL)
	{
		m_hDebugModule->AddMsgInfo(StopIOCPSucceed);
	}
	return true;
}
bool CTcpBaseModule::StopClientIOCPHandle(void)
{
	if (m_hIOCPHandleClient != NULL)
	{
		if (PostQueuedCompletionStatus(m_hIOCPHandleClient,(DWORD)0,(DWORD)NULL,(LPOVERLAPPED)NULL) != true)
		{
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(StopIOCPError);
			}
			return false;
		}
	}
	if (m_hDebugModule != NULL)
	{
		m_hDebugModule->AddMsgInfo(StopIOCPSucceed);
	}
	return true;
}
bool CTcpBaseModule::CloseServerIOCPHandle(void)
{
	vector<string>									vParam;
	int												hr=0;
	char											cErrorCode[MAXCHARSIZE];
	if (m_hIOCPHandleServer != NULL)
	{
		if ( CloseHandle(m_hIOCPHandleServer) == true)
		{			
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(CloseIOCPSucceed);
			}
			m_hIOCPHandleServer = NULL;
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				hr = GetLastError();
				m_hDebugModule->AddMsgInfo(CloseIOCPError);
				m_hDebugModule->AddErrorInfo(hr);
			}
			m_hIOCPHandleServer = NULL;
			return false;
		}
	}
	
	return true;
}
bool CTcpBaseModule::CloseClientIOCPHandle(void)
{
	vector<string>									vParam;
	int												hr=0;
	char											cErrorCode[MAXCHARSIZE];
	if (m_hIOCPHandleClient != NULL)
	{
		if ( CloseHandle(m_hIOCPHandleClient) == true)
		{			
			if (m_hDebugModule != NULL)
			{
				m_hDebugModule->AddMsgInfo(CloseIOCPSucceed);
			}
			m_hIOCPHandleServer = NULL;
		}
		else
		{
			if (m_hDebugModule != NULL)
			{
				hr = GetLastError();
				m_hDebugModule->AddMsgInfo(CloseIOCPError);
				m_hDebugModule->AddErrorInfo(hr);
			}
			m_hIOCPHandleServer = NULL;
			return false;
		}
	}

	return true;
}
CTcpServerVectorCriticalSection::CTcpServerVectorCriticalSection(void)
{
	EnterCriticalSection(&g_hTCPServerVectorMutex);
};
CTcpServerVectorCriticalSection::~CTcpServerVectorCriticalSection(void)
{
	LeaveCriticalSection(&g_hTCPServerVectorMutex);
};
CTcpClientVectorCriticalSection::CTcpClientVectorCriticalSection(void)
{
	EnterCriticalSection(&g_hTCPClientVectorMutex);
};
CTcpClientVectorCriticalSection::~CTcpClientVectorCriticalSection(void)
{
	LeaveCriticalSection(&g_hTCPClientVectorMutex);
};
CTcpServerIOCPVectorCriticalSection::CTcpServerIOCPVectorCriticalSection(void)
{
	EnterCriticalSection(&g_hTCPServerIOCPVectorMutex);
};
CTcpServerIOCPVectorCriticalSection::~CTcpServerIOCPVectorCriticalSection(void)
{
	LeaveCriticalSection(&g_hTCPServerIOCPVectorMutex);
};
CTcpClientIOCPVectorCriticalSection::CTcpClientIOCPVectorCriticalSection(void)
{
	EnterCriticalSection(&g_hTCPClientIOCPVectorMutex);
};
CTcpClientIOCPVectorCriticalSection::~CTcpClientIOCPVectorCriticalSection(void)
{
	LeaveCriticalSection(&g_hTCPClientIOCPVectorMutex);
};
CTcpServerThreadCriticalSection::CTcpServerThreadCriticalSection(void)
{
	EnterCriticalSection(&g_hTCPServerThreadMutex);
}
CTcpServerThreadCriticalSection::~CTcpServerThreadCriticalSection(void)
{
	LeaveCriticalSection(&g_hTCPServerThreadMutex);
}
CTcpClientThreadCriticalSection::CTcpClientThreadCriticalSection(void)
{
	EnterCriticalSection(&g_hTCPClientThreadMutex);
}
CTcpClientThreadCriticalSection::~CTcpClientThreadCriticalSection(void)
{
	LeaveCriticalSection(&g_hTCPClientThreadMutex);
}
