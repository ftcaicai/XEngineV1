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
///���ӽṹ��
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
	///�ͷ�TCPģ��
	void ReleaseTcpModuleFrameworkHandle(void);
	///��ʼ������
	bool InitServer(int nPort,int nListen);
	///�����Ƿ���Flash��֤��Ϣ
	bool SetSendFlashMsg(int nFlag);
	///��������
	bool StartServer(void);
	///�رշ����Լ�����
	bool StopService(void);

	///��ʼ������
	bool InitClient(void);
	///�����ͻ���
	bool StartClient(void);
	///���ӷ�����
	bool ConnectServer(const char * cIPAddr,int nPort);

	///���������ӿͻ���
	vector<GUIDKEY> GetClientID(void);
	///���������ӷ����
	vector<GUIDKEY> GetServerID(void);
	///�����յ�����Ϣ���������ˣ�
	vector<string> RecvServerMsg(GUIDKEY nClientID);
	///���ûص������յ�����Ϣ���������ˣ�
	void CallRecvServerMsg(GUIDKEY nClientID,NetCallBack pCallbackServer);
	///�����յ�����Ϣ���ͻ��ˣ�
	vector<string> RecvClientMsg(GUIDKEY nServerID);
	///���ûص������յ�����Ϣ���ͻ��ˣ�
	void CallRecvClientMsg(GUIDKEY nServerID,NetCallBack pCallbackClient);
	///�������˷������ݸ��ض��ͻ���
	bool SendServerMsg(GUIDKEY nClientID,vector<string> vSend);
	///�ͻ��˷������ݸ��ض�������
	bool SendClientMsg(GUIDKEY nServerID,vector<string> vSend);
	///�ر�ĳ���ͻ���
	bool CloseServerClient(GUIDKEY nClientID);
	///ĳ���ͻ��˶Ͽ�
	bool CloseClient(GUIDKEY nServerID);
	///GUID�Ƿ���Ч
	bool IsExistGUID(GUIDKEY nLinkID);



private:
	CTcpModuleFramework(void);
	~CTcpModuleFramework(void);
	unsigned int NetLoopServer(void);												//�̵߳�ѭ���ṹ
	unsigned int NetLoopClient(void);												//�̵߳�ѭ���ṹ
	unsigned int AcceptLoop(void);													//�����߳�
	unsigned int SendLoop(void);													//���������߳�
	static unsigned _stdcall NetThreadServer(void * Param);							//����ѭ���ṹ�ĺ���
	static unsigned _stdcall NetThreadClient(void * Param);							//����ѭ���ṹ�ĺ���
	static unsigned _stdcall AcceptThread(void * Param);							//����ѭ���ṹ�ĺ���
	static unsigned _stdcall SendThread(void * Param);								//����ѭ���ṹ�ĺ���

	int FindClient(GUIDKEY nClientID);												///����һ���ͻ��˵���Ϣ
	void UpdataClient(void);														///���¿ͻ�����Ϣ
	int FindServer(GUIDKEY nServerID);												///����һ������������Ϣ
	void UpdataServer(void);														///���·�������Ϣ
	///�߳�����
	typedef struct ThreadSetting
	{
		//�߳�������
		SECURITY_ATTRIBUTES					psa;									//��ȫ�Ե�����
		HANDLE								m_handle;								//�߳̾��
		unsigned							m_uID;									//�̵߳ı�־ֵ


		DWORD								m_iSleep;								//������
	}stThreadSetting;
	stThreadSetting 						m_stSetServer[50];						//���������߳̾��
	bool									m_bServerFlag;							//������������־
	stThreadSetting							m_stSetClient[50];						//�ͻ����߳̾��
	bool									m_bClientFlag;							//�ͻ���������־

	
	int										m_iSendFlashMsg;						//����Flash��Ϣ
	stThreadSetting							m_sAcceptSet;							//�����߳�����
	stThreadSetting							m_sSendSet;								//�����߳�����

	static CTcpModuleFramework *			m_hFramework;							//��ܾ��

	///�ҷ��Է���������ʽ���Է��Կͻ��˵���ʽ
	map<GUIDKEY,int>						m_mClientIO;							//��������������
	vector<stTCPLinkIO *>					m_vClientIO;

	///�ҷ��Կͻ��˵���ʽ���Է��Է�������ʽ
	map<GUIDKEY,int>						m_mServerIO;							//�ͻ��˽�������
	vector<stTCPLinkIO *>					m_vServerIO;

	int										m_iHandleCount;
	int										m_iLoop;								//ѭ������
};

#endif
