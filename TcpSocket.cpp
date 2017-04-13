// TcpSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "TcpSocket.h"


// CTcpSocket

CTcpSocket::CTcpSocket()
{

}

CTcpSocket::~CTcpSocket()
{

}


// CTcpSocket 成员函数


void CTcpSocket::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CTcpSocketClient *m_client(0);
	m_client= new CTcpSocketClient(&m_listSockets);
	Accept(*m_client);
	m_listSockets.AddTail(m_client);
	CSocket::OnAccept(nErrorCode);
}
