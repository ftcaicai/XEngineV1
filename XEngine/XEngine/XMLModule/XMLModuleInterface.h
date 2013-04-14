#ifndef __XML_MODULE_INTERFACE_H__
#define __XML_MODULE_INTERFACE_H__
class CXMLModuleInterface
{
public:
	virtual void ReleaseXMLhandle(void) = 0;
	///打开一个XML
	virtual string GetXMLOpenFilePath(HWND hWnd) = 0;
	///保存一个XML
	virtual string GetXMLSaveFilePath(HWND hWnd) = 0;
	/// 创建XML标题格式
	virtual bool CreateXMLTitle(const char * cFileName) = 0;
	/// 加载XML
	virtual bool LoadXML(const char * cFileName) = 0;
	/// 查找具有相同名字的节点指针列表
	virtual bool FindNodeList( const char * cNodeName ) = 0;
	/// 返回列表指针的长度
	virtual int GetNodeListCount( void ) = 0;
	/// 在节点指针列表里，修改指定的节点
	virtual bool ChangeTextInNodeList(const char * cChangeText,int nIndexNode = 0) = 0;
	/// 在节点列表里添加新的节点
	virtual bool AppendNodeInNodeList( const char * cAppNode,int nIndexNode = 0 ) = 0;
	/// 在节点列表里移除某个节点
	virtual bool RemoveNodeInNodeList(int nIndexNode = 0) = 0;
	/// 保存XML
	virtual bool SaveXML(_variant_t cFileName) = 0;
	/// 遍历XML树型
	virtual bool SearchXMLTree(void) = 0;
	/// XML节点的数量
	virtual int GetXMLCount(void) = 0;
	/// 按索引返回节点名
	virtual const char * GetXMLNodeName(int nIndexNode) = 0;
	/// 按索引返回节点文字
	virtual const char * GetXMLNodeText(int nIndexNode) = 0;
	/// 按节点名查找节点文字
	virtual const char * FindNodeText(string sNodeName) = 0;
protected:
	CXMLModuleInterface(){};
	virtual ~CXMLModuleInterface(){};
};
CXMLModuleInterface * CreateXMLLib(void);
#endif