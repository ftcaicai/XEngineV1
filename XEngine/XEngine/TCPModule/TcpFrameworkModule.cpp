#include "TcpFrameworkModule.h"

CTcpModuleFramework * CTcpModuleFramework::m_hFramework = NULL;



CTcpModuleFramework::CTcpModuleFramework()
{
	m_iSendFlashMsg = 0;
	m_bServerFlag = false;
	m_bClientFlag = false;
	m_iHandleCount = 0;
}
CTcpModuleFramework::~CTcpModuleFramework()
{
	m_iSendFlashMsg = 0;
	m_bServerFlag = false;
	m_bClientFlag = false;
}
CTcpModuleFramework * CTcpModuleFramework::GetTcpModuleFrameworkHandle(void)
{
	if (m_hFramework == NULL)
	{
		m_hFramework = new CTcpModuleFramework;
		if (m_hFramework != NULL)
		{
			m_hFramework->m_iHandleCount = 0;
			return m_hFramework;
		}
		else
			return NULL;
	}
	else
	{
		m_hFramework->m_iHandleCount++;
		return m_hFramework;
	}
}
void CTcpModuleFramework::ReleaseTcpModuleFrameworkHandle(void)
{
	if (m_hFramework != NULL && m_hFramework->m_iHandleCount == 0)
	{
		delete m_hFramework;
		m_hFramework = NULL;
	}
	else
	{
		m_iHandleCount--;
	}
}
//--------------------------------------------------------内部调用----------------------------------------------------
//对关键的消息进行必要的应答
unsigned int CTcpModuleFramework::NetLoopServer(void)
{
	GUIDKEY							nClientID = 0;
	int								nType = OP_NONE;
	int								nClientPos;
	vector<string>					vRecv;
	vector<string>					vSendMsg;
	string							sTemp;
	char							cFlashFlag[23] = {"<policy-file-request/>"};

	while (m_hFramework->m_iLoop == 1)
	{
		{
			CTcpServerThreadCriticalSection							hCSectionThread;
			CTcpBaseModule *	hNet =  CTcpBaseModule::GetTcpBaseModule();
			if (hNet != NULL && nType != OP_STOP)
			{
				nClientID = hNet->GetMsgIOCPServer(nType,vRecv);
				if ( nClientID != -1)
				{
					switch(nType)
					{
					case OP_CLOSE:
						{
							nClientPos = this->FindClient(nClientID);
							if (nClientPos > -1)
							{
								{
									CTcpServerVectorCriticalSection				hCSectionVector;
									m_vClientIO[nClientPos]->vRecv.clear();
									delete m_vClientIO[nClientPos];
									m_vClientIO.erase(m_vClientIO.begin() + nClientPos);
								}
								this->UpdataClient();
							}
						}break;
					case OP_READ:
						{
							nClientPos = this->FindClient(nClientID);
							if (nClientPos > -1)
							{	
								{
									CTcpServerVectorCriticalSection				hCSectionVector;
									if (m_iSendFlashMsg ==1 && strcmp(vRecv[0].c_str(),cFlashFlag) == 0)
									{
										hNet->SendFlashMsg(nClientID);
										vRecv.clear();

									}
									else
									{
										for (int nCount = 0; nCount < (int)vRecv.size(); ++nCount)
										{
											sTemp.clear();
											sTemp.append(vRecv[nCount].c_str(),vRecv[nCount].size());
											m_vClientIO[nClientPos]->vRecv.push_back(sTemp);
										}
										/*
										else if(m_iNetMsgMode == SSM_CallBackMode)
										{
											NetCallBack func = m_pCallbackServer;
											vSendMsg = func(vRecv);
											stTCPSendStruct *	pSend = new stTCPSendStruct;
											if (pSend != NULL)
											{
												pSend->nTargetGuid = _atoi64(vSendMsg[0].c_str());
												pSend->nSendFleg = LT_TCPServer;
												for (int nCount = 1; nCount < (int)vSendMsg.size(); ++nCount)
												{
													m_vClientIO[nClientPos]->vSendSt.push_back(pSend);
												}
											}
										}
										else
										{
											NetCallBack func = m_pCallbackServer;
											vSendMsg = func(vRecv);
											if ((int)vSendMsg.size() > 0)
											{
												for (int nCount = 0; nCount < (int)vSendMsg.size(); ++nCount)
												{
													sTemp.clear();
													sTemp.append(vSendMsg[nCount].c_str(),vSendMsg[nCount].size());
													m_vClientIO[nClientPos]->vRecv.push_back(sTemp);

												}
											}
										}
										*/
										
									}
									vRecv.clear();
								}
								hNet->ReceiveMsg(nClientID,LT_TCPServer);

							}
							else
							{
								{
									CTcpServerVectorCriticalSection				hCSectionVector;
									vRecv.clear();
								}
							}
						}break;
					};
				}
				else
				{
					{
						CTcpServerVectorCriticalSection				hCSectionVector;
						vRecv.clear();
					}
				}
			}
		}
	}
	return 0;
}

