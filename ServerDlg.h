
// ServerDlg.h : ͷ�ļ�
//

#pragma once

#include "UdpSocket.h"
#include "TcpSocket.h"
#include "afxwin.h"
#include "afxcmn.h"

// CServerDlg �Ի���
class CServerDlg : public CDialogEx
{
// ����
public:
	CServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnMsg(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CUdpSocket *m_udpsocket;//UDP
	CTcpSocket *m_tcpsocket;//TCP
	CButton m_button1;//����������
	CButton m_button2;//�˳�
	afx_msg void OnBnClickedControl();
	afx_msg void OnBnClickedQuit();
	CListCtrl m_listcontrol;//�б���ͼ�ؼ����Ʊ���
	CString sPath;
	int tcp_tag;//TCP������ʶ
	bool m_tag;//������������ʶ
	afx_msg void OnClose();
	void GetPath(CString &sPath,CString filename);
	void Write();
};
