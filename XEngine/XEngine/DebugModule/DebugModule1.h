#ifndef __DEBUG_MODULE_H__
#define __DEBUG_MODULE_H__
#include "System_Header.h"
#include "Save_Info.h"
#include "DebugModuleInterface.h"

///����һ������Debug�ĵ������ģ�Ĭ��״̬�ǹرյġ�
class CDebugModule :public CDebugModuleInterface
{
public:
	//����һ����Ϣ���ľ��
	static CDebugModule * GetDebugModule(void);
	//����һ����Ϣ���ľ��
	void ReleaseDebugModule();
	///����Ϣ�ռ�ϵͳ
	void SetOn(void);
	///�ر���Ϣ�ռ�ϵͳ
	void SetOff(void);
	///������Ϣ�ռ�ϵͳ״̬
	int GetState(void);
	/// �����Ϣ
	void AddMsgInfo(int iMsg);
	///���ϵͳ������Ϣ
	void AddErrorInfo(int iErrorCode);
	///����ַ���Ϣ
	void AddStringInfo(const char * cValue);
	/// ������Ϣ������
	int GetInfoCount(void);
	//��������
	int GetType(int nNode);					
	//������Ϣ
	int GetMsgCode(int nNode);
	//���صȼ�
	int GetLevel(int nNode);
	//����ϵͳ�Ĵ������
	int GetErrorCode(int nNode);
	//������ʾ
	const char * GetMsg(int nNode);
	///���ĳһ����Ϣ
	bool DelInfo(int nNode);
	///���������Ϣ
	void ClearInfo(void);
	///������Ϣ����������Ϣ
	void SaveInfo(const char * cSaveName);
private:
	CDebugModule(void);
	~CDebugModule(void);
	static CDebugModule *							m_pHandle;			/// ���
	int												m_iInfoState;		/// ��Ϣ����״̬
	vector<CSaveInfo *>								m_vInfoList;		/// ��Ϣ����
	

};
#endif