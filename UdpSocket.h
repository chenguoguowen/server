#pragma once
// CUdpSocket ����Ŀ��

class CUdpSocket : public CSocket
{
public:
	CUdpSocket();  
	virtual ~CUdpSocket();
	virtual void OnReceive(int nErrorCode);

public:
	CString clientip;
	SOCKADDR_IN m_clientAddr;
};


