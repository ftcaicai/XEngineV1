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
	OVERLAPPED							ol;										// �ص��ṹ 
	char								cRecvMsg[MAXNETSIZE];					// ���ܵ�������
	
}stIOCPIO;
///���������ӿͻ��˽ṹ
typedef struct IOCPHandleServer
{
	GUIDKEY								nGuid;									// GUID��
	SOCKET								hSocket;								// ������;
	sockaddr_in							stAddr;									// �Է��ĵ�ַ
	HANDLE								hIOCPHandle;							// IOCP���
}stIOCPHandleServer;
///�ͻ������ӷ���������
typedef struct IOCPHandleClient
{
	GUIDKEY								nGuid;									// GUID��
	SOCKET								hSocket;								// ������;
	HANDLE								hIOCPHandle;							// IOCP���
}stIOCPHandleClient;
///���������ӿͻ��˽ṹ
typedef struct IOCPClientLinkHandle
{
	stIOCPHandleServer *				pClientHandle;							// �ͻ��˾��
	stIOCPIO	 *						pClientType;							// �ͻ��˲�������
}stIOCPClientLinkHandle;
///�ͻ������ӷ������ṹ
typedef struct IOCPServerLinkHandle
{
	stIOCPHandleClient *				pServerHandle;							// �ͻ��˾��
	stIOCPIO	 *						pServerType;							// �ͻ��˲�������
}stIOCPServerLinkHandle;

class CTcpBaseModule
{
public:
	static CTcpBaseModule * GetTcpBaseModule(void);
	void ReleaseTcpBaseModule(void);
	///�������WinSocket
	bool StartSocket(void);
	///����WinSocket����
	bool CreateSocketListen(int nListenType);
	///�󶨶˿�(������)
	bool BindSocket(int Port);
	///�����˿�(������)
	bool ListenSocket(int nListen);
	///����ͻ���
	SOCKET LinkClient(SOCKADDR_IN * pAddr);
	///��������������
	SOCKET ConnectOtherServer(const char * IPAddr,int Port);
	///��������
	bool ReceiveMsg(GUIDKEY nLinkID,int nType);
	///������Ϣ
	bool SendMsg(GUIDKEY nLinkID,vector<string> vSendMsg,int nType);
	///����Flash����
	bool SendFlashMsg(GUIDKEY nLinkID);
	///��鵱ǰIOCP����״̬��������ר��
	GUIDKEY GetMsgIOCPServer(int & nType,vector<string> &vMsg);
	///��鵱ǰIOCP����״̬���ͻ���ר��
	GUIDKEY GetMsgIOCPClient(int & nType,vector<string> &vMsg);
	///�ر�����
	bool CloseSocket(GUIDKEY nLinkID,int nType,int nErrorCode);
	///IOCP������������ר��
	GUIDKEY CreateIOCPServer(SOCKET hClient,SOCKADDR_IN * pAddr);
	///IOCP�������ͻ���ר��
	GUIDKEY CreateIOCPClient(SOCKET hConnect);
	///�رռ���
	bool CloseListen(void);
	///�Ͽ����з������Ͽͻ���IOCP�߳�
	bool CloseAllServerIOCP(void);
	///�Ͽ����пͻ����Ͽͻ���IOCP�߳�
	bool CloseAllClientIOCP(void);
	///ֹͣ���з�����IOCP����
	bool StopServerIOCPHandle(void);
	///ֹͣ���пͻ���IOCP����
	bool StopClientIOCPHandle(void);
	///�ر����з�����IOCP���
	bool CloseServerIOCPHandle(void);
	///�ر����пͻ���IOCP���
	bool CloseClientIOCPHandle(void);
private:
	CTcpBaseModule(void);
	~CTcpBaseModule(void);

	///����һ���ͻ��˵���Ϣ
	int FindClient(GUIDKEY nClientID);
	///���¿ͻ�����Ϣ
	void UpdataClient(void);
	///����һ������������Ϣ
	int FindServer(GUIDKEY nServerID);
	///���·�������Ϣ
	void UpdataServer(void);

	static  CTcpBaseModule *						m_hTCPHandle;

	CDebugModuleInterface *							m_hDebugModule;

	SOCKET											m_hListen;
	SOCKET											m_hConnect;
	HANDLE											m_hIOCPHandleServer;
	HANDLE											m_hIOCPHandleClient;

	///�Է��Է�������ʽ���ҷ��Կͻ�����ʽ
	vector<stIOCPServerLinkHandle *>				m_vServer;
	map<GUIDKEY,int>									m_mServerMap;
	///�Է��Կͻ�����ʽ���ҷ��Է�������ʽ
	vector<stIOCPClientLinkHandle *>				m_vClient;
	map<GUIDKEY,int>									m_mClientMap;

};
///�ٽ���
///��������Vector�ٽ�����Framework�����
class CTcpServerVectorCriticalSection
{
public:
	CTcpServerVectorCriticalSection(void);
	~CTcpServerVectorCriticalSection(void);
};
///�ͻ���Vector�ٽ�����Framework�����
class CTcpClientVectorCriticalSection
{
public:
	CTcpClientVectorCriticalSection(void);
	~CTcpClientVectorCriticalSection(void);
};
///������Vector�������IOCP�����
class CTcpServerIOCPVectorCriticalSection
{
public:
	CTcpServerIOCPVectorCriticalSection(void);
	~CTcpServerIOCPVectorCriticalSection(void);
};
///�ͻ���Vector�������IOCP�����
class CTcpClientIOCPVectorCriticalSection
{
public:
	CTcpClientIOCPVectorCriticalSection(void);
	~CTcpClientIOCPVectorCriticalSection(void);
};
///���߳�ͬ����
///������
class CTcpServerThreadCriticalSection
{
public:
	CTcpServerThreadCriticalSection(void);
	~CTcpServerThreadCriticalSection(void);
};
///�ͻ���
class CTcpClientThreadCriticalSection
{
public:
	CTcpClientThreadCriticalSection(void);
	~CTcpClientThreadCriticalSection(void);
};
#endif
