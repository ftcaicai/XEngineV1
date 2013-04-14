#ifndef __DEBUG_MODULE_INTERFACE_H__
#define __DEBUG_MODULE_INTERFACE_H__

#include "System_Header.h"

///定义类型(_Type)
#define SYSTEMINFO			1				/// 操作系统提示的错误
#define ENGINERINFO			2				/// 引擎信息

///定义级别(_Level)
#define HARD				10				//严重级（一般系统级）
#define NORMAL				11				//普通级（输入参数级、流程级）
#define NONE				12				//无级（一般提示信息）

#define INFOLIST(_Type,_Level,_Msg,_ShowMsg) _Msg,

enum
{
#include "ListInfo.txt"
};

enum DebugSwitch
{
	DS_ON,												/// 打开信息收集
	DS_OFF,												/// 关闭信息收集
};

class CDebugModuleInterface
{
public:
	///打开信息收集系统
	virtual void SetOn(bool bIsWindow) = 0;
	///关闭信息收集系统
	virtual void SetOff(void) = 0;
	///返回信息收集系统状态
	virtual int GetState(void) = 0;
	/// 添加消息
	virtual void AddMsgInfo(int iMsg) = 0;
	///添加系统错误信息
	virtual void AddErrorInfo(int iErrorCode) = 0;
	///添加字符信息
	virtual void AddStringInfo(const char * cValue) = 0;
	/// 返回消息的总数
	virtual int GetInfoCount(void) = 0;
	//返回类型
	virtual int GetType(int nNode) = 0;					
	//返回消息
	virtual int GetMsgCode(int nNode) = 0;
	//返回等级
	virtual int GetLevel(int nNode) = 0;
	//返回系统的错误代码
	virtual int GetErrorCode(int nNode) = 0;
	//返回提示
	virtual const char * GetMsg(int nNode) = 0;
	///清除某一条消息
	virtual bool DelInfo(int nNode) = 0;
	///清除所有消息
	virtual void ClearInfo(void) = 0;
	///保存信息中心所有信息
	virtual void SaveInfo(const char * cSaveName) = 0;
	//销毁一个信息中心句柄
	virtual void ReleaseDebugModule() = 0;
	//返回指令
	virtual string GetDosCommand() = 0;
protected:
	CDebugModuleInterface(){};
	virtual ~CDebugModuleInterface(){};
};
CDebugModuleInterface * CreateDebugHandle();
#endif