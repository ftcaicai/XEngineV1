// TestClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../Common/ExportModuleInterface.h"
#include <ctime>

typedef	CExportModuleInterface * (* TcpDataModule)(void);
HINSTANCE						g_hinstLib;					//DLL实例
TcpDataModule					g_hModule;
CExportModuleInterface	*		g_hTcpModule;
vector<GUIDKEY>					g_vServerID;

int								g_nConnect = 1000;

char							g_cMsg[1024];

vector<string>					g_vPassword;
vector<string>					g_vAccount;

vector<string>					g_vRegAccount;
vector<string>					g_vLoginAccount;
string							g_sReqPlayer;
bool							g_bExit = true;
string							g_sInput;

vector<int>						g_vTime;
time_t							g_tmStart,g_tmEnd;

typedef struct LoginReg
{
	int					nLen;
	int					nMsgID;
	int					nPassword;
	char  				cPassword[16];
	int					nAccount;
	char  				cAccount[16];
}stLoginReg;
typedef struct LoginIn
{
	int					nLen;
	int					nMsgID;
	int					nPassword;
	char				cPassword[16];
	int					nAccount;
	char				cAccount[16];
}stLoginIn;
typedef struct ReqPlayerInfo
{
	int					nLen;
	int					nMsgID;
}stReqPlayerInfo;
void ReqPlayerInfo(void)
{
	stReqPlayerInfo				stReq;
	stReq.nMsgID = 5;
	stReq.nLen = sizeof(stReq) - 4;
	memcpy(g_cMsg,&stReq,sizeof(stReq));

	g_sReqPlayer.clear();
	g_sReqPlayer.append(g_cMsg,sizeof(stReq));
}
void RegAccount(void)
{
	for(int nCount = 0; nCount < (int)g_vAccount.size(); ++nCount)
	{
		stLoginReg				stReglogin;
		string					sProtocol;
		stReglogin.nMsgID = 2;
		memset(stReglogin.cAccount,'\0',sizeof(stReglogin.cAccount));
		memset(stReglogin.cPassword,'\0',sizeof(stReglogin.cPassword));
		strcpy(stReglogin.cAccount,g_vAccount[nCount].c_str());
		strcpy(stReglogin.cPassword,g_vPassword[nCount].c_str());
		stReglogin.nPassword = sizeof(stReglogin.cPassword);
		stReglogin.nAccount = sizeof(stReglogin.cAccount);
		stReglogin.nLen = sizeof(stReglogin) - 4;
		memcpy(g_cMsg,&stReglogin,sizeof(stReglogin));
		
		sProtocol.clear();
		sProtocol.append(g_cMsg,sizeof(stReglogin));
		g_vRegAccount.push_back(sProtocol);
	}
}
void LoginAccount(void)
{
	for(int nCount = 0; nCount < (int)g_vAccount.size(); ++nCount)
	{
		stLoginIn					stLogin;
		string						sProtocol;
		stLogin.nMsgID = 3;
		memset(stLogin.cAccount,'\0',sizeof(stLogin.cAccount));
		memset(stLogin.cPassword,'\0',sizeof(stLogin.cPassword));
		strcpy(stLogin.cAccount,g_vAccount[nCount].c_str());
		strcpy(stLogin.cPassword,g_vPassword[nCount].c_str());
		stLogin.nPassword = sizeof(stLogin.cPassword);
		stLogin.nAccount = sizeof(stLogin.cAccount);
		stLogin.nLen = sizeof(stLogin) - 4;
		memcpy(g_cMsg,&stLogin,sizeof(stLogin));

		sProtocol.clear();
		sProtocol.append(g_cMsg,sizeof(stLogin));
		g_vLoginAccount.push_back(sProtocol);
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
			g_hTcpModule = g_hModule();
			g_hTcpModule->SetDebugOn(false);
			g_hTcpModule->CreateTcp();
			g_hTcpModule->InitClient();
			g_hTcpModule->StartClient();

			///create account and password
			for(int nCount = 0; nCount < g_nConnect; ++nCount)
			{
				char				cNumber[5];
				string				sAccount;
				string				sPassword;
				
				itoa(nCount,cNumber,10);
				sPassword = "test";
				sPassword += cNumber;
				sAccount = "test";
				sAccount += cNumber;

				g_vAccount.push_back(sAccount);
				g_vPassword.push_back(sPassword);
			}
			ReqPlayerInfo();
			while(g_bExit)
			{
				g_sInput = g_hTcpModule->GetDosCommand();
				if(strcmp(g_sInput.c_str(),"msg\n") == 0)
				{
					g_hTcpModule->ConnectServer("127.0.0.1",4500);
					g_vServerID = g_hTcpModule->GetServerID();

					vector<string>				vSendMsg;
					vSendMsg.clear();
					vSendMsg.push_back("xx");
					vSendMsg.push_back("YY");
					g_hTcpModule->SendClientMsg(g_vServerID[0],vSendMsg);
				}
				if(strcmp(g_sInput.c_str(),"reg\n") == 0)
				{
					RegAccount();
					for(int nCount = 0; nCount < g_nConnect; ++nCount)
					{
						printf("Client %d\n",nCount);
						g_hTcpModule->ConnectServer("127.0.0.1",4500);
					}
					g_vServerID = g_hTcpModule->GetServerID();
					for(int nCount = 0; nCount < (int)g_vServerID.size(); ++nCount)
					{
						vector<string>				vSendMsg;
						vSendMsg.clear();
						vSendMsg.push_back(g_vRegAccount[nCount]);
						g_hTcpModule->SendClientMsg(g_vServerID[nCount],vSendMsg);
					}
				}
				if(strcmp(g_sInput.c_str(),"login\n") == 0)
				{
					LoginAccount();
					g_vTime.clear();
					for(int nCount = 0; nCount < g_nConnect; ++nCount)
					{
						g_hTcpModule->ConnectServer("127.0.0.1",4500);
					}
					g_vServerID = g_hTcpModule->GetServerID();
					for(int nCount = 0; nCount<(int)g_vServerID.size(); ++nCount)
					{
						vector<string>				vSendMsg;
						g_tmStart = time(NULL);
						vSendMsg.clear();
						vSendMsg.push_back(g_vLoginAccount[nCount]);
						g_hTcpModule->SendClientMsg(g_vServerID[nCount],vSendMsg);
					}
					for(int nCount = 0; nCount<(int)g_vServerID.size(); ++nCount)
					{
						while(true)
						{
							vector<string>				vSendMsg;
							vector<string>				vRecv;
							vRecv.clear();
							vRecv = g_hTcpModule->RecvClientMsg(g_vServerID[nCount]);
							if(vRecv.empty() == false)
							{
								printf("Client %d\n",g_vServerID[nCount]);
								vSendMsg.clear();
								vSendMsg.push_back(g_sReqPlayer);
								g_hTcpModule->SendClientMsg(g_vServerID[nCount],vSendMsg);
								g_tmEnd = time(NULL);
								int				nTime;
								nTime = g_tmEnd - g_tmStart;
								g_vTime.push_back(nTime);
								printf("Complete!! %d\n",nCount);
								break;
							}
							
						}
					}
				}
				if(strcmp(g_sInput.c_str(),"result\n") == 0)
				{
					float							nAverage = 0;
					for(int nCount = 0; nCount < (int)g_vTime.size(); ++nCount)
					{
						printf("Login In %d = %d\n",nCount,g_vTime[nCount]);
						nAverage = nAverage + g_vTime[nCount];
					}
					nAverage = nAverage /(int)g_vTime.size();
					printf("Average %f\n",nAverage);
				}
				Sleep(16);
			}
			
			for(int nCount = 0; nCount < g_nConnect; ++nCount)
			{
				g_hTcpModule->CloseClient(g_vServerID[0]);
			}
			
		}
	}
	return 0;
}

