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
	int type;//��Ϣ����
	int tag;//�˺�����ƥ���ʶ
	char fromID[6];//��ϢԴ�ͻ����û���
	char toID[6];//��ϢĿ�Ŀͻ����û���
	class UserMsg usermsg;
	char ip[32];
	char msg[1024];//��Ϣ
};

