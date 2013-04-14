#include"ODBCBaseModule.h"
CODBCBaseModule * CODBCBaseModule::m_pHandle = NULL;
int			g_iODBCHandleCount;			/// ¼ÆÊýÆ÷
CODBCBaseModule::CODBCBaseModule()
{
	m_hEnv = NULL;
	m_hDbc = NULL;
	m_vStmt.clear();
	m_hDebugInterfaceModule = NULL;
	m_hDebugInterfaceModule = CreateDebugHandle();
}
CODBCBaseModule::~CODBCBaseModule()
{
	m_hEnv = NULL;
	m_hDbc = NULL;
	this->CloseDataBase();
	if(m_hDebugInterfaceModule != NULL)
	{
		m_hDebugInterfaceModule->ReleaseDebugModule();
		m_hDebugInterfaceModule = NULL;
	}
}
CODBCBaseModule * CODBCBaseModule::GetODBCHandle(void)
{
	if (m_pHandle == NULL)
	{
		m_pHandle = new CODBCBaseModule;
		if (m_pHandle != NULL)
		{
			g_iODBCHandleCount = 0;
			return m_pHandle;
		}
		else
			return NULL;
	}
	else
	{
		g_iODBCHandleCount++;
		return m_pHandle;
	}
}
void CODBCBaseModule::ReleaseODBCHandle(void)
{
	if (m_pHandle != NULL && g_iODBCHandleCount == 0)
	{
		delete m_pHandle;
		m_pHandle = NULL;
	}
	else
	{
		g_iODBCHandleCount--;
	}
}
bool CODBCBaseModule::InitODBCLink()
{
	SQLRETURN						hr;
	hr = SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&m_hEnv);
	if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
	{
		hr = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
		if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
		{
			hr = SQLAllocHandle(SQL_HANDLE_DBC,m_hEnv,&m_hDbc);
			if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
			{
				if (m_hDebugInterfaceModule != NULL)
				{
					m_hDebugInterfaceModule->AddMsgInfo(ODBCInitSucceed);
				}
				return true;
			}
			else
			{
				if (m_hDebugInterfaceModule != NULL)
				{
					m_hDebugInterfaceModule->AddMsgInfo(ODBCDbcError);
				}
				return false;
			}
		}
		else
		{
			if (m_hDebugInterfaceModule != NULL)
			{
				m_hDebugInterfaceModule->AddMsgInfo(ODBCEnvAttrError);
			}
			return false;
		}
	}
	else
	{
		if (m_hDebugInterfaceModule != NULL)
		{
			m_hDebugInterfaceModule->AddMsgInfo(ODBCEnvError);
		}
		return false;
	}
}
bool CODBCBaseModule::LinkDataBase(const char * cServerName,const char * cUserName,const char * cPassword)
{
	SQLRETURN						hr;
	hr = SQLConnect(m_hDbc,(SQLCHAR *)cServerName,(SQLSMALLINT)strlen(cServerName),(SQLCHAR *)cUserName,(SQLSMALLINT)strlen(cUserName),(SQLCHAR *)cPassword,(SQLSMALLINT)strlen(cPassword)); 
	if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
	{
		if (m_hDebugInterfaceModule != NULL)
		{
			m_hDebugInterfaceModule->AddMsgInfo(ODBCLinkSucceed);
		}
		return true;
	}
	else
	{
		if (m_hDebugInterfaceModule != NULL)
		{
			m_hDebugInterfaceModule->AddMsgInfo(ODBCLinkError);
		}
		return false;
	}
}
int CODBCBaseModule::CreateExecHandle(void)
{
	HSTMT							hStmt = NULL;
	SQLRETURN						hr;
	hr = SQLAllocHandle(SQL_HANDLE_STMT,m_hDbc,&hStmt);
	if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
	{
		m_vStmt.push_back(hStmt);
		if (m_hDebugInterfaceModule != NULL)
		{
			m_hDebugInterfaceModule->AddMsgInfo(CreateODBCExecHandleSucceed);
		}
		return (int)m_vStmt.size() - 1;
	}
	else
	{
		if (m_hDebugInterfaceModule != NULL)
		{
			m_hDebugInterfaceModule->AddMsgInfo(CreateODBCExecHandleError);
		}
		return -1;
	}
}
bool CODBCBaseModule::BindDataBaseCol(int nExeHandleIndex,int nType,unsigned short usColumnNumber,void * pValue,int iBufferLen,int &iDataFlag)
{
	SQLRETURN						hr;
	SQLINTEGER						ODBCFlag;
	if (nExeHandleIndex >= 0  && nExeHandleIndex < (int)m_vStmt.size())
	{
		switch(nType)
		{
		case ODBC_CHAR:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_CHAR,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_BIT:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_BIT,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_BINARY:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_BINARY,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_VARBOOKMARK:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_VARBOOKMARK,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_UTINYINT:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_UTINYINT,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_STINYINT:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_STINYINT,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_SSHORT:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_SSHORT,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_USHORT:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_USHORT,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_SLONG:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_SLONG,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_ULONG:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_ULONG,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_BOOKMARK:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_ULONG,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_FLOAT:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_FLOAT,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_DOUBLE:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_DOUBLE,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_SBIGINT:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_SBIGINT,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_UBIGINT:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_UBIGINT,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_TYPE_DATE:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_TYPE_DATE,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_TYPE_TIME:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_TYPE_TIME,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_TYPE_TIMESTAMP:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_TYPE_TIMESTAMP,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_NUMERIC:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_NUMERIC,pValue,iBufferLen,&ODBCFlag);
			}break;
		case ODBC_GUID:
			{
				hr = SQLBindCol(m_vStmt[nExeHandleIndex],usColumnNumber,SQL_C_GUID,pValue,iBufferLen,&ODBCFlag);
			}break;
		};

		if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
		{
			if (ODBCFlag == SQL_NULL_DATA)
				iDataFlag = DATA_NULL;
			else
				iDataFlag = DATA_NO_NULL;

			if (m_hDebugInterfaceModule != NULL)
			{
				m_hDebugInterfaceModule->AddMsgInfo(ODBCBindSucceed);
			}
			return true;
		}
		else
		{
			iDataFlag = DATA_NULL;
			if (m_hDebugInterfaceModule != NULL)
			{
				m_hDebugInterfaceModule->AddMsgInfo(ODBCBindError);
			}
			return false;
		}
	}
	else
	{
		if (m_hDebugInterfaceModule != NULL)
		{
			m_hDebugInterfaceModule->AddMsgInfo(InputValueRangeOut);
		}
		return false;
	}
}
bool CODBCBaseModule::ExecMultipleSQL(int nExeHandleIndex,vector<string> vCommand)
{
	SQLRETURN						hr;
	int								nCount;
	if (nExeHandleIndex >= 0 && nExeHandleIndex < (int)m_vStmt.size())
	{
		for (nCount = 1; nCount < (int)vCommand.size(); ++nCount)
		{
			hr = SQLPrepare(m_vStmt[nExeHandleIndex],(SQLCHAR *)vCommand[nCount].c_str(),(SQLSMALLINT)vCommand[nCount].size());
			if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
			{
				continue;
			}
			else
			{
				break;
			}
		}
		if (nCount == (int)vCommand.size())
		{
			hr = SQLExecute(m_vStmt[nExeHandleIndex]);
			if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
			{
				if (m_hDebugInterfaceModule != NULL)
				{
					m_hDebugInterfaceModule->AddMsgInfo(ExecODBCSucceed);
				}
				return true;
			}
			else
			{
				if (m_hDebugInterfaceModule != NULL)
				{
					m_hDebugInterfaceModule->AddMsgInfo(ExecODBCError);
				}
				return false;
			}

		}
		else
		{
			if (m_hDebugInterfaceModule != NULL)
			{
				m_hDebugInterfaceModule->AddMsgInfo(ODBCStmtError);
			}
			return false;
		}
	}
	else
	{
		if (m_hDebugInterfaceModule != NULL)
		{
			m_hDebugInterfaceModule->AddMsgInfo(InputValueRangeOut);
		}
		return false;
	}
}


