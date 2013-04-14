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
	static CDebugModule * GetDebugModule(void);
	//销毁一个信息中心句柄
	void ReleaseDebugModule();
	///打开信息收集系统
	void SetOn(void);
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
private:
	CDebugModule(void);
	~CDebugModule(void);
	static CDebugModule *							m_pHandle;			/// 句柄
	int												m_iInfoState;		/// 信息中心状态
	vector<CSaveInfo *>								m_vInfoList;		/// 信息保存
	

};
#endif