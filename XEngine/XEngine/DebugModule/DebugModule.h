#ifndef __DEBUG_MODULE_H__
#define __DEBUG_MODULE_H__
#include "System_Header.h"
#include "Save_Info.h"
#include "DebugModuleInterface.h"

///这是一个集成Debug的调试中心，默认状态是关闭的。
class CDebugModule :public CDebugModuleInterface
{
public:
	//返回一个信息中心句柄
	static CDebugModule * GetDebugModule();
	//销毁一个信息中心句柄
	void ReleaseDebugModule();
	//给内部系统一个得到句柄的接口，非创建
	static CDebugModule * GetDebugHandle();
	///打开信息收集系统
	void SetOn(bool bIsWindow);
	///关闭信息收集系统
	void SetOff(void);
	///返回信息收集系统状态
	int GetState(void);
	/// 添加消息
	void AddMsgInfo(int iMsg);
	///添加系统错误信息
	void AddErrorInfo(int iErrorCode);
	///添加字符信息
	void AddStringInfo(const char * cValue);
	/// 返回消息的总数
	int GetInfoCount(void);
	//返回类型
	int GetType(int nNode);					
	//返回消息
	int GetMsgCode(int nNode);
	//返回等级
	int GetLevel(int nNode);
	//返回系统的错误代码
	int GetErrorCode(int nNode);
	//返回提示
	const char * GetMsg(int nNode);
	///清除某一条消息
	bool DelInfo(int nNode);
	///清除所有消息
	void ClearInfo(void);
	///保存信息中心所有信息
	void SaveInfo(const char * cSaveName);
	/////////////////////////////////////////////////////////////////////////////////////////
	//Dos模式输入
	/////////////////////////////////////////////////////////////////////////////////////////
	//返回指令
	string GetDosCommand();
	
private:
	///线程设置
	typedef struct ThreadSetting
	{
		//线程描述符
		SECURITY_ATTRIBUTES					psa;									//安全性的描述
		HANDLE								m_handle;								//线程句柄
		unsigned							m_uID;									//线程的标志值


		DWORD								m_iSleep;								//挂起间隔
	}stThreadSetting;
	
	stThreadSetting 						m_stDebugThread;						//调试模块线程句柄
	int										m_iLoop;								//循环开关
	static unsigned _stdcall DebugAutoDisplay(void * Param);						//调用循环结构的函数
	void AutoDisplay(void);															//自动显示信息
	unsigned int AutoDisplayLoop(void);												//自动显示循环
	void CloseAutoDisplay(void);
	int										m_iShowCount;
	//启动DOS版命令输入线程
	void StartDosCommandInput(void);
	//关闭DOS版命令输入线程
	bool CloseDosCommandInput();
	stThreadSetting							m_stInpuThread;
	int										m_iInputLoop;
	static unsigned _stdcall ThreadInput(void * Param);
	string									m_sCommand;
	
	CDebugModule(void);
	~CDebugModule(void);
	static CDebugModule *							m_pHandle;			/// 句柄
	int												m_iHandleCount;		/// 计数器
	int												m_iInfoState;		/// 信息中心状态
	vector<CSaveInfo *>								m_vInfoList;		/// 信息保存
	
	

};
///临界区
///服务器端Vector临界区，Framework层的锁
class CDebugCriticalSection
{
public:
	CDebugCriticalSection(void);
	~CDebugCriticalSection(void);
};
#endif