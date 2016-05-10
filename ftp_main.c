#include <stdio.h>
#include "ftp_func.h"

#define IP "127.0.0.1"
#define C_PORT 21

int main(int argc,const char* argv[])
{
	ftp_login(IP,C_PORT,"ftpuser","ftptest");
	

	ftp_syst( );
	ftp_noop( );
	ftp_pwd( );
	ftp_cwd("/");
	ftp_mkdir( "/hello/world");
	ftp_rmdir( "/hello/world");

	ftp_list("/");


	ftp_logout();
	return 0;
}

