// TcpSocketClient.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "TcpSocketClient.h"


// CTcpSocketClient

CTcpSocketClient::CTcpSocketClient(CPtrList* pList)
	:m_pList(pList)
{
}

CTcpSocketClient::~CTcpSocketClient()
{
}


// CTcpSocketClient 成员函数


void CTcpSocketClient::OnClose(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	theApp.m_clientMessage.erase(m_strID);
	theApp.head.type = 1;
	SendMessage(theApp.active->GetSafeHwnd(),WM_MSG,0,0);
	CHead Msg;
	Msg.type = 0;
	strcpy_s(Msg.usermsg.ID,(CStringA)m_strID);
	strcpy_s(Msg.ip,(CStringA)m_strIp);
	SendMsg(Msg);
	CSocket::OnClose(nErrorCode);
}


void CTcpSocketClient::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	int n,tag = 0;
	CHead Msg;
	n=Receive(&Msg,sizeof(Msg));
	if(Msg.type == 1)//登录信息
	{
		m_strID = Msg.usermsg.ID;
		m_strIp = Msg.ip;
		theApp.m_clientMessage.insert(make_pair(m_strID, m_strIp));
		theApp.head.type = 1;
		SendMessage(theApp.active->GetSafeHwnd(),WM_MSG,0,0);
	}
	if(Msg.type == 4)//验证信息
	{
		m_strID = Msg.usermsg.ID;
		m_strIp = Msg.ip;
		for(int i=0;i!=theApp.m_UserMsg.size();i++)
		{
			if(strcmp(theApp.m_UserMsg[i].ID,Msg.usermsg.ID) == 0&&strcmp(theApp.m_UserMsg[i].PASSWORD,Msg.usermsg.PASSWORD) == 0)
			{
				tag = 1;
				strcpy_s(Msg.usermsg.NAME,theApp.m_UserMsg[i].NAME);
				break;
			}
		}
		if(tag != 1)
			Msg.tag = 0;
	}
	if(Msg.type == 5)//注册信息
	{
		m_strIDtemp = Msg.msg;
		m_strIp = Msg.ip;
		CString IDtemp;
		int tagtemp = 0;
		while(tagtemp == 0)
		{
			IDtemp.Format(L"%d",rand()%90000+10000);
			int temp = 1;
			for(int i=0;i!=theApp.m_UserMsg.size();i++)
			{
				if(strcmp((CStringA)IDtemp,theApp.m_UserMsg[i].ID) == 0)
				{
					temp = 0;
					break;
				}
			}
			if(temp == 1)
			{
				strcpy_s(Msg.usermsg.ID,(CStringA)IDtemp);
				theApp.m_UserMsg.push_back(Msg.usermsg);
				tagtemp = 1;
			}
			else
				tagtemp = 0;
		}
	}
	SendMsg(Msg);
	CSocket::OnReceive(nErrorCode);
}

void CTcpSocketClient::SendMsg(CHead Msg)
{
	POSITION ps = m_pList->GetHeadPosition();  //取得，所有用户的队列
	switch(Msg.type)
	{
	case 0://有用户退出
	case 1://有用户上线
		while(ps!=NULL)
		{
			CTcpSocketClient* pTemp = (CTcpSocketClient*)m_pList->GetNext(ps);
			pTemp->Send(&Msg,sizeof(Msg));
		}
		break;
	case 2://已在线用户告知刚上线用户我在线
	case 3://发送消息
		while(ps!=NULL)
		{
			CTcpSocketClient* pTemp = (CTcpSocketClient*)m_pList->GetNext(ps);
			if(pTemp->m_strID == Msg.toID)
				pTemp->Send(&Msg,sizeof(Msg));
		}
		break;
	case 4:
		while(ps!=NULL)
		{
			CTcpSocketClient* pTemp = (CTcpSocketClient*)m_pList->GetNext(ps);
			if(pTemp->m_strID == Msg.fromID)
				pTemp->Send(&Msg,sizeof(Msg));
		}
		break;
	case 5:
		while(ps!=NULL)
		{
			CTcpSocketClient* pTemp = (CTcpSocketClient*)m_pList->GetNext(ps);
			if(pTemp->m_strIDtemp == Msg.msg)
			{
				pTemp->Send(&Msg,sizeof(Msg));
				pTemp->m_strIDtemp = L"";
			}
		}
		break;

	case 6:  //文件传输
		SendFile(Msg);
		break;

	default:
		break;
	}
}

void CTcpSocketClient::SendFile(CHead Msg)
{
	CTcpSocketClient* pTemp = NULL;
	POSITION ps = m_pList->GetHeadPosition();  //取得，所有用户的队列
	while (ps != NULL)
	{
		pTemp = (CTcpSocketClient*)m_pList->GetNext(ps);
		//pTemp->m_strID == Msg.fromID;
		if (pTemp->m_strID == Msg.toID)
		{
			pTemp->Send(&Msg, sizeof(Msg));
			break;
		}
	}

	if (pTemp == NULL)
	{
		return;
	}

	SOCKET_STREAM_FILE_INFO StreamFileInfo;

	Receive(&StreamFileInfo, sizeof(SOCKET_STREAM_FILE_INFO));

	CFile destFile(StreamFileInfo.szFileTitle, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyNone);

	UINT dwRead = 0;
	while (dwRead<StreamFileInfo.nFileSizeLow)
	{
		byte* data = new byte[1024];
		memset(data, 0, 1024);

		UINT dw = Receive(data, 1024);
		destFile.Write(data, dw);

		dwRead += dw;
	}

	SetFileTime((HANDLE)destFile.m_hFile, &StreamFileInfo.ftCreationTime,
		&StreamFileInfo.ftLastAccessTime, &StreamFileInfo.ftLastWriteTime);

	SetFileAttributes(StreamFileInfo.szFileTitle,StreamFileInfo.dwFileAttributes);

	destFile.Close();

	//接受完成后转发至目标客户中

	CFile file;
	if (!file.Open(StreamFileInfo.szFileTitle, CFile::modeRead))
	{
		AfxMessageBox(L"无指定文件");
		return;
	}
	SOCKET_STREAM_FILE_INFO	StreamFileInfo1;
	WIN32_FIND_DATA             FindFileData;

	FindClose(FindFirstFile(StreamFileInfo.szFileTitle, &FindFileData));
	memset(&StreamFileInfo1, 0, sizeof(SOCKET_STREAM_FILE_INFO));
	//StreamFileInfo.szFileTitle = myFile.GetFileTitle();
	wsprintf(StreamFileInfo1.szFileTitle, L"%s", file.GetFileTitle());
	//strcpy_s(StreamFileInfo.szFileTitle, myFile.GetFileTitle());

	int a = 0;
	StreamFileInfo1.dwFileAttributes = FindFileData.dwFileAttributes;
	StreamFileInfo1.ftCreationTime = FindFileData.ftCreationTime;
	StreamFileInfo1.ftLastAccessTime = FindFileData.ftLastAccessTime;
	StreamFileInfo1.ftLastWriteTime = FindFileData.ftLastWriteTime;
	StreamFileInfo1.nFileSizeHigh = FindFileData.nFileSizeHigh;
	StreamFileInfo1.nFileSizeLow = FindFileData.nFileSizeLow;

	
	pTemp->Send(&StreamFileInfo1, sizeof(SOCKET_STREAM_FILE_INFO));


    dwRead = 0;
	while (dwRead<StreamFileInfo1.nFileSizeLow)
	{
		byte* data = new byte[1024];
		UINT dw = file.Read(data, 1024);
		pTemp->Send(data, dw);
		dwRead += dw;
	}
	file.Close();
}