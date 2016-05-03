#include<netdb.h>
#include<stdio.h>
#include"connect_func.h"
#include"log_manage.h"
#include"ftp_func.h"

static char read_buf[READ_BUF_SIZE];
static char write_buf[WRITE_BUF_SIZE];
static enum state login_state=LOGOUT;

int login_ftp(int control_fd,const char* user,const char* pass)
{
	snprintf(write_buf,WRITE_BUF_SIZE,"USER %s\r\n",user);
	write_socket(control_fd,write_buf);
	read_socket(control_fd,read_buf,READ_BUF_SIZE);
	if(strncmp(read_buf,LOGIN_NEED_PASS,3)!=0){
		return -1;
	}
	 (LOG_FILE,"%s-%s :%s",__FILE__,__LINE__,read_buf);

	snprintf(write_buf,WRITE_BUF_SIZE,"PASS %s\r\n",pass);
	write_socket(control_fd,write_buf);
	read_socket(control_fd,read_buf,READ_BUF_SIZE);
	if(strncmp(read_buf,LOGIN_RIGHT,3)!=0){
			return -1;
	}

	login_state=LOGIN;
	return 0;
}

int connect_ftp(const char* ip,const int port)
{
	int control_fd=init_socket(ip,port);
	if(control_fd==-1){
			return -1;
	}

	struct in_addr addr;
	inet_aton(ip,&addr);
	struct hostent* h_info=gethostbyaddr(&addr,4,AF_INET);	
	if(h_info==NULL){
		return -1;
	}

	char read_buf[BUFSIZ];
	read(control_fd,read_buf,BUFSIZ);
	
	return control_fd;
}


int disconnect_ftp(const int control_fd)
{
	snprintf(write_buf,WRITE_BUF_SIZE,"QUIT\r\n");
	write_socket(control_fd,write_buf);
	read_socket(control_fd,read_buf,READ_BUF_SIZE);
	if(strncmp(read_buf,QUIT_OK,3)!=0){
		return -1;
	}

	login_state=LOGOUT;
	return close_socket(control_fd);
}



