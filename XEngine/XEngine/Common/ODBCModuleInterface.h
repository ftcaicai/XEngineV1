#ifndef __ODBC_MODULE_INTERFACE_H__
#define __ODBC_MODULE_INTERFACE_H__
#include "../Common/ExportModuleInterface.h"
class CODBCModuleInterface
{
public:
	//销毁一个ODBC句柄
	virtual void ReleaseODBCHandle(void) = 0;
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
protected:
	CODBCModuleInterface(){};
	virtual ~CODBCModuleInterface(){};

};
CODBCModuleInterface * CreateODBC(void);
#endif