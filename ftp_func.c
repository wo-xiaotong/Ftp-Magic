#define _BSD_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include "connect_func.h"
#include "log_manage.h"
#include "ftp_func.h"
#include "reply_deal.h"

static char read_buf[READ_BUFSIZE];
static user_state login_state=LOGOUT;

int ftp_login(const char* ip,int c_port,const char* user,const char* pass)
{
	int ctrl_fd=init_socket(ip,c_port);
	if(ctrl_fd==-1){
		return -1;
	}
	read_socket(ctrl_fd,read_buf,READ_BUFSIZE);
	log_console(2,read_buf);

	struct in_addr addr;
	inet_aton(ip,&addr);
	struct hostent* h_info=gethostbyaddr(&addr,4,AF_INET);	
	if(h_info==NULL){
		log_console_debug(0,LOG_DEBUG("get server name failed!"));
		return -1;
	}
	log_console(2,h_info->h_name);

	int u=check_reply_code(ctrl_fd,LOGIN_NEED_PASS,"USER %s\r\n",user);	
	int p=check_reply_code(ctrl_fd,LOGIN_RIGHT,"PASS %s\r\n",pass);	
	if(u!=0 || p!=0){
		log_console_debug(0,LOG_DEBUG("login error"));
		return -1;
	}
	
	log_console(2,"LOGIN OK");
	login_state=LOGIN;

	return ctrl_fd;
}

int ftp_logout(const int ctrl_fd)
{
	int ret=check_reply_code(ctrl_fd,QUIT_OK,"QUIT\r\n");	
	if(ret!=0){
		log_console_debug(0,LOG_DEBUG("logout error"));
	}

	login_state=LOGOUT;
	log_console(2,"Quit Ok");

	int n=close_socket(ctrl_fd);
	if(n==-1){
		log_console_debug(0,LOG_DEBUG("close control socket failed!"));
		return -1;
	}
	
	return 0;
}

//return data port
int goto_PASV(const int ctrl_fd)
{
	read_after_write(ctrl_fd,read_buf,READ_BUFSIZE,"PASV\r\n");	
	if(strncmp(read_buf,PASV_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG("got pasv mode error"));
		return -1;
	}

	log_console(2,read_buf);
	int port=get_pasv_port(read_buf);
	login_state=PASV_MODE;

	return port;
}

int open_data_connect(const char* IP,int d_port)
{
	int data_fd=init_socket(IP,d_port);
	if(data_fd!=-1){
		log_console(2,"open data socket ok!");
	}
	
	return data_fd;
}

int close_data_connect(const int data_fd)
{
	int ret=close_socket(data_fd);
	if(ret!=-1){
		log_console(2,"close data socket ok!");
	}

	return ret;
}

int query_login_state()
{
	return login_state;
}

int print_working_dir(const int ctrl_fd)
{
	read_after_write(ctrl_fd,read_buf,READ_BUFSIZE,"PWD\r\n");	
	if(strncmp(read_buf,PWD_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG("pwd error"));
		return -1;
	}

	remove_reply_code(read_buf);
	log_console(2,read_buf);
	return 0;
}

int print_sysinfo(const int ctrl_fd)
{
	read_after_write(ctrl_fd,read_buf,READ_BUFSIZE,"SYST\r\n");	

	if(strncmp(read_buf,SYST_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG(read_buf));
		return -1;
	}

	remove_reply_code(read_buf);
	log_console(2,read_buf);
	return 0;
}

int ftp_mkdir(const int ctrl_fd,const char* dir_name)
{
	read_after_write(ctrl_fd,read_buf,READ_BUFSIZE,"MKD %s\r\n",dir_name);	

	if(strncmp(read_buf,MKD_OK,3)!=0){
		log_console_debug(0,LOG_DEB
	return 0;	
}

int ftp_rmdir(const int ctrl_fd,const char* dir_name)
{
	snprintf(write_buf,WRITE_BUF_SIZE,"RMD %s\r\n",dir_name);
	write_socket(ctrl_fd,write_buf);
	read_socket(ctrl_fd,read_buf,READ_BUFSIZE);

	if(strncmp(read_buf,RMD_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG(read_buf));
		return -1;
	}

	remove_reply_code(read_buf);
	log_console(2,read_buf);
	return 0;	
}

int ftp_cwd(const int ctrl_fd,const char* dir_name)
{
	snprintf(write_buf,WRITE_BUF_SIZE,"CWD %s\r\n",dir_name);
	write_socket(ctrl_fd,write_buf);
	read_socket(ctrl_fd,read_buf,READ_BUFSIZE);

	if(strncmp(read_buf,CWD_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG(read_buf));
		return -1;
	}

	log_console(2,read_buf);
	return 0;	
}

int ftp_cdup(const int ctrl_fd,const char* dir_name)
{
	snprintf(write_buf,WRITE_BUF_SIZE,"CDUP %s\r\n",dir_name);
	write_socket(ctrl_fd,write_buf);
	read_socket(ctrl_fd,read_buf,READ_BUFSIZE);

	if(strncmp(read_buf,CDUP_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG(read_buf));
		return -1;
	}

	log_console(2,read_buf);
	return 0;
}

int ftp_noop(const int ctrl_fd)
{
	snprintf(write_buf,WRITE_BUF_SIZE,"NOOP\r\n");
	write_socket(ctrl_fd,write_buf);
	read_socket(ctrl_fd,read_buf,READ_BUFSIZE);

	if(strncmp(read_buf,NOOP_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG(read_buf));
		return -1;
	}

	return 0;
}