bool CODBCBaseModule::ExecSQL(int nExeHandleIndex,const char * cCommand)
{
	SQLRETURN						hr;
	if (nExeHandleIndex >= 0 && nExeHandleIndex < (int)m_vStmt.size())
	{
		hr = SQLExecDirect(m_vStmt[nExeHandleIndex],(SQLCHAR *)cCommand,(SQLSMALLINT)strlen(cCommand));
		if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
		{
			if (m_hDebugInterfaceModule != NULL)
			{
				m_hDebugInterfaceModule->AddMsgInfo(ExecODBCSucceed);
			}
			return true;
		}
		else
		{
			if (m_hDebugInterfaceModule != NULL)
			{
				m_hDebugInterfaceModule->AddMsgInfo(ExecODBCError);
			}
			return false;
		}
	}
	else
	{
		if (m_hDebugInterfaceModule != NULL)
		{
			m_hDebugInterfaceModule->AddMsgInfo(InputValueRangeOut);
		}
		return false;
	}
}
bool CODBCBaseModule::ExecFetch(int nExeHandleIndex,int &iNoDataFlag)
{
	SQLRETURN						hr;
	if (nExeHandleIndex >= 0 && nExeHandleIndex < (int)m_vStmt.size())
	{
		hr = SQLFetch(m_vStmt[nExeHandleIndex]);
		if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
		{
			if (m_hDebugInterfaceModule != NULL)
			{
				m_hDebugInterfaceModule->AddMsgInfo(GetODBCRecordSucceed);
			}
			iNoDataFlag = 0;
			return true;
		}
		else
		{
			if (hr == SQL_NO_DATA)
			{
				if (m_hDebugInterfaceModule != NULL)
				{
					m_hDebugInterfaceModule->AddMsgInfo(GetODBCRecordCompleted);
				}
				iNoDataFlag = 1;
				return true;
			}

			if (m_hDebugInterfaceModule != NULL)
			{
				m_hDebugInterfaceModule->AddMsgInfo(GetODBCRecordError);
			}
			return false;
		}
	}
	else
	{
		if (m_hDebugInterfaceModule != NULL)
		{
			m_hDebugInterfaceModule->AddMsgInfo(InputValueRangeOut);
		}
		return false;
	}
}
bool CODBCBaseModule::FreeExecHandle(int nExeHandleIndex)
{
	SQLRETURN						hr;
	if (nExeHandleIndex >= 0 && nExeHandleIndex < (int)m_vStmt.size())
	{
		if (m_vStmt[nExeHandleIndex] != NULL)
		{
			hr = SQLFreeHandle(SQL_HANDLE_STMT,m_vStmt[nExeHandleIndex]);
			if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
			{
				m_vStmt.erase(m_vStmt.begin() + nExeHandleIndex);
				if (m_hDebugInterfaceModule != NULL)
				{
					m_hDebugInterfaceModule->AddMsgInfo(ReleaseODBCExecHandleSucceed);
				}
				return true;
			}
			else
			{
				if (m_hDebugInterfaceModule != NULL)
				{
					m_hDebugInterfaceModule->AddMsgInfo(ReleaseODBCExecHandleError);
				}
				return false;
			}
		}
		else
			return false;
	}
	else
	{
		if (m_hDebugInterfaceModule != NULL)
		{
			m_hDebugInterfaceModule->AddMsgInfo(InputValueRangeOut);
		}
		return false;
	}
}
bool CODBCBaseModule::CloseDataBase(void)
{
	SQLRETURN						hr;
	if (m_hDbc != NULL)
	{
		hr = SQLDisconnect(m_hDbc);
		if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
		{
			for (int iCount = 0; iCount < (int)m_vStmt.size(); ++iCount)
			{
				if (m_vStmt[iCount] != NULL)
				{
					SQLFreeHandle(SQL_HANDLE_STMT,m_vStmt[iCount]);
				}
			}
			m_vStmt.clear();

			hr = SQLFreeHandle(SQL_HANDLE_DBC,m_hDbc);
			if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
			{
				m_hDbc = NULL;
				if (m_hDebugInterfaceModule != NULL)
				{
					m_hDebugInterfaceModule->AddMsgInfo(CloseDBCSucceed);
				}
			}
			else
			{
				m_hDbc = NULL;
				if (m_hDebugInterfaceModule != NULL)
				{
					m_hDebugInterfaceModule->AddMsgInfo(CloseODBCError);
				}
				m_hDbc = NULL;
				return false;
			}


			SQLFreeHandle(SQL_HANDLE_ENV,m_hEnv);
			if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
			{
				m_hEnv = NULL;
				if (m_hDebugInterfaceModule != NULL)
				{
					m_hDebugInterfaceModule->AddMsgInfo(CloseENVSucceed);
				}
			}
			else
			{
				m_hEnv = NULL;
				if (m_hDebugInterfaceModule != NULL)
				{
					m_hDebugInterfaceModule->AddMsgInfo(CloseODBCError);
				}
				m_hEnv = NULL;
				return false;
			}

			if (m_hDebugInterfaceModule != NULL)
			{
				m_hDebugInterfaceModule->AddMsgInfo(CloseODBCSucceed);
			}
			return true;
		}

	}
	else
	{
		if (m_hDebugInterfaceModule != NULL)
		{
			m_hDebugInterfaceModule->AddMsgInfo(CloseODBCSucceed);
		}
		return true;
	}
}
CODBCModuleInterface * CreateODBC(void)
{
	CODBCModuleInterface * hHandle;
	hHandle = static_cast<CODBCModuleInterface *>(CODBCBaseModule::GetODBCHandle());
	if(hHandle != NULL)
	{
		return hHandle;
	}
	else
		return NULL;
}