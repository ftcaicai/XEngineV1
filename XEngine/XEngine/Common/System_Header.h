#ifndef __SYSTEM_HEADER_H__
#define __SYSTEM_HEADER_H__
#include <Winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include <ctime>
#include <process.h>
#include <vector>
#include <map>
#include <list>
#include <strsafe.h>
#include <string>
#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include <stdlib.h>
#include <mbstring.h>
#include <sstream>
#include <d3d9.h>

#import "msxml3.dll" named_guids
#pragma comment(lib, "ws2_32.lib")
using namespace std;
#define		MAXCHARSIZE					256					/// ���������ַ�������
#define		MAXNETSIZE					8190				/// ���紫�䳤��

typedef		unsigned int						GUIDKEY;			/// GUIDKEY

typedef		HRESULT								SYSRETURN;				/// ����ֵ
typedef		IXMLDOMDocument						SYSXMLDOC;				/// XML�ĵ�ָ��
typedef		IXMLDOMNodeList						SYSXMLNODELIST;			/// XML�ڵ��б�ָ��
typedef		IXMLDOMNode			 				SYSXMLNODE;				/// XML�ڵ�ָ��
typedef		IXMLDOMElement						SYSELEMENT;				/// XMLԪ�ؽڵ�
typedef		DOMNodeType							SYSNODETYPE;			/// XMLԪ�ؽڵ�����
typedef		BSTR								SYSNODESTR;				/// XMLԪ�ؽڵ�ֵ
typedef		IXMLDOMNamedNodeMap					SYSNODEMAP;				/// XMLԪ�ؽڵ��ͼ

#endif