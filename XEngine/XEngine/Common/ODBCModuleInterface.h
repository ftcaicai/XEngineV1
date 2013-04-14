#ifndef __ODBC_MODULE_INTERFACE_H__
#define __ODBC_MODULE_INTERFACE_H__
#include "../Common/ExportModuleInterface.h"
class CODBCModuleInterface
{
public:
	//����һ��ODBC���
	virtual void ReleaseODBCHandle(void) = 0;
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
protected:
	CODBCModuleInterface(){};
	virtual ~CODBCModuleInterface(){};

};
CODBCModuleInterface * CreateODBC(void);
#endif