unsigned int CTcpModuleFramework::NetLoopClient(void)
{
	GUIDKEY							nServerID;
	int								nType = OP_NONE;
	int								nServerPos;
	vector<string>					vRecv;
	vector<string>					vSendMsg;
	string							sTemp;

	while (m_hFramework->m_iLoop == 1)
	{
		{
			CTcpClientThreadCriticalSection							hCSectionThread;
			CTcpBaseModule *	hNet =  CTcpBaseModule::GetTcpBaseModule();
			if (hNet != NULL && nType != OP_STOP)
			{
				nServerID = hNet->GetMsgIOCPClient(nType,vRecv);
				if ( nServerID != -1)
				{
					switch(nType)
					{
					case OP_CLOSE:
						{
							nServerPos = this->FindServer(nServerID);
							if (nServerPos > -1)
							{
								{
									CTcpClientVectorCriticalSection							hCSectionVector;
									m_vServerIO[nServerPos]->vRecv.clear();
									delete m_vServerIO[nServerPos];
									m_vServerIO.erase(m_vServerIO.begin() + nServerPos);
								}
								this->UpdataServer();
							}
						}break;
					case OP_READ:
						{
							nServerPos = this->FindServer(nServerID);
							if (nServerPos > -1)
							{	
								{
									CTcpClientVectorCriticalSection							hCSectionVector;
									for (int nCount = 0; nCount < (int)vRecv.size(); ++nCount)
									{
										sTemp.clear();
										sTemp.append(vRecv[nCount].c_str(),vRecv[nCount].size());
										m_vServerIO[nServerPos]->vRecv.push_back(sTemp);
									}
									/*
									else if(m_iNetMsgMode == SSM_CallBackMode)
									{
										NetCallBack func = m_pCallbackClient;
										vSendMsg = func(vRecv);
										stTCPSendStruct *	pSend = new stTCPSendStruct;
										if (pSend != NULL)
										{
											pSend->nTargetGuid = _atoi64(vSendMsg[0].c_str());
											pSend->nSendFleg = LT_TCPClient;
											for (int nCount = 1; nCount < (int)vSendMsg.size(); ++nCount)
											{
												m_vServerIO[nServerPos]->vSendSt.push_back(pSend);
											}
										}
									}
									else
									{
										NetCallBack func = m_pCallbackClient;
										vSendMsg = func(vRecv);
										if ((int)vSendMsg.size() > 0)
										{
											for (int nCount = 0; nCount < (int)vSendMsg.size(); ++nCount)
											{
												sTemp.clear();
												sTemp.append(vSendMsg[nCount].c_str(),vSendMsg[nCount].size());
												m_vServerIO[nServerPos]->vRecv.push_back(sTemp);
											}
										}
									}
									*/
									vRecv.clear();
								}
								hNet->ReceiveMsg(nServerID,LT_TCPClient);
							}
							else
							{
								{
									CTcpClientVectorCriticalSection							hCSectionVector;
									vRecv.clear();
								}
							}
						}break;
					};

				}
				else
				{
					{
						CTcpClientVectorCriticalSection										hCSectionVector;
						vRecv.clear();
					}
				}
			}
		}
	}
	return 0;
}

