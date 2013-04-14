#ifndef __ODBC_BASE_MODULE_H__
#define __ODBC_BASE_MODULE_H__
#include "DebugModuleInterface.h"
#include "ODBCModuleInterface.h"
class CODBCBaseModule : public CODBCModuleInterface
{
public:
	//返回一个ODBC句柄
	static CODBCBaseModule * GetODBCHandle(void);
	//销毁一个ODBC句柄
	void ReleaseODBCHandle(void);
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

private:
	CODBCBaseModule();
	~CODBCBaseModule();
	static CODBCBaseModule *						m_pHandle;			/// 句柄
private:
	HENV										m_hEnv;				/// Environment   
	HDBC										m_hDbc;				/// Connection handle
	vector<HSTMT>								m_vStmt;			/// Statement handle
	int											m_iFlagDebug;		/// Debug模式的标志

	CDebugModuleInterface	*					m_hDebugInterfaceModule;
};
#endif