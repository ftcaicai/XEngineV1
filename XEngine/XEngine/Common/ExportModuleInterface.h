#ifndef __EXPORT_MODULE_INTERFACE_H__
#define __EXPORT_MODULE_INTERFACE_H__
#include "../Common/System_Header.h"
typedef void (* NetCallBack)( vector<string> vParam );
enum DataFlag
{
	DATA_NULL,
	DATA_NO_NULL,
};
enum SQLToCType
{
	ODBC_CHAR,
	ODBC_SSHORT,
	ODBC_USHORT,
	ODBC_SLONG,
	ODBC_ULONG,
	ODBC_FLOAT,
	ODBC_DOUBLE,
	ODBC_BIT,
	ODBC_STINYINT,
	ODBC_UTINYINT,
	ODBC_SBIGINT,
	ODBC_UBIGINT,
	ODBC_BINARY,
	ODBC_BOOKMARK,
	ODBC_VARBOOKMARK,
	ODBC_TYPE_DATE,
	ODBC_TYPE_TIME,
	ODBC_TYPE_TIMESTAMP,
	ODBC_NUMERIC,
	ODBC_GUID,
};
class CExportModuleInterface
{
public:
	///����Ϣ�ռ�ϵͳ
	virtual void SetDebugOn(bool bIsWindow) = 0;
	///�ر���Ϣ�ռ�ϵͳ
	virtual void SetDebugOff(void) = 0;
	///������Ϣ�ռ�ϵͳ״̬
	virtual int GetDebugState(void) = 0;
	/// �����Ϣ
	virtual void AddDebugMsgInfo(int iMsg) = 0;
	///���ϵͳ������Ϣ
	virtual void AddDebugErrorInfo(int iErrorCode) = 0;
	///����ַ���Ϣ
	virtual void AddDebugStringInfo(const char * cValue) = 0;
	/// ������Ϣ������
	virtual int GetDebugInfoCount(void) = 0;
	//��������
	virtual int GetDebugType(int nNode) = 0;					
	//������Ϣ
	virtual int GetDebugMsgCode(int nNode) = 0;
	//���صȼ�
	virtual int GetDebugLevel(int nNode) = 0;
	//����ϵͳ�Ĵ������
	virtual int GetDebugErrorCode(int nNode) = 0;
	//������ʾ
	virtual const char * GetDebugMsg(int nNode) = 0;
	///���ĳһ����Ϣ
	virtual bool DelDebugInfo(int nNode) = 0;
	///���������Ϣ
	virtual void ClearDebugInfo(void) = 0;
	///������Ϣ����������Ϣ
	virtual void SaveDebugInfo(const char * cSaveName) = 0;
	//����ָ��
	virtual string GetDosCommand() = 0;

	///����ODBC����
	virtual bool CreateODBCModule(void) = 0;
	///��ʼ��ODBC����
	virtual bool InitODBCLink(void) = 0;
	///�������ݿ�
	virtual bool LinkDataBase(const char * cServerName,const char * cUserName,const char * cPassword) = 0;
	///����һ��ִ�о��
	virtual int CreateExecHandle(void) = 0;
	///����
	virtual bool BindDataBaseCol(int nExeHandleIndex,int nType,unsigned short usColumnNumber,void * pValue,int iBufferLen,int &iDataFlag) = 0;
	///ִ�ж���SQL���
	virtual bool ExecMultipleSQL(int nExeHandleIndex,vector<string> vCommand) = 0;
	///ִ��һ��SQL���
	virtual bool ExecSQL(int nExeHandleIndex,const char * cCommand) = 0;
	///ִ���α�
	virtual bool ExecFetch(int nExeHandleIndex,int &iNoDataFlag) = 0;
	///�ͷ�һ��ִ�о��
	virtual bool FreeExecHandle(int nExeHandleIndex) = 0;
	///�ر����ݿ�����
	virtual bool CloseDataBase(void) = 0;

	///�������绷��
	virtual bool CreateTcp(void) = 0;
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
	///�ͷŵ������
	virtual void ReleaseExportModule(void) = 0;
protected:
	CExportModuleInterface(){};
	virtual ~CExportModuleInterface(){};
};
CExportModuleInterface * CreateExportHandle();
#endif