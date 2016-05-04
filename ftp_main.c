#include<stdio.h>
#include "ftp_func.h"

int control_fd;
int data_fd;

#define IP "127.0.0.1"
#define C_PORT 21

int main(int argc,const char* argv[])
{

	control_fd=login_ftp(IP,C_PORT,"ftpuser","ftptest");
	
	int data_port=goto_PASV(control_fd);
	printf("%d",data_port);
	data_fd=connect_data(IP,data_port);
	close_data(data_fd);

	logout_ftp(control_fd);
	printf("%d\n",query_login_state());
	return 0;
}

