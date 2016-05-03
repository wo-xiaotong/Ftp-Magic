#include<netdb.h>
#include<stdio.h>
#include"connect_func.h"
#include"log_manage.h"
#include"ftp_func.h"

static char read_buf[READ_BUF_SIZE];
static char write_buf[WRITE_BUF_SIZE];
static user_state login_state=LOGOUT;

int login_ftp(int control_fd,const char* user,const char* pass)
{
	snprintf(write_buf,WRITE_BUF_SIZE,"USER %s\r\n",user);
	write_socket(control_fd,write_buf);
	read_socket(control_fd,read_buf,READ_BUF_SIZE);
	if(strncmp(read_buf,LOGIN_NEED_PASS,3)!=0){
		log_console_debug(0,LOG_DEBUG("login user error"));
		return -1;
	}
	
	snprintf(write_buf,WRITE_BUF_SIZE,"PASS %s\r\n",pass);
	write_socket(control_fd,write_buf);
	read_socket(control_fd,read_buf,READ_BUF_SIZE);
	if(strncmp(read_buf,LOGIN_RIGHT,3)!=0){
		log_console_debug(0,LOG_DEBUG("login pass error"));
			return -1;
	}

	log_console(2,"LOGIN OK");
	login_state=LOGIN;
	return 0;
}

int connect_ftp(const char* ip,const int port)
{
	int control_fd=init_socket(ip,port);
	if(control_fd==-1){
		log_console_debug(0,LOG_DEBUG("init socket failed!"));
		return -1;
	}

	struct in_addr addr;
	inet_aton(ip,&addr);
	struct hostent* h_info=gethostbyaddr(&addr,4,AF_INET);	
	if(h_info==NULL){
		log_console_debug(0,LOG_DEBUG("get server name failed!"));
		return -1;
	}
	log_console(2,h_info->h_name);

	read_socket(control_fd,read_buf,READ_BUF_SIZE);
	log_console(2,read_buf);
	return control_fd;
}


int disconnect_ftp(const int control_fd)
{
	snprintf(write_buf,WRITE_BUF_SIZE,"QUIT\r\n");
	write_socket(control_fd,write_buf);
	read_socket(control_fd,read_buf,READ_BUF_SIZE);
	if(strncmp(read_buf,QUIT_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG("Quit failed!"));
		return -1;
	}

	login_state=LOGOUT;
	int n=close_socket(control_fd);
	
	log_console(2,"Quit Ok");
	return 0;
}
