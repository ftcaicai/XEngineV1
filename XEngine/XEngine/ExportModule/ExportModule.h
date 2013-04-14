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
	///反回导出句柄
	static CExportModule * GetExportModule(void);
	///打开信息收集系统
	void SetDebugOn(bool bIsWindow);
	///关闭信息收集系统
	void SetDebugOff(void);
	///返回信息收集系统状态
	int GetDebugState(void);
	/// 添加消息
	void AddDebugMsgInfo(int iMsg);
	///添加系统错误信息
	void AddDebugErrorInfo(int iErrorCode);
	///添加字符信息
	void AddDebugStringInfo(const char * cValue);
	/// 返回消息的总数
	int GetDebugInfoCount(void);
	//返回类型
	int GetDebugType(int nNode);					
	//返回消息
	int GetDebugMsgCode(int nNode);
	//返回等级
	int GetDebugLevel(int nNode);
	//返回系统的错误代码
	int GetDebugErrorCode(int nNode);
	//返回提示
	const char * GetDebugMsg(int nNode);
	///清除某一条消息
	bool DelDebugInfo(int nNode);
	///清除所有消息
	void ClearDebugInfo(void);
	///保存信息中心所有信息
	void SaveDebugInfo(const char * cSaveName);
	//返回指令
	string GetDosCommand();

	///创建ODBC环境
	bool CreateODBCModule(void);
	///初始化ODBC环境
	bool InitODBCLink(void);
	///连接数据库
	bool LinkDataBase(const char * cServerName,const char * cUserName,const char * cPassword);
	///创建一个执行句柄
	int CreateExecHandle(void);
	///绑定列
	bool BindDataBaseCol(int nExeHandleIndex,int nType,unsigned short usColumnNumber,void * pValue,int iBufferLen,int &iDataFlag);
	///执行多条SQL语句
	bool ExecMultipleSQL(int nExeHandleIndex,vector<string> vCommand);
	///执行一条SQL语句
	bool ExecSQL(int nExeHandleIndex,const char * cCommand);
	///执行游标
	bool ExecFetch(int nExeHandleIndex,int &iNoDataFlag);
	///释放一个执行句柄
	bool FreeExecHandle(int nExeHandleIndex);
	///关闭数据库连接
	bool CloseDataBase(void);

	///创建网络环境
	bool CreateTcp(void);
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
	///释放导出句柄
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