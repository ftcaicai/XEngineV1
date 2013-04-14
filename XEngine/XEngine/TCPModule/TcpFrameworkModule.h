#ifndef __TCP_FRAMEWORK_MODULE_H__
#define __TCP_FRAMEWORK_MODULE_H__
#include "TcpBaseModule.h"
#include "TcpModuleInterface.h"

typedef struct TCPSendStruct
{
	GUIDKEY								nTargetGuid;
	vector<string>						vSend;
	int									nSendFleg;
}stTCPSendStruct;
///连接结构体
typedef struct TCPLinkIO
{
	GUIDKEY								nGuid;
	vector<string>						vRecv;
	vector<stTCPSendStruct *>			vSendSt;
}stTCPLinkIO;


class CTcpModuleFramework : public CTcpModuleInterface
{
public:
	static CTcpModuleFramework * GetTcpModuleFrameworkHandle(void);
	///释放TCP模块
	void ReleaseTcpModuleFrameworkHandle(void);
	///初始化网络
	bool InitServer(int nPort,int nListen);
	///设置是否发送Flash认证消息
	bool SetSendFlashMsg(int nFlag);
	///启动服务
	bool StartServer(void);
	///关闭服务以及网络
	bool StopService(void);

	///初始化网格
	bool InitClient(void);
	///启动客户端
	bool StartClient(void);
	///连接服务器
	bool ConnectServer(const char * cIPAddr,int nPort);

	///返回已连接客户端
	vector<GUIDKEY> GetClientID(void);
	///返回已连接服务端
	vector<GUIDKEY> GetServerID(void);
	///返回收到的消息（服务器端）
	vector<string> RecvServerMsg(GUIDKEY nClientID);
	///调用回调处理收到的消息（服务器端）
	void CallRecvServerMsg(GUIDKEY nClientID,NetCallBack pCallbackServer);
	///返回收到的消息（客户端）
	vector<string> RecvClientMsg(GUIDKEY nServerID);
	///调用回调处理收到的消息（客户端）
	void CallRecvClientMsg(GUIDKEY nServerID,NetCallBack pCallbackClient);
	///服务器端发送数据给特定客户端
	bool SendServerMsg(GUIDKEY nClientID,vector<string> vSend);
	///客户端发送数据给特定服务器
	bool SendClientMsg(GUIDKEY nServerID,vector<string> vSend);
	///关闭某个客户端
	bool CloseServerClient(GUIDKEY nClientID);
	///某个客户端断开
	bool CloseClient(GUIDKEY nServerID);
	///GUID是否有效
	bool IsExistGUID(GUIDKEY nLinkID);



private:
	CTcpModuleFramework(void);
	~CTcpModuleFramework(void);
	unsigned int NetLoopServer(void);												//线程的循环结构
	unsigned int NetLoopClient(void);												//线程的循环结构
	unsigned int AcceptLoop(void);													//接入线程
	unsigned int SendLoop(void);													//发送数据线程
	static unsigned _stdcall NetThreadServer(void * Param);							//调用循环结构的函数
	static unsigned _stdcall NetThreadClient(void * Param);							//调用循环结构的函数
	static unsigned _stdcall AcceptThread(void * Param);							//调用循环结构的函数
	static unsigned _stdcall SendThread(void * Param);								//调用循环结构的函数

	int FindClient(GUIDKEY nClientID);												///查找一个客户端的信息
	void UpdataClient(void);														///更新客户端信息
	int FindServer(GUIDKEY nServerID);												///查找一个服务器的信息
	void UpdataServer(void);														///更新服务器信息
	///线程设置
	typedef struct ThreadSetting
	{
		//线程描述符
		SECURITY_ATTRIBUTES					psa;									//安全性的描述
		HANDLE								m_handle;								//线程句柄
		unsigned							m_uID;									//线程的标志值


		DWORD								m_iSleep;								//挂起间隔
	}stThreadSetting;
	stThreadSetting 						m_stSetServer[50];						//服务器端线程句柄
	bool									m_bServerFlag;							//服务器启动标志
	stThreadSetting							m_stSetClient[50];						//客户端线程句柄
	bool									m_bClientFlag;							//客户端启动标志

	
	int										m_iSendFlashMsg;						//发送Flash消息
	stThreadSetting							m_sAcceptSet;							//接收线程设置
	stThreadSetting							m_sSendSet;								//发送线程设置

	static CTcpModuleFramework *			m_hFramework;							//框架句柄

	///我方以服务器的形式，对方以客户端的形式
	map<GUIDKEY,int>						m_mClientIO;							//服务器接收数据
	vector<stTCPLinkIO *>					m_vClientIO;

	///我方以客户端的形式，对方以服务器形式
	map<GUIDKEY,int>						m_mServerIO;							//客户端接收数据
	vector<stTCPLinkIO *>					m_vServerIO;

	int										m_iHandleCount;
	int										m_iLoop;								//循环开关
};

#endif
