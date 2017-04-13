#pragma once
#include "TcpSocketClient.h"
// CTcpSocket 命令目标

class CTcpSocket : public CSocket
{ 
public:
	CTcpSocket();
	virtual ~CTcpSocket();
	virtual void OnAccept(int nErrorCode);
	CPtrList m_listSockets;//用来保存服务器与所有客户端连接成功后的Socket
};