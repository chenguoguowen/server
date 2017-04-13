#pragma once
class UserMsg
{
public:
	char ID[6];
	char NAME[21];
	char PASSWORD[21];
};
class CHead
{
public:
	CHead(void); 
	~CHead(void);
public:
	int type;//信息类型
	int tag;//账号密码匹配标识
	char fromID[6];//信息源客户端用户名
	char toID[6];//信息目的客户端用户名
	class UserMsg usermsg;
	char ip[32];
	char msg[1024];//信息
};

