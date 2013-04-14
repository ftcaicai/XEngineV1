// TestServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../Common/ExportModuleInterface.h"
#include <ctime>
#include "Interface/Protocol_logic_Interface.h"

typedef	CExportModuleInterface * (* TcpDataModule)(void);
HINSTANCE						g_hinstLib;					//DLL实例
TcpDataModule					g_hModule;
CExportModuleInterface	*		g_hTcpModule;
CProtocolLogicInterface *		g_hProtocol;

vector<GUIDKEY>					g_vClientID;
vector<string>					g_vRecvMsg;
GUIDKEY							g_nSendClientID;
vector<string>					g_vSendMsg;
GUIDKEY							g_nDisconnectID;

int _tmain(int argc, _TCHAR* argv[])
{
	//g_hinstLib=LoadLibraryA("XEngine");
	//if (g_hinstLib!=NULL)
	{
		//g_hModule=(TcpDataModule)GetProcAddress(g_hinstLib,"CreateExportHandle");
		//if (g_hModule!=NULL)
		{
			g_hTcpModule = CreateExportHandle();
			g_hTcpModule->SetDebugOn(true);
			g_hTcpModule->CreateTcp();
			g_hTcpModule->InitServer(4501,3000);
			g_hTcpModule->StartServer();
			g_vClientID.clear();
			g_hProtocol = CreateProtocolLogic();
			while(true)
			{
				g_vClientID = g_hTcpModule->GetClientID();
				for(int nCount = 0; nCount < (int)g_vClientID.size(); ++nCount)
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
				Sleep(10);
			}
		}
	}

	return 0;
}

