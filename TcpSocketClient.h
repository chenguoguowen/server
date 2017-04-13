#pragma once
#include "stdafx.h"
#include "Head.h"
// CTcpSocketClient 命令目标

typedef struct _SOCKET_STREAM_FILE_INFO {

	TCHAR       szFileTitle[128];                   //文件的标题名
	DWORD       dwFileAttributes;                   //文件的属性
	FILETIME    ftCreationTime;                     //文件的创建时间
	FILETIME    ftLastAccessTime;                   //文件的最后访问时间
	FILETIME    ftLastWriteTime;                    //文件的最后修改时间
	DWORD       nFileSizeHigh;                      //文件大小的高位双字
	DWORD       nFileSizeLow;                       //文件大小的低位双字
	DWORD       dwReserved0;                        //保留，为0
	DWORD       dwReserved1;                        //保留，为0

}SOCKET_STREAM_FILE_INFO, *PSOCKET_STREAM_FILE_INFO;

class CTcpSocketClient : public CSocket
{
public:
	CTcpSocketClient(CPtrList* pList); 
	virtual ~CTcpSocketClient();
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
public:
	CPtrList* m_pList;//保存服务器Socket中List的东西
	CString m_strID; //连接账号
	CString m_strIDtemp;
	CString m_strIp;//连接IP
	void SendMsg(CHead Msg);
	void SendFile(CHead Msg);
};


