#include "StdAfx.h"
#include "Head.h"


CHead::CHead(void)
{ 
	type = -1;
	tag = 1;
	strcpy_s(fromID,"");
	strcpy_s(toID,"");
	strcpy_s(ip,"");
	strcpy_s(msg,"");
	strcpy_s(usermsg.ID,"");
	strcpy_s(usermsg.NAME,"");
	strcpy_s(usermsg.PASSWORD,"");
}


CHead::~CHead(void)
{
}
