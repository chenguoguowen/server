
// ServerDlg.h : 头文件
//

#pragma once

#include "UdpSocket.h"
#include "TcpSocket.h"
#include "afxwin.h"
#include "afxcmn.h"

// CServerDlg 对话框
class CServerDlg : public CDialogEx
{
// 构造
public:
	CServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CUdpSocket *m_udpsocket;//UDP
	CTcpSocket *m_tcpsocket;//TCP
	CButton m_button1;//服务器控制
	CButton m_button2;//退出
	afx_msg void OnBnClickedControl();
	afx_msg void OnBnClickedQuit();
	CListCtrl m_listcontrol;//列表视图控件控制变量
	CString sPath;
	int tcp_tag;//TCP启动标识
	bool m_tag;//服务器启动标识
	afx_msg void OnClose();
	void GetPath(CString &sPath,CString filename);
	void Write();
};