unsigned int CTcpModuleFramework::AcceptLoop(void)
{
	SOCKET							hClient;
	SOCKADDR_IN *					pAddr = NULL;
	GUIDKEY							nClientID = NULL;
	while (m_hFramework->m_iLoop == 1)
	{
		hClient = NULL;
		CTcpBaseModule *	hNet =  CTcpBaseModule::GetTcpBaseModule();
		if (hNet != NULL)
		{
			hClient = hNet->LinkClient(pAddr);
			if ( hClient !=  NULL)
			{
				nClientID = hNet->CreateIOCPServer(hClient,pAddr);
				if ( nClientID != -1)
				{
					stTCPLinkIO * pClientIO = new stTCPLinkIO;
					if (pClientIO != NULL)
					{
						{
							CTcpServerVectorCriticalSection			hCSection;
							pClientIO->nGuid = nClientID;
							m_vClientIO.push_back(pClientIO);
						}
						this->UpdataClient();
						if (hNet->ReceiveMsg(nClientID,LT_TCPServer) == true)
						{
							continue;						
						}
					}
				}
			}
		}		
	}
	return 0;
}
unsigned int CTcpModuleFramework::SendLoop(void)
{
	int						nClientCount = 0;
	int						nServerCount = 0;
	while (m_hFramework->m_iLoop == 1)
	{
		CTcpBaseModule *	hNet =  CTcpBaseModule::GetTcpBaseModule();
		if (hNet != NULL)
		{
			if (nClientCount >= (int)m_vClientIO.size())
			{
				nClientCount = 0;
			}
			if (nServerCount >= (int)m_vServerIO.size())
			{
				nServerCount  = 0;
			}
			if (m_vClientIO.empty() == false)
			{
				{
					CTcpClientVectorCriticalSection				hCSectionVector;
					int iSendStSize = (int)m_vClientIO[nClientCount]->vSendSt.size();
					if ( iSendStSize > 0)
					{
						hNet->SendMsg(m_vClientIO[nClientCount]->vSendSt[0]->nTargetGuid,m_vClientIO[nClientCount]->vSendSt[0]->vSend,m_vClientIO[nClientCount]->vSendSt[0]->nSendFleg);
						m_vClientIO[nClientCount]->vSendSt[0]->vSend.clear();
						delete m_vClientIO[nClientCount]->vSendSt[0];
						m_vClientIO[nClientCount]->vSendSt.erase(m_vClientIO[nClientCount]->vSendSt.begin() + 0);
					}
				}
				++nClientCount;
			}
			if (m_vServerIO.empty() == false)
			{
				{
					CTcpServerVectorCriticalSection				hCSectionVector;
					if ((int)m_vServerIO[nServerCount]->vSendSt.size() > 0)
					{
						hNet->SendMsg(m_vServerIO[nServerCount]->vSendSt[0]->nTargetGuid,m_vServerIO[nServerCount]->vSendSt[0]->vSend,m_vServerIO[nServerCount]->vSendSt[0]->nSendFleg);
						m_vServerIO[nServerCount]->vSendSt[0]->vSend.clear();
						delete m_vServerIO[nServerCount]->vSendSt[0];
						m_vServerIO[nServerCount]->vSendSt.erase(m_vServerIO[nServerCount]->vSendSt.begin() + 0);
					}
				}
				++nServerCount;
			}
			
			
			
		}
		Sleep(m_sSendSet.m_iSleep);
	
	}
	return 0;
}
unsigned _stdcall CTcpModuleFramework::NetThreadServer(void *Param)
{
	CTcpModuleFramework * ST=reinterpret_cast<CTcpModuleFramework *>(Param);
	if (ST!=NULL)
	{
		ST->NetLoopServer();
		return 0;
	}
	else
	{
		return 1;
	}
}
unsigned _stdcall CTcpModuleFramework::NetThreadClient(void *Param)
{
	CTcpModuleFramework * ST=reinterpret_cast<CTcpModuleFramework *>(Param);
	if (ST!=NULL)
	{
		ST->NetLoopClient();
		return 0;
	}
	else
	{
		return 1;
	}
}
unsigned _stdcall CTcpModuleFramework::AcceptThread(void * Param)
{
	CTcpModuleFramework * ST=reinterpret_cast<CTcpModuleFramework *>(Param);
	if (ST!=NULL)
	{
		ST->AcceptLoop();
		return 0;
	}
	else
	{
		return 1;
	}
}
unsigned _stdcall CTcpModuleFramework::SendThread(void * Param)
{
	CTcpModuleFramework * ST=reinterpret_cast<CTcpModuleFramework *>(Param);
	if (ST!=NULL)
	{
		ST->SendLoop();
		return 0;
	}
	else
	{
		return 1;
	}
}
int CTcpModuleFramework::FindClient(GUIDKEY nClientID)
{
	{
		CTcpServerVectorCriticalSection				hCSectionVector;
		map<GUIDKEY,int>::iterator					pClient;
		pClient = m_mClientIO.find(nClientID);
		if (pClient != m_mClientIO.end())
			return pClient->second;
		else
			return -1;
	}
}
void CTcpModuleFramework::UpdataClient(void)
{
	{
		CTcpServerVectorCriticalSection				m_hCSectionVector;
		m_mClientIO.clear();
		for (int nCount = 0; nCount < (int)m_vClientIO.size(); ++nCount)
		{
			m_mClientIO.insert(make_pair(m_vClientIO[nCount]->nGuid,nCount));
		}
	}
}
int CTcpModuleFramework::FindServer(GUIDKEY nServerID)
{
	{
		CTcpClientVectorCriticalSection				hCSectionVector;
		map<GUIDKEY,int>::iterator					pServer;
		pServer = m_mServerIO.find(nServerID);
		if (pServer != m_mServerIO.end())
			return pServer->second;
		else
			return -1;
	}
}
void CTcpModuleFramework::UpdataServer(void)
{
	{
		CTcpClientVectorCriticalSection				hCSectionVector;
		m_mServerIO.clear();
		for (int nCount = 0; nCount < (int)m_vServerIO.size(); ++nCount)
		{
			m_mServerIO.insert(make_pair(m_vServerIO[nCount]->nGuid,nCount));

		}
	}
}
bool CTcpModuleFramework::InitServer(int nPort,int nListen)
{
	CTcpBaseModule *	hNet =  CTcpBaseModule::GetTcpBaseModule();
	if (hNet != NULL)
	{
		if (hNet->StartSocket() == true)
		{
			if (hNet->CreateSocketListen(LT_TCPServer) == true)
			{
				if (hNet->BindSocket(nPort) == true)
				{
					if (hNet->ListenSocket(nListen) == true)
					{
						if (hNet->CreateIOCPServer(NULL,NULL) == -1)
							return true;
						else
							return false;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
bool CTcpModuleFramework::SetSendFlashMsg(int nFlag)
{
	m_iSendFlashMsg = nFlag;
	return true;
}
bool CTcpModuleFramework::StartServer(void)
{
	SYSTEM_INFO					stSysInfo;
	int							nCount;
	GetSystemInfo(&stSysInfo);
	m_hFramework->m_iLoop = 1;
	m_bServerFlag = true;
	for (nCount = 0; nCount < (int)stSysInfo.dwNumberOfProcessors * 2; ++nCount)
	{
		m_stSetServer[nCount].psa.nLength = sizeof(m_stSetServer[nCount].psa);											
		m_stSetServer[nCount].psa.lpSecurityDescriptor = NULL;
		m_stSetServer[nCount].psa.bInheritHandle = TRUE;
		m_stSetServer[nCount].m_iSleep = 16;
		m_stSetServer[nCount].m_handle=(HANDLE)_beginthreadex(&m_stSetServer[nCount].psa,0,NetThreadServer,this,nCount,&m_stSetServer[nCount].m_uID);
		if (m_stSetServer[nCount].m_handle!=NULL)
		{
			continue;
		}
		else
		{
			break;
		}
	}
	if (nCount == stSysInfo.dwNumberOfProcessors * 2)
	{
		m_sAcceptSet.psa.nLength = sizeof(m_sAcceptSet.psa);											
		m_sAcceptSet.psa.lpSecurityDescriptor = NULL;
		m_sAcceptSet.psa.bInheritHandle = TRUE;
		m_sAcceptSet.m_iSleep = 16;
		m_sAcceptSet.m_handle=(HANDLE)_beginthreadex(&m_sAcceptSet.psa,0,AcceptThread,this,0,&m_sAcceptSet.m_uID);
		if (m_sAcceptSet.m_handle!=NULL)
		{	
			m_sSendSet.psa.nLength = sizeof(m_sSendSet.psa);											
			m_sSendSet.psa.lpSecurityDescriptor = NULL;
			m_sSendSet.psa.bInheritHandle = TRUE;
			m_sSendSet.m_iSleep = 30;
			m_sSendSet.m_handle=(HANDLE)_beginthreadex(&m_sSendSet.psa,0,SendThread,this,0,&m_sSendSet.m_uID);
			if (m_sSendSet.m_handle!=NULL)
			{
				return true;
			}
			else
			{
				return false;
			}

		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}
bool CTcpModuleFramework::StopService(void)
{
	int						iState;
	int						nCount;
	int						nCountThread;
	unsigned long			ExitCode;
	SYSTEM_INFO				stSysInfo;


	GetSystemInfo(&stSysInfo);
	CTcpBaseModule *	hNet =  CTcpBaseModule::GetTcpBaseModule();
	m_hFramework->m_iLoop = 0;
	if (hNet != NULL)
	{	
		if (m_bServerFlag == true)
		{
			nCountThread = (int)stSysInfo.dwNumberOfProcessors * 2;
			while(nCountThread != 0)
			{
				for (nCount = 0; nCount < (int)stSysInfo.dwNumberOfProcessors * 2; ++nCount)
				{
					if (m_stSetServer[nCount].m_handle != NULL)
					{
						//等待线程结束
						iState = WaitForSingleObject(m_stSetServer[nCount].m_handle,5000);
						if (iState == WAIT_OBJECT_0)
						{
							//得到线程退出代码
							GetExitCodeThread(m_stSetServer[nCount].m_handle,&ExitCode);
							if (ExitCode == 0)
							{
								--nCountThread;
								printf("一个IOCP线程已停止！\n");
							}
							else
							{
								if (hNet->CloseAllServerIOCP() == true)
								{
									if (hNet->StopServerIOCPHandle() == true)
									{
										continue;
									}
									else
										return false;
								}
								else
									return false;
							}
						}
						else
						{
							if (iState == WAIT_TIMEOUT)
							{
								if (hNet->CloseAllServerIOCP() == true)
								{
									if (hNet->StopServerIOCPHandle() == true)
									{
										continue;
									}
									else
										return false;
								}
								else
									return false;
							}
							return false;
						}
					}
					else
						continue;
					
				}
			}
			if ( hNet->CloseServerIOCPHandle() == false)
			{
				return false;
			}
		}
		if(m_bClientFlag == true)
		{
			nCountThread = (int)stSysInfo.dwNumberOfProcessors * 2;
			while(nCountThread != 0)
			{
				for (nCount = 0; nCount < (int)stSysInfo.dwNumberOfProcessors * 2; ++nCount)
				{
					if (m_stSetClient[nCount].m_handle != NULL)
					{
						//等待线程结束
						iState = WaitForSingleObject(m_stSetClient[nCount].m_handle,5000);
						if (iState == WAIT_OBJECT_0)
						{
							//得到线程退出代码
							GetExitCodeThread(m_stSetClient[nCount].m_handle,&ExitCode);
							if (ExitCode == 0)
							{
								--nCountThread;
								printf("一个IOCP线程已停止！\n");
							}
							else
							{
								if (hNet->CloseAllClientIOCP() == true)
								{
									if (hNet->StopClientIOCPHandle() == true)
									{
										continue;
									}
									else
										return false;
								}
								else
									return false;
							}
						}
						else
						{
							if (iState == WAIT_TIMEOUT)
							{
								if (hNet->CloseAllClientIOCP() == true)
								{
									if (hNet->StopClientIOCPHandle() == true)
									{
										continue;
									}
									else
										return false;
								}
								else
									return false;
							}
							return false;
						}
					}
				}
			}
			if (hNet->CloseClientIOCPHandle() == false)
			{
				return false;
			}
		}
		if (nCountThread == 0)
		{
			if (m_sSendSet.m_handle != NULL)
			{
				iState = WaitForSingleObject(m_sSendSet.m_handle,5000);
				if (iState == WAIT_OBJECT_0 || iState == WAIT_TIMEOUT)
				{
					if (iState == WAIT_TIMEOUT)
					{
						_endthreadex(m_sSendSet.m_uID);
					}
					//得到线程退出代码
					GetExitCodeThread(m_sSendSet.m_handle,&ExitCode);
					if (ExitCode == 0)
					{
						printf("发送线程停止\n");
						if (m_sAcceptSet.m_handle != NULL)
						{
							//等待线程结束
							iState = WaitForSingleObject(m_sAcceptSet.m_handle,5000);
							if (iState == WAIT_OBJECT_0 || iState == WAIT_TIMEOUT)
							{
								if (iState == WAIT_TIMEOUT)
								{
									TerminateThread(m_sAcceptSet.m_handle,ExitCode);
								}			
								//得到线程退出代码
								if (ExitCode == 0)
								{
									printf("接入监听线程停止\n");
									if (hNet != NULL)
									{
										printf("所有服务线程全部停止\n");
										return hNet->CloseListen();
									}
									else
									{
										return false;
									}
								}
								else
								{
									return false;
								}
							}
							else
							{
								return false;
							}
						}
						else
						{
							return false;
						}
					}
					else
						return false;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}
bool CTcpModuleFramework::InitClient(void)
{
	CTcpBaseModule *	hNet =  CTcpBaseModule::GetTcpBaseModule();
	if (hNet != NULL)
	{
		if(hNet->StartSocket() == true)
		{
			if (hNet->CreateIOCPClient(NULL) == -1)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}
bool CTcpModuleFramework::StartClient(void)
{
	SYSTEM_INFO					stSysInfo;
	int							nCount;
	GetSystemInfo(&stSysInfo);
	m_hFramework->m_iLoop = 1;
	m_bClientFlag = true;
	for (nCount = 0; nCount < (int)stSysInfo.dwNumberOfProcessors * 2; ++nCount)
	{
		m_stSetClient[nCount].psa.nLength = sizeof(m_stSetClient[nCount].psa);											
		m_stSetClient[nCount].psa.lpSecurityDescriptor = NULL;
		m_stSetClient[nCount].psa.bInheritHandle = TRUE;
		m_stSetClient[nCount].m_iSleep = 16;
		m_stSetClient[nCount].m_handle=(HANDLE)_beginthreadex(&m_stSetClient[nCount].psa,0,NetThreadClient,this,nCount,&m_stSetClient[nCount].m_uID);
		if (m_stSetClient[nCount].m_handle!=NULL)
		{

			continue;
		}
		else
		{
			break;
		}
	}
	if (nCount == stSysInfo.dwNumberOfProcessors * 2)
	{
		m_sSendSet.psa.nLength = sizeof(m_sSendSet.psa);											
		m_sSendSet.psa.lpSecurityDescriptor = NULL;
		m_sSendSet.psa.bInheritHandle = TRUE;
		m_sSendSet.m_iSleep = 30;
		m_sSendSet.m_handle=(HANDLE)_beginthreadex(&m_sSendSet.psa,0,SendThread,this,0,&m_sSendSet.m_uID);
		if (m_sSendSet.m_handle!=NULL)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
		return false;
}
bool CTcpModuleFramework::ConnectServer(const char * cIPAddr,int nPort)
{
	SOCKET								hSocket;
	GUIDKEY								nServerGUID;
	CTcpBaseModule *	hNet =  CTcpBaseModule::GetTcpBaseModule();
	if (hNet != NULL)
	{
		if (hNet->CreateSocketListen(LT_TCPClient) == true)
		{
			hSocket = hNet->ConnectOtherServer(cIPAddr,nPort);
			if (hSocket != NULL)
			{
				nServerGUID = hNet->CreateIOCPClient(hSocket);
				if ( nServerGUID != -1)
				{
					stTCPLinkIO * pServerIO = new stTCPLinkIO;
					if (pServerIO != NULL)
					{
						{
							CTcpClientVectorCriticalSection			hCSectionVector;
							pServerIO->nGuid = nServerGUID;
							m_vServerIO.push_back(pServerIO);
						}
						this->UpdataServer();
						if (hNet->ReceiveMsg(nServerGUID,LT_TCPClient) == true)
						{
							return true;		
						}
						else
						{
							return false;
						}
					}
					else
						return false;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}
vector<GUIDKEY> CTcpModuleFramework::GetClientID(void)
{
	vector<GUIDKEY>							vResult;
	{
		CTcpServerVectorCriticalSection				hCSectionVector;
		vResult.clear();
		for (int nCount = 0; nCount < (int)m_vClientIO.size(); ++nCount)
		{

			vResult.push_back(m_vClientIO[nCount]->nGuid);
		}
		return vResult;
	}
		
}
vector<GUIDKEY> CTcpModuleFramework::GetServerID(void)
{
	vector<GUIDKEY>							vResult;
	{
		CTcpClientVectorCriticalSection				hCSectionVector;
		vResult.clear();
		for (int nCount = 0; nCount < (int)m_vServerIO.size(); ++nCount)
		{
			vResult.push_back(m_vServerIO[nCount]->nGuid);
		}
		return vResult;
	}

}
vector<string> CTcpModuleFramework::RecvServerMsg(GUIDKEY nClientID)
{
	vector<string>						vResult;
	int									nClientPos;
	nClientPos = this->FindClient(nClientID);
	if (nClientPos > -1)
	{
		{
			CTcpServerVectorCriticalSection						hCSectionVector;
			vResult.clear();
			vResult = m_vClientIO[nClientPos]->vRecv;
			m_vClientIO[nClientPos]->vRecv.clear();
			return vResult;
		}
		
	}
	else
	{
		vResult.clear();
		return vResult;
	}
}
void CTcpModuleFramework::CallRecvServerMsg(GUIDKEY nClientID,NetCallBack pCallbackServer)
{
	int									nClientPos;
	nClientPos = this->FindClient(nClientID);
	if (nClientPos > -1)
	{
		{
			CTcpServerVectorCriticalSection						hCSectionVector;
			if(m_vClientIO[nClientPos]->vRecv.empty() == false)
			{
				pCallbackServer(m_vClientIO[nClientPos]->vRecv);
				m_vClientIO[nClientPos]->vRecv.clear();
			}
		}

	}
}
vector<string> CTcpModuleFramework::RecvClientMsg(GUIDKEY nServerID)
{
	vector<string>						vResult;
	int									nServerPos;
	nServerPos = this->FindServer(nServerID);
	if (nServerPos > -1)
	{
		{
			CTcpClientVectorCriticalSection					hCSectionVector;
			vResult.clear();
			vResult = m_vServerIO[nServerPos]->vRecv;
			m_vServerIO[nServerPos]->vRecv.clear();
			return vResult;
		}

	}
	else
	{
		vResult.clear();
		return vResult;
	}
}
void CTcpModuleFramework::CallRecvClientMsg(GUIDKEY nServerID,NetCallBack pCallbackClient)
{
	int									nServerPos;
	nServerPos = this->FindServer(nServerID);
	if (nServerPos > -1)
	{
		{
			CTcpClientVectorCriticalSection					hCSectionVector;
			if(m_vServerIO[nServerPos]->vRecv.empty() == false)
			{
				pCallbackClient(m_vServerIO[nServerPos]->vRecv);
				m_vServerIO[nServerPos]->vRecv.clear();
			}
		}

	}
}
bool CTcpModuleFramework::SendServerMsg(GUIDKEY nClientID,vector<string> vSend)
{
	int									nClientPos;
	CTcpBaseModule *	hNet =  CTcpBaseModule::GetTcpBaseModule();
	if (hNet != NULL)
	{
		nClientPos = this->FindClient(nClientID);
		if (nClientPos > -1)
		{
			{
				CTcpServerVectorCriticalSection				hCSectionVector;
				bool bPut = false;
				{
					vector<stTCPLinkIO*>::iterator iter = m_vClientIO.begin();
					for (; iter != m_vClientIO.end(); iter++)
					{
						if ((*iter)->nGuid == nClientID)
						{
							std::vector<stTCPSendStruct*> vSendSt = (*iter)->vSendSt;
							if (vSendSt.size() > 0){
								stTCPSendStruct* vSendStItem = vSendSt[0];
								if (vSendStItem){
									vSendStItem->vSend.insert(vSendStItem->vSend.end(), vSend.begin(), vSend.end());
									bPut = true;
								}
							}
							break;
						}
					}
				}
				if (!bPut) {
					stTCPSendStruct *	pSend = new stTCPSendStruct;
					if (pSend != NULL)
					{
						pSend->nTargetGuid = nClientID;
						pSend->nSendFleg = LT_TCPServer;
						pSend->vSend = vSend;
						m_vClientIO[nClientPos]->vSendSt.push_back(pSend);
					}
				}
				
			}
			return true;
		}
		else
			return false;
	}
	else
		return false;
}
bool CTcpModuleFramework::SendClientMsg(GUIDKEY nServerID,vector<string> vSend)
{
	int									nServerPos;
	CTcpBaseModule *	hNet =  CTcpBaseModule::GetTcpBaseModule();
	if (hNet != NULL)
	{
		nServerPos = this->FindServer(nServerID);
		if (nServerPos > -1)
		{
			{
				CTcpClientVectorCriticalSection				hCSectionVector;
				stTCPSendStruct *	pSend = new stTCPSendStruct;
				if (pSend != NULL)
				{
					pSend->nTargetGuid = nServerID;
					pSend->nSendFleg = LT_TCPClient;
					pSend->vSend = vSend;
					m_vServerIO[nServerPos]->vSendSt.push_back(pSend);
				}
			}
			return true;
		}
		else
			return false;
	}
	else
		return false;

}
bool CTcpModuleFramework::CloseServerClient(GUIDKEY nClientID)
{
	int							nClientPos;
	CTcpBaseModule *	hNet =  CTcpBaseModule::GetTcpBaseModule();
	if (hNet != NULL)
	{
		if (hNet->CloseSocket(nClientID,LT_TCPServer,0) == true)
		{
			nClientPos = this->FindClient(nClientID);
			if (nClientPos > -1)
			{
				{
					CTcpServerVectorCriticalSection			m_hCSectionVector;
					m_vClientIO[nClientPos]->vRecv.clear();
					delete m_vClientIO[nClientPos];
					m_vClientIO.erase(m_vClientIO.begin() + nClientPos);
				}
				this->UpdataClient();
				return true;
			}
			else
				return false;
		}
		else
			return false; 
	}
	else
		return false;
}
bool CTcpModuleFramework::CloseClient(GUIDKEY nServerID)
{
	int							nServerPos;
	CTcpBaseModule *	hNet =  CTcpBaseModule::GetTcpBaseModule();
	if (hNet != NULL)
	{
		if (hNet->CloseSocket(nServerID,LT_TCPClient,0) == true)
		{
			nServerPos = this->FindServer(nServerID);
			if (nServerPos > -1)
			{
				{
					CTcpClientVectorCriticalSection			m_hCSectionVector;
					m_vServerIO[nServerPos]->vRecv.clear();
					delete m_vServerIO[nServerPos];
					m_vServerIO.erase(m_vServerIO.begin() + nServerPos);
				}
				this->UpdataServer();
				return true;
			}
			else
				return false;
		}
		else
			return false; 
	}
	else
		return false;
}
bool CTcpModuleFramework::IsExistGUID(GUIDKEY nLinkID)
{
	int							m_nLinkPos;
	m_nLinkPos = this->FindClient(nLinkID);
	if (m_nLinkPos > -1)
		return true;
	else
	{
		m_nLinkPos = this->FindServer(nLinkID);
		if (m_nLinkPos > -1)
			return true;
		else
			return false;
	}
}
CTcpModuleInterface * CreateTcpModule()
{
	CTcpModuleInterface * hHandle;
	hHandle = static_cast<CTcpModuleInterface *>(CTcpModuleFramework::GetTcpModuleFrameworkHandle());
	if(hHandle != NULL)
	{
		return hHandle;
	}
	else
		return NULL;
}