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
	static CDebugModule * GetDebugModule();
	//����һ����Ϣ���ľ��
	void ReleaseDebugModule();
	//���ڲ�ϵͳһ���õ�����Ľӿڣ��Ǵ���
	static CDebugModule * GetDebugHandle();
	///����Ϣ�ռ�ϵͳ
	void SetOn(bool bIsWindow);
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
	/////////////////////////////////////////////////////////////////////////////////////////
	//Dosģʽ����
	/////////////////////////////////////////////////////////////////////////////////////////
	//����ָ��
	string GetDosCommand();
	
private:
	///�߳�����
	typedef struct ThreadSetting
	{
		//�߳�������
		SECURITY_ATTRIBUTES					psa;									//��ȫ�Ե�����
		HANDLE								m_handle;								//�߳̾��
		unsigned							m_uID;									//�̵߳ı�־ֵ


		DWORD								m_iSleep;								//������
	}stThreadSetting;
	
	stThreadSetting 						m_stDebugThread;						//����ģ���߳̾��
	int										m_iLoop;								//ѭ������
	static unsigned _stdcall DebugAutoDisplay(void * Param);						//����ѭ���ṹ�ĺ���
	void AutoDisplay(void);															//�Զ���ʾ��Ϣ
	unsigned int AutoDisplayLoop(void);												//�Զ���ʾѭ��
	void CloseAutoDisplay(void);
	int										m_iShowCount;
	//����DOS�����������߳�
	void StartDosCommandInput(void);
	//�ر�DOS�����������߳�
	bool CloseDosCommandInput();
	stThreadSetting							m_stInpuThread;
	int										m_iInputLoop;
	static unsigned _stdcall ThreadInput(void * Param);
	string									m_sCommand;
	
	CDebugModule(void);
	~CDebugModule(void);
	static CDebugModule *							m_pHandle;			/// ���
	int												m_iHandleCount;		/// ������
	int												m_iInfoState;		/// ��Ϣ����״̬
	vector<CSaveInfo *>								m_vInfoList;		/// ��Ϣ����
	
	

};
///�ٽ���
///��������Vector�ٽ�����Framework�����
class CDebugCriticalSection
{
public:
	CDebugCriticalSection(void);
	~CDebugCriticalSection(void);
};
#endif