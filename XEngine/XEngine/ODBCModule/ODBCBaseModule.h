#ifndef __ODBC_BASE_MODULE_H__
#define __ODBC_BASE_MODULE_H__
#include "DebugModuleInterface.h"
#include "ODBCModuleInterface.h"
class CODBCBaseModule : public CODBCModuleInterface
{
public:
	//����һ��ODBC���
	static CODBCBaseModule * GetODBCHandle(void);
	//����һ��ODBC���
	void ReleaseODBCHandle(void);
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

private:
	CODBCBaseModule();
	~CODBCBaseModule();
	static CODBCBaseModule *						m_pHandle;			/// ���
private:
	HENV										m_hEnv;				/// Environment   
	HDBC										m_hDbc;				/// Connection handle
	vector<HSTMT>								m_vStmt;			/// Statement handle
	int											m_iFlagDebug;		/// Debugģʽ�ı�־

	CDebugModuleInterface	*					m_hDebugInterfaceModule;
};
#endif