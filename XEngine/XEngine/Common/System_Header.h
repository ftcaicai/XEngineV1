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
#define		MAXCHARSIZE					256					/// 定义最大的字符串长度
#define		MAXNETSIZE					8190				/// 网络传输长度

typedef		unsigned int						GUIDKEY;			/// GUIDKEY

typedef		HRESULT								SYSRETURN;				/// 返回值
typedef		IXMLDOMDocument						SYSXMLDOC;				/// XML文档指针
typedef		IXMLDOMNodeList						SYSXMLNODELIST;			/// XML节点列表指针
typedef		IXMLDOMNode			 				SYSXMLNODE;				/// XML节点指针
typedef		IXMLDOMElement						SYSELEMENT;				/// XML元素节点
typedef		DOMNodeType							SYSNODETYPE;			/// XML元素节点类型
typedef		BSTR								SYSNODESTR;				/// XML元素节点值
typedef		IXMLDOMNamedNodeMap					SYSNODEMAP;				/// XML元素节点地图

#endif