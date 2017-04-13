
// 服务器.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "ServerDlg.h"
#include "Head.h"

// CServerApp:
// 有关此类的实现，请参阅 Server.cpp
//

class CServerApp : public CWinApp
{
public:
	CServerApp();
	CServerDlg *active;
	CHead head;
	map<CString,CString> m_clientMessage;//记录当前连接的所有客户端信息
	vector<UserMsg> m_UserMsg;//账号密码
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CServerApp theApp;    //全局变量，作为信息的存储，extern为了防止重复定义