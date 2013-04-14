#ifndef __EXPORT_MODULE_H__
#define __EXPORT_MODULE_H__
#include "System_Header.h"
#include "ExportModuleInterface.h"
#include "DebugModuleInterface.h"
#include "ODBCModuleInterface.h"
#include "TcpModuleInterface.h"

class CExportModule : public CExportModuleInterface
{
public:
	///���ص������
	static CExportModule * GetExportModule(void);
	///����Ϣ�ռ�ϵͳ
	void SetDebugOn(bool bIsWindow);
	///�ر���Ϣ�ռ�ϵͳ
	void SetDebugOff(void);
	///������Ϣ�ռ�ϵͳ״̬
	int GetDebugState(void);
	/// �����Ϣ
	void AddDebugMsgInfo(int iMsg);
	///���ϵͳ������Ϣ
	void AddDebugErrorInfo(int iErrorCode);
	///����ַ���Ϣ
	void AddDebugStringInfo(const char * cValue);
	/// ������Ϣ������
	int GetDebugInfoCount(void);
	//��������
	int GetDebugType(int nNode);					
	//������Ϣ
	int GetDebugMsgCode(int nNode);
	//���صȼ�
	int GetDebugLevel(int nNode);
	//����ϵͳ�Ĵ������
	int GetDebugErrorCode(int nNode);
	//������ʾ
	const char * GetDebugMsg(int nNode);
	///���ĳһ����Ϣ
	bool DelDebugInfo(int nNode);
	///���������Ϣ
	void ClearDebugInfo(void);
	///������Ϣ����������Ϣ
	void SaveDebugInfo(const char * cSaveName);
	//����ָ��
	string GetDosCommand();

	///����ODBC����
	bool CreateODBCModule(void);
	///��ʼ��ODBC����
	bool InitODBCLink(void);
	///�������ݿ�
	bool LinkDataBase(const char * cServerName,const char * cUserName,const char * cPassword);
	///����һ��ִ�о��
	int CreateExecHandle(void);
	///����
	bool BindDataBaseCol(int nExeHandleIndex,int nType,unsigned short usColumnNumber,void * pValue,int iBufferLen,int &iDataFlag);
	///ִ�ж���SQL���
	bool ExecMultipleSQL(int nExeHandleIndex,vector<string> vCommand);
	///ִ��һ��SQL���
	bool ExecSQL(int nExeHandleIndex,const char * cCommand);
	///ִ���α�
	bool ExecFetch(int nExeHandleIndex,int &iNoDataFlag);
	///�ͷ�һ��ִ�о��
	bool FreeExecHandle(int nExeHandleIndex);
	///�ر����ݿ�����
	bool CloseDataBase(void);

	///�������绷��
	bool CreateTcp(void);
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
	///�ͷŵ������
	void ReleaseExportModule(void);
private:
	CExportModule(void);
	~CExportModule(void);
	static CExportModule *				m_pHandle;
	CDebugModuleInterface *				m_hDebugHandle;
	CODBCModuleInterface *				m_hODBCHandle;
	CTcpModuleInterface *				m_hTcpHandle;
};
#endif