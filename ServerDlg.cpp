
// ServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// C服务器Dlg 对话框




CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	theApp.active = this;
	m_tag = false;//服务器启动标识初始化为0
	m_udpsocket = NULL;
	m_tcpsocket = NULL;
	tcp_tag = 0;//tcp启动标识
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


// CServerDlg 消息处理程序

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetPath(sPath,L"\\res\\UserMsg.txt");  //文件操作
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
	// 获取编程语言列表视图控件的位置和大小
	m_listcontrol.GetClientRect(&rect);
	// 为列表视图控件添加全行选中和栅格风格
	m_listcontrol.SetExtendedStyle(m_listcontrol.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 为列表视图控件添加两列
	m_listcontrol.InsertColumn(0, L"在线用户", LVCFMT_CENTER, rect.Width()/2, 1);
	m_listcontrol.InsertColumn(1, L"在线用户", LVCFMT_CENTER, rect.Width()/2, 1);
	m_listcontrol.InsertColumn(2, L"用户IP", LVCFMT_CENTER, rect.Width()/2, 1);
	m_listcontrol.DeleteColumn(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServerDlg::GetPath(CString &sPath,CString filename)
{
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);//返回该当前应用程序*.exe的全路径
	sPath.ReleaseBuffer();
	int nPos;
	nPos=sPath.ReverseFind ('\\');//ReverseFind()在一个较大的字符串中从末端开始查找某个字符
	sPath=sPath.Left(nPos);
	nPos=sPath.ReverseFind ('\\');
	sPath=sPath.Left(nPos);
	sPath+=filename;
}

LRESULT CServerDlg::OnMsg(WPARAM wParam,LPARAM lParam)//自定义消息响应
{
	map<CString,CString>::iterator it;
	int i;
	switch(theApp.head.type)
	{
	case 0://启动TCP
		if(tcp_tag == 0)//tcp启动标识，每次启动服务器，tcp套接字只create一次
		{
			m_tcpsocket = new CTcpSocket();
			if(!m_tcpsocket->Create(1003))   //端口号
			{
				MessageBox(L"创建套接字错误！");
				m_tcpsocket->Close();
			}
			else if(!m_tcpsocket->Listen())
			{
				MessageBox(L"监听失败！");
				m_tcpsocket->Close();
			}
			tcp_tag = 1;
		}
		break;
	case 1://更新在线用户信息
		// 在列表视图控件中插入列表项，并设置列表子项文本
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CServerDlg::OnBnClickedControl()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_tag)//服务器未启动
	{
		if(m_udpsocket == NULL)
		{
			m_udpsocket = new CUdpSocket();
			m_udpsocket->Create(1001,SOCK_DGRAM);
		}
		m_button1.SetWindowTextW(L"断开");
		m_tag = true;
		m_button2.EnableWindow(false);
	}
	else//服务器已启动
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
		m_button1.SetWindowTextW(L"启动");
		m_tag = false;
		tcp_tag = 0;
		m_listcontrol.DeleteAllItems();
		m_button2.EnableWindow(true);
	}
}


void CServerDlg::OnBnClickedQuit()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_tag)
		return;
	Write();
	CDialogEx::OnClose();
}
