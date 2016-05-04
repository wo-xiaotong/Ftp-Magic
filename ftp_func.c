#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"connect_func.h"
#include"log_manage.h"
#include"ftp_func.h"

static char read_buf[READ_BUF_SIZE];
static char write_buf[WRITE_BUF_SIZE];
static user_state login_state=LOGOUT;

int login_ftp(const char* ip,int c_port,const char* user,const char* pass)
{
	int ctrl_fd=init_socket(ip,c_port);
	if(ctrl_fd==-1)return -1;

	struct in_addr addr;
	inet_aton(ip,&addr);
	struct hostent* h_info=gethostbyaddr(&addr,4,AF_INET);	
	if(h_info==NULL){
		log_console_debug(0,LOG_DEBUG("get server name failed!"));
		return -1;
	}
	log_console(2,h_info->h_name);

	read_socket(ctrl_fd,read_buf,READ_BUF_SIZE);
	log_console(2,read_buf);

	snprintf(write_buf,WRITE_BUF_SIZE,"USER %s\r\n",user);
	write_socket(ctrl_fd,write_buf);
	read_socket(ctrl_fd,read_buf,READ_BUF_SIZE);
	if(strncmp(read_buf,LOGIN_NEED_PASS,3)!=0){
		log_console_debug(0,LOG_DEBUG(read_buf));
		return -1;
	}
	
	snprintf(write_buf,WRITE_BUF_SIZE,"PASS %s\r\n",pass);
	write_socket(ctrl_fd,write_buf);
	read_socket(ctrl_fd,read_buf,READ_BUF_SIZE);
	if(strncmp(read_buf,LOGIN_RIGHT,3)!=0){
		log_console_debug(0,LOG_DEBUG(read_buf));
			return -1;
	}

	log_console(2,"LOGIN OK");
	login_state=LOGIN;

	return ctrl_fd;
}

int logout_ftp(const int ctrl_fd)
{
	snprintf(write_buf,WRITE_BUF_SIZE,"QUIT\r\n");
	write_socket(ctrl_fd,write_buf);
	read_socket(ctrl_fd,read_buf,READ_BUF_SIZE);
	if(strncmp(read_buf,QUIT_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG(read_buf));
		return -1;
	}

	login_state=LOGOUT;
	log_console(2,"Quit Ok");

	int n=close_socket(ctrl_fd);
	if(n==-1){
		log_console_debug(0,LOG_DEBUG("close socket failed!"));
		return -1;
	}
	
	return 0;
}

//return data port
int goto_PASV(const int ctrl_fd)
{
	snprintf(write_buf,WRITE_BUF_SIZE,"PASV\r\n");
	write_socket(ctrl_fd,write_buf);
	read_socket(ctrl_fd,read_buf,READ_BUF_SIZE);

	if(strncmp(read_buf,PASV_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG(read_buf));
		return -1;
	}
	
	log_console(2,read_buf);
	char* p=strtok(read_buf," ,.()");
	int cnt=7;
	while (cnt-- && p!=NULL){
		p=strtok (NULL," ,.()");
	}
	char* p1=strtok(NULL," ,.()");
	char* p2=strtok(NULL," ,.()");
	
	return atoi(p1)*256+atoi(p2);
}

int connect_data(const char* IP,int d_port)
{
	int data_fd=init_socket(IP,d_port);
	log_console(2,"open data socket ok!");

	return data_fd;
}

int close_data(const int data_fd)
{
	int ret=close_socket(data_fd);
	log_console(2,"close data socket ok!");

	return ret;
}

int query_login_state()
{
	return login_state;
}

int 
