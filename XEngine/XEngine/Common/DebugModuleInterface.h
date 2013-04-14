#ifndef __DEBUG_MODULE_INTERFACE_H__
#define __DEBUG_MODULE_INTERFACE_H__

#include "System_Header.h"

///��������(_Type)
#define SYSTEMINFO			1				/// ����ϵͳ��ʾ�Ĵ���
#define ENGINERINFO			2				/// ������Ϣ

///���弶��(_Level)
#define HARD				10				//���ؼ���һ��ϵͳ����
#define NORMAL				11				//��ͨ������������������̼���
#define NONE				12				//�޼���һ����ʾ��Ϣ��

#define INFOLIST(_Type,_Level,_Msg,_ShowMsg) _Msg,

enum
{
#include "ListInfo.txt"
};

enum DebugSwitch
{
	DS_ON,												/// ����Ϣ�ռ�
	DS_OFF,												/// �ر���Ϣ�ռ�
};

class CDebugModuleInterface
{
public:
	///����Ϣ�ռ�ϵͳ
	virtual void SetOn(bool bIsWindow) = 0;
	///�ر���Ϣ�ռ�ϵͳ
	virtual void SetOff(void) = 0;
	///������Ϣ�ռ�ϵͳ״̬
	virtual int GetState(void) = 0;
	/// �����Ϣ
	virtual void AddMsgInfo(int iMsg) = 0;
	///���ϵͳ������Ϣ
	virtual void AddErrorInfo(int iErrorCode) = 0;
	///����ַ���Ϣ
	virtual void AddStringInfo(const char * cValue) = 0;
	/// ������Ϣ������
	virtual int GetInfoCount(void) = 0;
	//��������
	virtual int GetType(int nNode) = 0;					
	//������Ϣ
	virtual int GetMsgCode(int nNode) = 0;
	//���صȼ�
	virtual int GetLevel(int nNode) = 0;
	//����ϵͳ�Ĵ������
	virtual int GetErrorCode(int nNode) = 0;
	//������ʾ
	virtual const char * GetMsg(int nNode) = 0;
	///���ĳһ����Ϣ
	virtual bool DelInfo(int nNode) = 0;
	///���������Ϣ
	virtual void ClearInfo(void) = 0;
	///������Ϣ����������Ϣ
	virtual void SaveInfo(const char * cSaveName) = 0;
	//����һ����Ϣ���ľ��
	virtual void ReleaseDebugModule() = 0;
	//����ָ��
	virtual string GetDosCommand() = 0;
protected:
	CDebugModuleInterface(){};
	virtual ~CDebugModuleInterface(){};
};
CDebugModuleInterface * CreateDebugHandle();
#endif