#ifndef __TCP_MODULE_INTERFACE_H__
#define __TCP_MODULE_INTERFACE_H__
#include "System_Header.h"
#include "../Common/ExportModuleInterface.h"
class CTcpModuleInterface
{
public:
	///释放TCP模块
	virtual void ReleaseTcpModuleFrameworkHandle(void) = 0;
	///初始化网络
	virtual bool InitServer(int nPort,int nListen) = 0;
	///设置是否发送Flash认证消息
	virtual bool SetSendFlashMsg(int nFlag) = 0;
	///启动服务
	virtual bool StartServer(void) = 0;
	///关闭服务以及网络
	virtual bool StopService(void) = 0;

	///初始化网格
	virtual bool InitClient(void) = 0;
	///启动客户端
	virtual bool StartClient(void) = 0;
	///连接服务器
	virtual bool ConnectServer(const char * cIPAddr,int nPort) = 0;

	///返回已连接客户端
	virtual vector<GUIDKEY> GetClientID(void) = 0;
	///返回已连接服务端
	virtual vector<GUIDKEY> GetServerID(void) = 0;
	///返回收到的消息（服务器端）
	virtual vector<string> RecvServerMsg(GUIDKEY nClientID) = 0;
	///调用回调处理收到的消息（服务器端）
	virtual void CallRecvServerMsg(GUIDKEY nClientID,NetCallBack pCallbackServer) = 0;
	///返回收到的消息（客户端）
	virtual vector<string> RecvClientMsg(GUIDKEY nServerID) = 0;
	///调用回调处理收到的消息（客户端）
	virtual void CallRecvClientMsg(GUIDKEY nServerID,NetCallBack pCallbackClient) = 0;
	///服务器端发送数据给特定客户端
	virtual bool SendServerMsg(GUIDKEY nClientID,vector<string> vSend) = 0;
	///客户端发送数据给特定服务器
	virtual bool SendClientMsg(GUIDKEY nServerID,vector<string> vSend) = 0;
	///关闭某个客户端
	virtual bool CloseServerClient(GUIDKEY nClientID) = 0;
	///某个客户端断开
	virtual bool CloseClient(GUIDKEY nServerID) = 0;
	///GUID是否有效
	virtual bool IsExistGUID(GUIDKEY nLinkID) = 0;
};
CTcpModuleInterface * CreateTcpModule();
#endif