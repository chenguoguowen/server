
// ServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// C������Dlg �Ի���




CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	theApp.active = this;
	m_tag = false;//������������ʶ��ʼ��Ϊ0
	m_udpsocket = NULL;
	m_tcpsocket = NULL;
	tcp_tag = 0;//tcp������ʶ
	srand((unsigned)time(0));
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTROL, m_button1);
	DDX_Control(pDX, IDC_QUIT, m_button2);
	DDX_Control(pDX, IDC_CLIENTMSG, m_listcontrol);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONTROL, &CServerDlg::OnBnClickedControl)
	ON_BN_CLICKED(IDC_QUIT, &CServerDlg::OnBnClickedQuit)
	ON_MESSAGE(WM_MSG,OnMsg)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CServerDlg ��Ϣ�������

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetPath(sPath,L"\\res\\UserMsg.txt");  //�ļ�����
	fstream infile(sPath,fstream::in);
	while(!infile.eof())
	{
		UserMsg temp;
		string id,name,password;
		infile>>id>>name>>password;
		strcpy_s(temp.ID,id.c_str());
		strcpy_s(temp.NAME,name.c_str());
		strcpy_s(temp.PASSWORD,password.c_str());
		theApp.m_UserMsg.push_back(temp);
	}
	infile.close();
	CRect rect;
	// ��ȡ��������б���ͼ�ؼ���λ�úʹ�С
	m_listcontrol.GetClientRect(&rect);
	// Ϊ�б���ͼ�ؼ����ȫ��ѡ�к�դ����
	m_listcontrol.SetExtendedStyle(m_listcontrol.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// Ϊ�б���ͼ�ؼ��������
	m_listcontrol.InsertColumn(0, L"�����û�", LVCFMT_CENTER, rect.Width()/2, 1);
	m_listcontrol.InsertColumn(1, L"�����û�", LVCFMT_CENTER, rect.Width()/2, 1);
	m_listcontrol.InsertColumn(2, L"�û�IP", LVCFMT_CENTER, rect.Width()/2, 1);
	m_listcontrol.DeleteColumn(0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CServerDlg::GetPath(CString &sPath,CString filename)
{
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);//���ظõ�ǰӦ�ó���*.exe��ȫ·��
	sPath.ReleaseBuffer();
	int nPos;
	nPos=sPath.ReverseFind ('\\');//ReverseFind()��һ���ϴ���ַ����д�ĩ�˿�ʼ����ĳ���ַ�
	sPath=sPath.Left(nPos);
	nPos=sPath.ReverseFind ('\\');
	sPath=sPath.Left(nPos);
	sPath+=filename;
}

LRESULT CServerDlg::OnMsg(WPARAM wParam,LPARAM lParam)//�Զ�����Ϣ��Ӧ
{
	map<CString,CString>::iterator it;
	int i;
	switch(theApp.head.type)
	{
	case 0://����TCP
		if(tcp_tag == 0)//tcp������ʶ��ÿ��������������tcp�׽���ֻcreateһ��
		{
			m_tcpsocket = new CTcpSocket();
			if(!m_tcpsocket->Create(1003))   //�˿ں�
			{
				MessageBox(L"�����׽��ִ���");
				m_tcpsocket->Close();
			}
			else if(!m_tcpsocket->Listen())
			{
				MessageBox(L"����ʧ�ܣ�");
				m_tcpsocket->Close();
			}
			tcp_tag = 1;
		}
		break;
	case 1://���������û���Ϣ
		// ���б���ͼ�ؼ��в����б���������б������ı�
		m_listcontrol.DeleteAllItems();

		for(it = theApp.m_clientMessage.begin(),i = 0;it != theApp.m_clientMessage.end();it++,i++)
		{
			m_listcontrol.InsertItem(i, it->first);
			m_listcontrol.SetItemText(i, 1, it->second);
		}
		break;
	default:
		break;
	}
	return TRUE;
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerDlg::OnBnClickedControl()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(!m_tag)//������δ����
	{
		if(m_udpsocket == NULL)
		{
			m_udpsocket = new CUdpSocket();
			m_udpsocket->Create(1001,SOCK_DGRAM);
		}
		m_button1.SetWindowTextW(L"�Ͽ�");
		m_tag = true;
		m_button2.EnableWindow(false);
	}
	else//������������
	{
		if(m_udpsocket != NULL)
		{
			m_udpsocket->Close();
			delete m_udpsocket;
			m_udpsocket = NULL;
		}
		if(m_tcpsocket != NULL)
		{
			m_tcpsocket->Close();
			delete m_tcpsocket;
			m_tcpsocket = NULL;
		}
		m_button1.SetWindowTextW(L"����");
		m_tag = false;
		tcp_tag = 0;
		m_listcontrol.DeleteAllItems();
		m_button2.EnableWindow(true);
	}
}


void CServerDlg::OnBnClickedQuit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Write();
	exit(0);
}

void CServerDlg::Write()
{
	if(!theApp.m_UserMsg.empty())
	{
		fstream outfile(sPath,fstream::out);
		outfile<<theApp.m_UserMsg[0].ID<<'\t'<<theApp.m_UserMsg[0].NAME<<'\t'<<theApp.m_UserMsg[0].PASSWORD;
		for(int i = 1;i != theApp.m_UserMsg.size();i++)
		{
			outfile<<endl<<theApp.m_UserMsg[i].ID<<'\t'<<theApp.m_UserMsg[i].NAME<<'\t'<<theApp.m_UserMsg[i].PASSWORD;
		}
		outfile.close();
	}
}

void CServerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_tag)
		return;
	Write();
	CDialogEx::OnClose();
}
