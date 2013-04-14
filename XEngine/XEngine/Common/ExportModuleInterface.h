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
	///打开信息收集系统
	virtual void SetDebugOn(bool bIsWindow) = 0;
	///关闭信息收集系统
	virtual void SetDebugOff(void) = 0;
	///返回信息收集系统状态
	virtual int GetDebugState(void) = 0;
	/// 添加消息
	virtual void AddDebugMsgInfo(int iMsg) = 0;
	///添加系统错误信息
	virtual void AddDebugErrorInfo(int iErrorCode) = 0;
	///添加字符信息
	virtual void AddDebugStringInfo(const char * cValue) = 0;
	/// 返回消息的总数
	virtual int GetDebugInfoCount(void) = 0;
	//返回类型
	virtual int GetDebugType(int nNode) = 0;					
	//返回消息
	virtual int GetDebugMsgCode(int nNode) = 0;
	//返回等级
	virtual int GetDebugLevel(int nNode) = 0;
	//返回系统的错误代码
	virtual int GetDebugErrorCode(int nNode) = 0;
	//返回提示
	virtual const char * GetDebugMsg(int nNode) = 0;
	///清除某一条消息
	virtual bool DelDebugInfo(int nNode) = 0;
	///清除所有消息
	virtual void ClearDebugInfo(void) = 0;
	///保存信息中心所有信息
	virtual void SaveDebugInfo(const char * cSaveName) = 0;
	//返回指令
	virtual string GetDosCommand() = 0;

	///创建ODBC环境
	virtual bool CreateODBCModule(void) = 0;
	///初始化ODBC环境
	virtual bool InitODBCLink(void) = 0;
	///连接数据库
	virtual bool LinkDataBase(const char * cServerName,const char * cUserName,const char * cPassword) = 0;
	///创建一个执行句柄
	virtual int CreateExecHandle(void) = 0;
	///绑定列
	virtual bool BindDataBaseCol(int nExeHandleIndex,int nType,unsigned short usColumnNumber,void * pValue,int iBufferLen,int &iDataFlag) = 0;
	///执行多条SQL语句
	virtual bool ExecMultipleSQL(int nExeHandleIndex,vector<string> vCommand) = 0;
	///执行一条SQL语句
	virtual bool ExecSQL(int nExeHandleIndex,const char * cCommand) = 0;
	///执行游标
	virtual bool ExecFetch(int nExeHandleIndex,int &iNoDataFlag) = 0;
	///释放一个执行句柄
	virtual bool FreeExecHandle(int nExeHandleIndex) = 0;
	///关闭数据库连接
	virtual bool CloseDataBase(void) = 0;

	///创建网络环境
	virtual bool CreateTcp(void) = 0;
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
	///释放导出句柄
	virtual void ReleaseExportModule(void) = 0;
protected:
	CExportModuleInterface(){};
	virtual ~CExportModuleInterface(){};
};
CExportModuleInterface * CreateExportHandle();
#endif