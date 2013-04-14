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
	///打开一个XML
	string GetXMLOpenFilePath(HWND hWnd);
	///保存一个XML
	string GetXMLSaveFilePath(HWND hWnd);
	/// 创建XML标题格式
	bool CreateXMLTitle(const char * cFileName);
	/// 加载XML
	bool LoadXML(const char * cFileName);
	/// 查找具有相同名字的节点指针列表
	bool FindNodeList( const char * cNodeName );
	/// 返回列表指针的长度
	int GetNodeListCount( void );
	/// 在节点指针列表里，修改指定的节点
	bool ChangeTextInNodeList(const char * cChangeText,int nIndexNode = 0);
	/// 在节点列表里添加新的节点
	bool AppendNodeInNodeList( const char * cAppNode,int nIndexNode = 0 );
	/// 在节点列表里移除某个节点
	bool RemoveNodeInNodeList(int nIndexNode = 0);
	/// 保存XML
	bool SaveXML(_variant_t cFileName);
	/// 遍历XML树型
	bool SearchXMLTree(void);
	/// XML节点的数量
	int GetXMLCount(void);
	/// 按索引返回节点名
	const char * GetXMLNodeName(int nIndexNode);
	/// 按索引返回节点文字
	const char * GetXMLNodeText(int nIndexNode);
	/// 按节点名查找节点文字
	const char * FindNodeText(string sNodeName);
private:
	///更新MAP
	void UpdateNode();
	///搜索XML树型
	bool SearchTree(SYSXMLNODE* pNode);
	///搜索XML树型
	bool SearchNode(SYSXMLNODE* pNode);
	static CXMLModule *											m_hXMLHandle;
	CXMLModule();
	~CXMLModule();

	SYSXMLDOC *													m_pDoc;					//文档指针
	SYSXMLNODELIST *											m_pNodeList;			//节点指针列表
	SYSXMLNODE *												m_pNode;				//节点指针

	vector<stXMLNode *>											m_vTreeNode;
	map<string,int>												m_mTreeNode;
};
#endif