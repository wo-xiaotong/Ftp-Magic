#include <stdio.h>
#include "ftp_func.h"

int control_fd;
int data_fd;

#define IP "127.0.0.1"
#define C_PORT 21


int main(int argc,const char* argv[])
{
int i=10;
while(i--){
	control_fd=ftp_login(IP,C_PORT,"ftpuser","ftptest");
	
	/*
	ftp_syst(control_fd);
	ftp_noop(control_fd);
	ftp_pwd(control_fd);
	ftp_cwd(control_fd,"/");
	ftp_mkdir(control_fd,"/hello/world");
	ftp_rmdir(control_fd,"/hello/world");
	*/

	int d_port=goto_pasv_mode(control_fd);
	printf("%d\n",d_port);
	data_fd=open_data_connect(IP,d_port);
	close_data_connect(data_fd);

	ftp_logout(control_fd);
}
	return 0;
}

