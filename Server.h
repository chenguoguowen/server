
// ������.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "ServerDlg.h"
#include "Head.h"

// CServerApp:
// �йش����ʵ�֣������ Server.cpp
//

class CServerApp : public CWinApp
{
public:
	CServerApp();
	CServerDlg *active;
	CHead head;
	map<CString,CString> m_clientMessage;//��¼��ǰ���ӵ����пͻ�����Ϣ
	vector<UserMsg> m_UserMsg;//�˺�����
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CServerApp theApp;    //ȫ�ֱ�������Ϊ��Ϣ�Ĵ洢��externΪ�˷�ֹ�ظ�����