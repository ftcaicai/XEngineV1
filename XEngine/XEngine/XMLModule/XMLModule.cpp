#include "XMLModule.h"
CXMLModule * CXMLModule::m_hXMLHandle = NULL;
int						 g_iXMLHandleCount = 0;
CXMLModule::CXMLModule()
{
	SYSRETURN			hr;
	::CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,IID_IXMLDOMDocument, (LPVOID*)&m_pDoc);
	if (hr == S_OK)
	{	
		m_pNodeList = NULL;
		m_pNode = NULL;
	}
	else
		m_pDoc = NULL;
	m_mTreeNode.clear();
}
CXMLModule::~CXMLModule()
{
	for (int nCount = 0; nCount < (int)m_vTreeNode.size(); ++nCount)
	{
		m_vTreeNode[nCount]->g_cNodeName.clear();
		m_vTreeNode[nCount]->g_cNodeText.clear();
		delete m_vTreeNode[nCount];
		m_vTreeNode[nCount] = NULL;
	}
	m_vTreeNode.clear();
	m_mTreeNode.clear();

	if (m_pNodeList != NULL)
		m_pNodeList->Release();
	if(m_pNode != NULL)
		m_pNode->Release();
	if (m_pDoc != NULL)
		m_pDoc->Release();
	


	::CoUninitialize();
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}
CXMLModule * CXMLModule::GetXMLHandle(void)
{
	if (m_hXMLHandle == NULL)
	{
		m_hXMLHandle = new CXMLModule;
		if (m_hXMLHandle != NULL)
		{
			g_iXMLHandleCount = 0;
			return m_hXMLHandle;
		}
		else
			return NULL;
	}
	else
	{
		g_iXMLHandleCount++;
		return m_hXMLHandle;
	}
}
void CXMLModule::ReleaseXMLhandle(void)
{
	if (m_hXMLHandle != NULL && g_iXMLHandleCount == 0)
	{
		delete m_hXMLHandle;
		m_hXMLHandle = NULL;
	}
	else
	{
		g_iXMLHandleCount--;
	}
}
string CXMLModule::GetXMLOpenFilePath(HWND hWnd)
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	string sFile;

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "XML\0*.xml\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn)==TRUE) 
	{
		sFile.clear();
		sFile.append(ofn.lpstrFile);
		return sFile;
	}
	else
	{
		sFile.clear();
		return sFile;
	}
}
string CXMLModule::GetXMLSaveFilePath(HWND hWnd)
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	string sFile;

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "XML\0*.xml\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetSaveFileName(&ofn)==TRUE) 
	{
		sFile.clear();
		sFile.append(ofn.lpstrFile);
		return sFile;
	}
	else
	{
		sFile.clear();
		return sFile;
	}
}
bool CXMLModule::CreateXMLTitle(const char * cFileName)
{
	FILE *			fp;
	if (cFileName != NULL)
	{
		fp = fopen(cFileName,"w+");
		if (fp != NULL)
		{
			fprintf(fp,"%s","<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
			fprintf(fp,"%s\n","<Root>");
			fprintf(fp,"%s","</Root>");
			fclose(fp);
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
bool CXMLModule::LoadXML(const char * cFileName)
{
	SYSRETURN			hr;
	VARIANT_BOOL		bhr;
	VARIANT				vt; 
	if (cFileName != NULL && m_pDoc != NULL)
	{
		vt.vt = VT_BSTR; 
		vt.bstrVal = _com_util::ConvertStringToBSTR(cFileName);
		hr = m_pDoc->load(vt,&bhr);
		if (hr == S_OK)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}
bool CXMLModule::FindNodeList( const char * cNodeName )
{
	SYSRETURN					hr;
	if (m_pDoc != NULL && cNodeName != NULL)
	{
		hr = m_pDoc->getElementsByTagName(_com_util::ConvertStringToBSTR(cNodeName),&m_pNodeList);
		if (hr == S_OK)
		{
			return true;
		} 
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}
int CXMLModule::GetNodeListCount( void )
{
	long					m_iListLength;
	if (m_pNodeList != NULL)
	{
		m_pNodeList->get_length(&m_iListLength);
		return (int)m_iListLength;
	}
	else
	{
		return 0;
	}

}
bool CXMLModule::ChangeTextInNodeList(const char * cChangeText,int nIndexNode)
{
	SYSXMLNODE * 					m_pChangeNode = NULL;
	if (m_pNodeList != NULL && cChangeText != NULL )
	{
		if (nIndexNode >= 0 && nIndexNode < GetNodeListCount())
		{
			m_pNodeList->get_item(nIndexNode,&m_pChangeNode);
			if (m_pChangeNode != NULL)
			{
				m_pChangeNode->put_text(_com_util::ConvertStringToBSTR(cChangeText));
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}


}
bool CXMLModule::AppendNodeInNodeList( const char * cAppNode,int nIndexNode )
{
	SYSRETURN							hr;
	SYSXMLNODE *						m_pAppendNode;
	if (m_pNodeList != NULL && cAppNode != NULL)
	{
		if (nIndexNode >= 0 && nIndexNode <= GetNodeListCount())
		{
			SYSELEMENT * pSection = NULL; //´´½¨ÔªËØ
			hr = m_pDoc->createElement(_com_util::ConvertStringToBSTR(cAppNode), &pSection);
			if (hr == S_OK)
			{
				m_pNodeList->get_item(nIndexNode,&m_pNode);
				if (m_pNode != NULL )
				{
					hr = m_pNode->appendChild(pSection,&m_pAppendNode);
					if (hr == S_OK)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}
bool CXMLModule::RemoveNodeInNodeList(int nIndexNode)
{
	SYSXMLNODE * 					m_pDelNode;
	if (m_pNodeList != NULL)
	{
		if(nIndexNode >= 0 && nIndexNode < GetNodeListCount())
		{
			m_pNodeList->get_item(nIndexNode,&m_pDelNode);
			if (m_pNode != NULL )
			{
				m_pDelNode->removeChild(m_pDelNode,&m_pDelNode);
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
bool CXMLModule::SaveXML(_variant_t cFileName)
{
	HRESULT					hr;
	if (m_pDoc != NULL)
	{
		hr = m_pDoc->save(cFileName);
		if (hr == S_OK)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}
bool CXMLModule::SearchXMLTree(void)
{
	HRESULT					hr;
	if (m_pDoc != NULL) 
	{
		hr = m_pDoc->QueryInterface(IID_IXMLDOMNode,(void**)(&m_pNode));
		if (hr == S_OK)
		{
			for (int nCount = 0; nCount < (int)m_vTreeNode.size(); ++nCount)
			{
				delete m_vTreeNode[nCount];
				m_vTreeNode[nCount] = NULL;
			}
			m_vTreeNode.clear();
			this->SearchTree(m_pNode);
			this->UpdateNode();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
int CXMLModule::GetXMLCount(void)
{
	return (int)m_vTreeNode.size();
}
const char * CXMLModule::GetXMLNodeName(int nIndexNode)
{
	if (nIndexNode >= 0 && nIndexNode < (int)m_vTreeNode.size())
	{
		return m_vTreeNode[nIndexNode]->g_cNodeName.c_str();
	}
	else
	{
		return "\0";
	}
}
const char * CXMLModule::GetXMLNodeText(int nIndexNode)
{
	if (nIndexNode >= 0 && nIndexNode < (int)m_vTreeNode.size())
	{
		return m_vTreeNode[nIndexNode]->g_cNodeText.c_str();
	}
	else
	{
		return "\0";
	}
}
const char * CXMLModule::FindNodeText(string sNodeName)
{
	map<string,int>::iterator it = m_mTreeNode.find(sNodeName);
	if(it != m_mTreeNode.end())
	{
		return m_vTreeNode[it->second]->g_cNodeText.c_str();
	}
	else
	{
		return "\0";
	}
}
void CXMLModule::UpdateNode()
{
	m_mTreeNode.clear();
	for(int nCount = 0; nCount < (int)m_vTreeNode.size(); ++nCount)
	{
		m_mTreeNode.insert(make_pair(m_vTreeNode[nCount]->g_cNodeName,nCount));
	}
}
bool CXMLModule::SearchTree(SYSXMLNODE* pNode)
{
	SYSXMLNODE*		pNext;
	SYSXMLNODE*		pChild;
	BSTR			bNodeName,bNodeText;
	SYSNODETYPE		eEnum;
	static stXMLNode *	m_pXMLNode = NULL;
	pNode->get_nodeType(&eEnum);
	switch(eEnum)
	{
	case NODE_TEXT:
		{
			if (m_pXMLNode != NULL)
			{
				pNode->get_text(&bNodeText);
				m_pXMLNode->g_cNodeText.append(_com_util::ConvertBSTRToString(bNodeText));
				m_vTreeNode.push_back(m_pXMLNode);
				m_pXMLNode = NULL;
			}
		}break;
	case NODE_ELEMENT:
		{
			m_pXMLNode = new stXMLNode;
			if (m_pXMLNode != NULL)
			{
				pNode->get_nodeName(&bNodeName);
				m_pXMLNode->g_cNodeName.append(_com_util::ConvertBSTRToString(bNodeName));
			}
		}break;
	};


	this->SearchNode(pNode);
	pNode->get_firstChild(&pChild);
	while(pChild)
	{
		SearchTree(pChild);
		pChild->get_nextSibling(&pNext);
		pChild->Release();
		pChild=pNext;
	}
	return true;
}
bool CXMLModule::SearchNode(SYSXMLNODE* pNode)
{
	SYSNODEMAP*				pAttrs;
	BSTR					bNodeName,bNodeText;
	SYSNODETYPE				eEnum;
	static stXMLNode *	m_pXMLNode = NULL;
	if(SUCCEEDED(pNode->get_attributes(&pAttrs))&&(pAttrs!=NULL))
	{
		SYSXMLNODE* pChild;
		pAttrs->nextNode(&pChild);
		while(pChild)
		{
			pNode->get_nodeType(&eEnum);
			switch(eEnum)
			{
			case NODE_TEXT:
				{
					if (m_pXMLNode != NULL)
					{
						pNode->get_text(&bNodeText);
						m_pXMLNode->g_cNodeText.append(_com_util::ConvertBSTRToString(bNodeText));
						m_vTreeNode.push_back(m_pXMLNode);
						m_pXMLNode = NULL;
					}
				}break;
			case NODE_ELEMENT:
				{
					m_pXMLNode = new stXMLNode;
					if (m_pXMLNode != NULL)
					{
						pNode->get_nodeName(&bNodeName);
						m_pXMLNode->g_cNodeName.append(_com_util::ConvertBSTRToString(bNodeName));
					}
				}break;
			};
			pChild->Release();
			pAttrs->nextNode(&pChild);
		}
		pAttrs->Release();
	}
	return true;
}

CXMLModuleInterface * CreateXMLLib(void)
{
	static CXMLModuleInterface * pHandle =static_cast<CXMLModuleInterface *>(CXMLModule::GetXMLHandle());
	if (pHandle != NULL)
		return pHandle;
	else
		return NULL;
}
