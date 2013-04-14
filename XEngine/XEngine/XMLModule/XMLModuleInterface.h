#ifndef __XML_MODULE_INTERFACE_H__
#define __XML_MODULE_INTERFACE_H__
class CXMLModuleInterface
{
public:
	virtual void ReleaseXMLhandle(void) = 0;
	///��һ��XML
	virtual string GetXMLOpenFilePath(HWND hWnd) = 0;
	///����һ��XML
	virtual string GetXMLSaveFilePath(HWND hWnd) = 0;
	/// ����XML�����ʽ
	virtual bool CreateXMLTitle(const char * cFileName) = 0;
	/// ����XML
	virtual bool LoadXML(const char * cFileName) = 0;
	/// ���Ҿ�����ͬ���ֵĽڵ�ָ���б�
	virtual bool FindNodeList( const char * cNodeName ) = 0;
	/// �����б�ָ��ĳ���
	virtual int GetNodeListCount( void ) = 0;
	/// �ڽڵ�ָ���б���޸�ָ���Ľڵ�
	virtual bool ChangeTextInNodeList(const char * cChangeText,int nIndexNode = 0) = 0;
	/// �ڽڵ��б�������µĽڵ�
	virtual bool AppendNodeInNodeList( const char * cAppNode,int nIndexNode = 0 ) = 0;
	/// �ڽڵ��б����Ƴ�ĳ���ڵ�
	virtual bool RemoveNodeInNodeList(int nIndexNode = 0) = 0;
	/// ����XML
	virtual bool SaveXML(_variant_t cFileName) = 0;
	/// ����XML����
	virtual bool SearchXMLTree(void) = 0;
	/// XML�ڵ������
	virtual int GetXMLCount(void) = 0;
	/// ���������ؽڵ���
	virtual const char * GetXMLNodeName(int nIndexNode) = 0;
	/// ���������ؽڵ�����
	virtual const char * GetXMLNodeText(int nIndexNode) = 0;
	/// ���ڵ������ҽڵ�����
	virtual const char * FindNodeText(string sNodeName) = 0;
protected:
	CXMLModuleInterface(){};
	virtual ~CXMLModuleInterface(){};
};
CXMLModuleInterface * CreateXMLLib(void);
#endif