// TcpSocket.cpp : ʵ���ļ�
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


// CTcpSocket ��Ա����


void CTcpSocket::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CTcpSocketClient *m_client(0);
	m_client= new CTcpSocketClient(&m_listSockets);
	Accept(*m_client);
	m_listSockets.AddTail(m_client);
	CSocket::OnAccept(nErrorCode);
}
