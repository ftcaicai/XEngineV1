#ifndef __TCP_BASE_MODULE_H__
#define __TCP_BASE_MODULE_H__
#include "DebugModuleInterface.h"

#define OP_NONE	  0
#define OP_READ   1 
#define OP_WRITE  2 
#define OP_CLOSE  3
#define OP_STOP	  4

enum ListenTCPType
{
	LT_TCPServer,
	LT_TCPClient,
};

typedef struct IOCPIO
{
	OVERLAPPED							ol;										// 重叠结构 
	char								cRecvMsg[MAXNETSIZE];					// 接受到的数据
	
}stIOCPIO;
///服务器连接客户端结构
typedef struct IOCPHandleServer
{
	GUIDKEY								nGuid;									// GUID号
	SOCKET								hSocket;								// 网络句柄;
	sockaddr_in							stAddr;									// 对方的地址
	HANDLE								hIOCPHandle;							// IOCP句柄
}stIOCPHandleServer;
///客户端连接服务器构构
typedef struct IOCPHandleClient
{
	GUIDKEY								nGuid;									// GUID号
	SOCKET								hSocket;								// 网络句柄;
	HANDLE								hIOCPHandle;							// IOCP句柄
}stIOCPHandleClient;
///服务器连接客户端结构
typedef struct IOCPClientLinkHandle
{
	stIOCPHandleServer *				pClientHandle;							// 客户端句柄
	stIOCPIO	 *						pClientType;							// 客户端操作类型
}stIOCPClientLinkHandle;
///客户端连接服务器结构
typedef struct IOCPServerLinkHandle
{
	stIOCPHandleClient *				pServerHandle;							// 客户端句柄
	stIOCPIO	 *						pServerType;							// 客户端操作类型
}stIOCPServerLinkHandle;

class CTcpBaseModule
{
public:
	static CTcpBaseModule * GetTcpBaseModule(void);
	void ReleaseTcpBaseModule(void);
	///点火启动WinSocket
	bool StartSocket(void);
	///建立WinSocket监听
	bool CreateSocketListen(int nListenType);
	///绑定端口(服务器)
	bool BindSocket(int Port);
	///监听端口(服务器)
	bool ListenSocket(int nListen);
	///接入客户端
	SOCKET LinkClient(SOCKADDR_IN * pAddr);
	///连接其它服务器
	SOCKET ConnectOtherServer(const char * IPAddr,int Port);
	///接收数据
	bool ReceiveMsg(GUIDKEY nLinkID,int nType);
	///发送消息
	bool SendMsg(GUIDKEY nLinkID,vector<string> vSendMsg,int nType);
	///发送Flash连接
	bool SendFlashMsg(GUIDKEY nLinkID);
	///检查当前IOCP连接状态，服务器专用
	GUIDKEY GetMsgIOCPServer(int & nType,vector<string> &vMsg);
	///检查当前IOCP连接状态，客户端专用
	GUIDKEY GetMsgIOCPClient(int & nType,vector<string> &vMsg);
	///关闭连接
	bool CloseSocket(GUIDKEY nLinkID,int nType,int nErrorCode);
	///IOCP监听，服务器专用
	GUIDKEY CreateIOCPServer(SOCKET hClient,SOCKADDR_IN * pAddr);
	///IOCP监听，客户端专用
	GUIDKEY CreateIOCPClient(SOCKET hConnect);
	///关闭监听
	bool CloseListen(void);
	///断开所有服务器上客户端IOCP线程
	bool CloseAllServerIOCP(void);
	///断开所有客户端上客户端IOCP线程
	bool CloseAllClientIOCP(void);
	///停止所有服务器IOCP句柄活动
	bool StopServerIOCPHandle(void);
	///停止所有客户端IOCP句柄活动
	bool StopClientIOCPHandle(void);
	///关闭所有服务器IOCP句柄
	bool CloseServerIOCPHandle(void);
	///关闭所有客户端IOCP句柄
	bool CloseClientIOCPHandle(void);
private:
	CTcpBaseModule(void);
	~CTcpBaseModule(void);

	///查找一个客户端的信息
	int FindClient(GUIDKEY nClientID);
	///更新客户端信息
	void UpdataClient(void);
	///查找一个服务器的信息
	int FindServer(GUIDKEY nServerID);
	///更新服务器信息
	void UpdataServer(void);

	static  CTcpBaseModule *						m_hTCPHandle;

	CDebugModuleInterface *							m_hDebugModule;

	SOCKET											m_hListen;
	SOCKET											m_hConnect;
	HANDLE											m_hIOCPHandleServer;
	HANDLE											m_hIOCPHandleClient;

	///对方以服务器形式，我方以客户端形式
	vector<stIOCPServerLinkHandle *>				m_vServer;
	map<GUIDKEY,int>									m_mServerMap;
	///对方以客户端形式，我方以服务器形式
	vector<stIOCPClientLinkHandle *>				m_vClient;
	map<GUIDKEY,int>									m_mClientMap;

};
///临界区
///服务器端Vector临界区，Framework层的锁
class CTcpServerVectorCriticalSection
{
public:
	CTcpServerVectorCriticalSection(void);
	~CTcpServerVectorCriticalSection(void);
};
///客户端Vector临界区，Framework层的锁
class CTcpClientVectorCriticalSection
{
public:
	CTcpClientVectorCriticalSection(void);
	~CTcpClientVectorCriticalSection(void);
};
///服务器Vector监界区，IOCP层的锁
class CTcpServerIOCPVectorCriticalSection
{
public:
	CTcpServerIOCPVectorCriticalSection(void);
	~CTcpServerIOCPVectorCriticalSection(void);
};
///客户端Vector监界区，IOCP层的锁
class CTcpClientIOCPVectorCriticalSection
{
public:
	CTcpClientIOCPVectorCriticalSection(void);
	~CTcpClientIOCPVectorCriticalSection(void);
};
///多线程同步锁
///服务器
class CTcpServerThreadCriticalSection
{
public:
	CTcpServerThreadCriticalSection(void);
	~CTcpServerThreadCriticalSection(void);
};
///客户端
class CTcpClientThreadCriticalSection
{
public:
	CTcpClientThreadCriticalSection(void);
	~CTcpClientThreadCriticalSection(void);
};
#endif
