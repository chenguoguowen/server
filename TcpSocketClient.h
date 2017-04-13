#pragma once
#include "stdafx.h"
#include "Head.h"
// CTcpSocketClient ����Ŀ��

typedef struct _SOCKET_STREAM_FILE_INFO {

	TCHAR       szFileTitle[128];                   //�ļ��ı�����
	DWORD       dwFileAttributes;                   //�ļ�������
	FILETIME    ftCreationTime;                     //�ļ��Ĵ���ʱ��
	FILETIME    ftLastAccessTime;                   //�ļ���������ʱ��
	FILETIME    ftLastWriteTime;                    //�ļ�������޸�ʱ��
	DWORD       nFileSizeHigh;                      //�ļ���С�ĸ�λ˫��
	DWORD       nFileSizeLow;                       //�ļ���С�ĵ�λ˫��
	DWORD       dwReserved0;                        //������Ϊ0
	DWORD       dwReserved1;                        //������Ϊ0

}SOCKET_STREAM_FILE_INFO, *PSOCKET_STREAM_FILE_INFO;

class CTcpSocketClient : public CSocket
{
public:
	CTcpSocketClient(CPtrList* pList); 
	virtual ~CTcpSocketClient();
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
public:
	CPtrList* m_pList;//���������Socket��List�Ķ���
	CString m_strID; //�����˺�
	CString m_strIDtemp;
	CString m_strIp;//����IP
	void SendMsg(CHead Msg);
	void SendFile(CHead Msg);
};


