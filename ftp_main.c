#include <stdio.h>
#include "ftp_func.h"

int control_fd;
int data_fd;

#define IP "127.0.0.1"
#define C_PORT 21


int main(int argc,const char* argv[])
{
	control_fd=ftp_login(IP,C_PORT,"ftpuser","ftptest");
	

	ftp_logout(control_fd);

	return 0;
}

