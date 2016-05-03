#include<stdio.h>
#include "ftp_func.h"

int main(int argc,const char* argv[])
{

	int cfd=connect_ftp("127.0.0.1",21);
	login_ftp(cfd,"ftpuser","ftptest");
	disconnect_ftp(cfd);
	return 0;
}

