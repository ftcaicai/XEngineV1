#ifndef __XML_MODULE_H__
#define __XML_MODULE_H__
#include "DebugModuleInterface.h"
#include "XMLModuleInterface.h"
///XML
typedef struct XMLNode
{
	string				g_cNodeName;
	string				g_cNodeText;
}stXMLNode;

class CXMLModule :public CXMLModuleInterface
{
public:
	static CXMLModule * GetXMLHandle(void);
	void ReleaseXMLhandle(void);
	///��һ��XML
	string GetXMLOpenFilePath(HWND hWnd);
	///����һ��XML
	string GetXMLSaveFilePath(HWND hWnd);
	/// ����XML�����ʽ
	bool CreateXMLTitle(const char * cFileName);
	/// ����XML
	bool LoadXML(const char * cFileName);
	/// ���Ҿ�����ͬ���ֵĽڵ�ָ���б�
	bool FindNodeList( const char * cNodeName );
	/// �����б�ָ��ĳ���
	int GetNodeListCount( void );
	/// �ڽڵ�ָ���б���޸�ָ���Ľڵ�
	bool ChangeTextInNodeList(const char * cChangeText,int nIndexNode = 0);
	/// �ڽڵ��б�������µĽڵ�
	bool AppendNodeInNodeList( const char * cAppNode,int nIndexNode = 0 );
	/// �ڽڵ��б����Ƴ�ĳ���ڵ�
	bool RemoveNodeInNodeList(int nIndexNode = 0);
	/// ����XML
	bool SaveXML(_variant_t cFileName);
	/// ����XML����
	bool SearchXMLTree(void);
	/// XML�ڵ������
	int GetXMLCount(void);
	/// ���������ؽڵ���
	const char * GetXMLNodeName(int nIndexNode);
	/// ���������ؽڵ�����
	const char * GetXMLNodeText(int nIndexNode);
	/// ���ڵ������ҽڵ�����
	const char * FindNodeText(string sNodeName);
private:
	///����MAP
	void UpdateNode();
	///����XML����
	bool SearchTree(SYSXMLNODE* pNode);
	///����XML����
	bool SearchNode(SYSXMLNODE* pNode);
	static CXMLModule *											m_hXMLHandle;
	CXMLModule();
	~CXMLModule();

	SYSXMLDOC *													m_pDoc;					//�ĵ�ָ��
	SYSXMLNODELIST *											m_pNodeList;			//�ڵ�ָ���б�
	SYSXMLNODE *												m_pNode;				//�ڵ�ָ��

	vector<stXMLNode *>											m_vTreeNode;
	map<string,int>												m_mTreeNode;
};
#endif