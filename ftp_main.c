#include<stdio.h>
#include<netdb.h>
#include"connect_func.h"
#include"log_manage.h"

int connect_ftp(const char* ip,const int port);
int disconnect_ftp(const int control_fd);

int main()
{
	int cfd=connect_ftp("127.0.0.1",21);
	disconnect_ftp(cfd);
	return 0;
}

int connect_ftp(const char* ip,const int port)
{
	int control_fd=init_socket(ip,port);
	if(control_fd==-1){
		log_2_file(LOG_FILE,0,"control socket init faild!");
		return -1;
	}

	struct in_addr addr;
	inet_aton(ip,&addr);
	struct hostent* h_info=gethostbyaddr(&addr,4,AF_INET);	
	if(h_info==NULL){
		log_2_file(LOG_FILE,0,"gethostbyaddr failed");
		return -1;
	}
	log_2_file(LOG_FILE,2,h_info->h_name);

	char read_buf[BUFSIZ];
	read(control_fd,read_buf,BUFSIZ);
	log_2_file(LOG_FILE,1,read_buf);
	
	return control_fd;
}


int disconnect_ftp(const int control_fd)
{
	return close_socket(control_fd);
}



