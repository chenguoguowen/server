// UdpSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "UdpSocket.h"


// CUdpSocket

CUdpSocket::CUdpSocket()
{
}

CUdpSocket::~CUdpSocket()
{
}


// CUdpSocket 成员函数


void CUdpSocket::OnReceive(int nErrorCode)//服务器监听端口1001，发送端口1000
{
	// TODO: 在此添加专用代码和/或调用基类
	TCHAR recBuf[1024] = {0};
	int len = sizeof(SOCKADDR_IN);
	int recBytes = ReceiveFrom(recBuf,1023,(SOCKADDR*)&m_clientAddr,&len,0);
	clientip = inet_ntoa(m_clientAddr.sin_addr);
  	if(wcscmp(recBuf,L"客户端查找服务器") == 0)//宽字符串比较
	{
		CString str = L"服务器应答";
		SendTo(str,2*str.GetLength(),1000,clientip);
		//发送启动TCP消息
		theApp.head.type = 0;//消息类型设为0
		SendMessage(theApp.active->GetSafeHwnd(),WM_MSG,0,0);
	}
	CSocket::OnReceive(nErrorCode);
}