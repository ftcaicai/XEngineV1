#ifndef __TCP_MODULE_INTERFACE_H__
#define __TCP_MODULE_INTERFACE_H__
#include "System_Header.h"
#include "../Common/ExportModuleInterface.h"
class CTcpModuleInterface
{
public:
	///�ͷ�TCPģ��
	virtual void ReleaseTcpModuleFrameworkHandle(void) = 0;
	///��ʼ������
	virtual bool InitServer(int nPort,int nListen) = 0;
	///�����Ƿ���Flash��֤��Ϣ
	virtual bool SetSendFlashMsg(int nFlag) = 0;
	///��������
	virtual bool StartServer(void) = 0;
	///�رշ����Լ�����
	virtual bool StopService(void) = 0;

	///��ʼ������
	virtual bool InitClient(void) = 0;
	///�����ͻ���
	virtual bool StartClient(void) = 0;
	///���ӷ�����
	virtual bool ConnectServer(const char * cIPAddr,int nPort) = 0;

	///���������ӿͻ���
	virtual vector<GUIDKEY> GetClientID(void) = 0;
	///���������ӷ����
	virtual vector<GUIDKEY> GetServerID(void) = 0;
	///�����յ�����Ϣ���������ˣ�
	virtual vector<string> RecvServerMsg(GUIDKEY nClientID) = 0;
	///���ûص������յ�����Ϣ���������ˣ�
	virtual void CallRecvServerMsg(GUIDKEY nClientID,NetCallBack pCallbackServer) = 0;
	///�����յ�����Ϣ���ͻ��ˣ�
	virtual vector<string> RecvClientMsg(GUIDKEY nServerID) = 0;
	///���ûص������յ�����Ϣ���ͻ��ˣ�
	virtual void CallRecvClientMsg(GUIDKEY nServerID,NetCallBack pCallbackClient) = 0;
	///�������˷������ݸ��ض��ͻ���
	virtual bool SendServerMsg(GUIDKEY nClientID,vector<string> vSend) = 0;
	///�ͻ��˷������ݸ��ض�������
	virtual bool SendClientMsg(GUIDKEY nServerID,vector<string> vSend) = 0;
	///�ر�ĳ���ͻ���
	virtual bool CloseServerClient(GUIDKEY nClientID) = 0;
	///ĳ���ͻ��˶Ͽ�
	virtual bool CloseClient(GUIDKEY nServerID) = 0;
	///GUID�Ƿ���Ч
	virtual bool IsExistGUID(GUIDKEY nLinkID) = 0;
};
CTcpModuleInterface * CreateTcpModule();
#endif