#pragma once
#include "TcpSocketClient.h"
// CTcpSocket ����Ŀ��

class CTcpSocket : public CSocket
{ 
public:
	CTcpSocket();
	virtual ~CTcpSocket();
	virtual void OnAccept(int nErrorCode);
	CPtrList m_listSockets;//������������������пͻ������ӳɹ����Socket
};