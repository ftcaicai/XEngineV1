// GateServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../Common/ExportModuleInterface.h"
#include "../Interface/Protocol_logic_Interface.h"

#include <signal.h> 
#include "../MySQL/CMysql.h"
#include "../PlayerMgr/PlayerMgr.h"
#include "../HarryMgr/HarryMgr.h"
#include "../CSVData/CsvReaderMgr.h"


typedef	CExportModuleInterface * (* TcpDataModule)(void);
HINSTANCE						g_hinstLib;					//DLL实例
TcpDataModule					g_hModule;
CExportModuleInterface	*		g_hTcpModule;

vector<GUIDKEY>					g_vClientID;
CProtocolLogicInterface *		g_hProtocol;
vector<string>					g_vRecvMsg;
GUIDKEY							g_nSendClientID;
vector<string>					g_vSendMsg;
GUIDKEY							g_nDisconnectID;

bool g_bExit = false;


void SignalHandler(int sig)
{
	if(sig == SIGINT)
	{
		g_bExit = true;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	g_hinstLib=LoadLibraryA("XEngine");
	if (g_hinstLib!=NULL)
	{
		g_hModule=(TcpDataModule)GetProcAddress(g_hinstLib,"CreateExportHandle");
		if (g_hModule!=NULL)
		{
			signal(SIGINT, SignalHandler);

			CCsvReaderMgr::CreateInstance();
			if(! CCsvReaderMgr::Instance()->Init())
			{
				printf("CCsvReaderMgr init failed");
			}

			CMysql::CreateInstance();
			if(!CMysql::Instance()->Init())
			{
				printf("CMysql init failed.\n");
				//goto release;		//kakassi ignore 
			}

			CPlayerMgr::CreateInstance();
			if (!CPlayerMgr::Instance()->Init())
			{
				printf("CPlayerMgr init failed.\n");
				//goto release;		//kakassi ignore 
			}

			CHarryMgr::CreateInstance();
			if (!CHarryMgr::Instance()->Init())
			{
				printf("CHarryMgr init failed.\n");
				//goto release;		//kakassi ignore 
			}
			g_hTcpModule = g_hModule();
			g_hTcpModule->SetDebugOn(false);
			g_hTcpModule->CreateTcp();
			g_hTcpModule->InitServer(4500,3000);
			g_hTcpModule->StartServer();
			g_vClientID.clear();
			g_hProtocol = CreateProtocolLogic();
			while(true)
			{
				g_vClientID = g_hTcpModule->GetClientID();
				for(int nCount = 0; nCount < (int)g_vClientID.size(); ++nCount)
				{
					if(g_hProtocol)
					{
						///There is on disconnect
						if(g_hTcpModule->IsExistGUID(g_vClientID[nCount]) == false)
						{
							g_hProtocol->OnDisconnected(g_vClientID[nCount]);
							g_vClientID.erase(g_vClientID.begin() + nCount);
						}
						else
						{
							///There is receive the msg from the cleint
							g_vRecvMsg = g_hTcpModule->RecvServerMsg(g_vClientID[nCount]);
							if(g_vRecvMsg.empty() == false)
								g_hProtocol->OnRecv(g_vClientID[nCount],g_vRecvMsg);
						}
						///There is send the msg by server logic.
						for(int nCount1 = 0; nCount1 < g_hProtocol->GetSendMsgCount(); ++nCount1)
						{
							g_hProtocol->GetSendMsg(nCount1,g_nSendClientID,g_vSendMsg);
							g_hTcpModule->SendServerMsg(g_nSendClientID,g_vSendMsg);
						}
						g_hProtocol->ClearSendMsg();
						///There is disconnect the client by server logic.
						for(int nCount1 = 0; nCount1 < g_hProtocol->GetDisconnectClientIDCount(); ++nCount1)
						{
							g_nDisconnectID = g_hProtocol->GetDisconnectClientID(nCount1);
							g_hTcpModule->CloseClient(g_nDisconnectID);
							for(int nCount2 = 0; nCount2 < (int)g_vClientID.size(); ++nCount2)
							{
								if(g_vClientID[nCount2] == g_nDisconnectID)
								{
									g_vClientID.erase(g_vClientID.begin() + nCount2);
								}
							}
						}
						g_hProtocol->ClearDisconnectClientID();
						
						
					}
				}
				CPlayerMgr::Instance()->ProcessPlayerMgr();	
				Sleep(16);
			}
		}
	}
	return 0;
}

