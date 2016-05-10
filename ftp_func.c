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
#include "utils.h"

static char read_buf[READ_BUFSIZE];
static user_info *ftp_user;

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

	ftp_user=(user_info*)malloc(sizeof(user_info));
	strcpy(ftp_user->user,user);
	strcpy(ftp_user->pass,pass);
	strcpy(ftp_user->ip,ip);
	ftp_user->ctrl_fd=ctrl_fd;
	ftp_user->login_state=LOGIN;

	return ctrl_fd;
}

int ftp_logout()
{
	int ret=check_reply_code(ftp_user->ctrl_fd,QUIT_OK,"QUIT\r\n");	
	if(ret!=0){
		log_console_debug(0,LOG_DEBUG("logout error"));
		return -1;
	}

	ftp_user->login_state=LOGOUT;
	log_console(2,"Quit Ok");

	int n=close_socket(ftp_user->ctrl_fd);
	if(n==-1){
		log_console_debug(0,LOG_DEBUG("close control socket failed!"));
		return -1;
	}
	
	return 0;
}

//return data port
int goto_pasv_mode()
{
	read_after_write(ftp_user->ctrl_fd,read_buf,READ_BUFSIZE,"PASV\r\n");	
	if(strncmp(read_buf,PASV_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG("got pasv mode error"));
		return -1;
	}

	int port=get_pasv_port(read_buf);
	ftp_user->login_state=PASV_MODE;
	ftp_user->d_port=port;

	return port;
}

int open_data_connect()
{
	int data_fd=init_socket(ftp_user->ip,ftp_user->d_port);
	if(data_fd==-1){
		log_console(0,"open data socket error!");
	}else{
		ftp_user->data_fd=data_fd;
	}
	
	return data_fd;
}

int close_data_connect()
{
	int ret=close_socket(ftp_user->data_fd);
	if(ret!=-1){
		log_console(2,"close data socket ok!");
	}else{
		ftp_user->login_state=LOGIN;
	}
	
	return ret;
}

int query_login_state()
{
	return ftp_user->login_state;
}

//print current working dirtory
int ftp_pwd()
{
	read_after_write(ftp_user->ctrl_fd,read_buf,READ_BUFSIZE,"PWD\r\n");	
	if(strncmp(read_buf,PWD_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG("pwd error"));
		return -1;
	}

	remove_reply_code(read_buf);
	log_console(2,read_buf);
	return 0;
}

//print system info
int ftp_syst()
{
	read_after_write(ftp_user->ctrl_fd,read_buf,READ_BUFSIZE,"SYST\r\n");	

	if(strncmp(read_buf,SYST_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG(read_buf));
		return -1;
	}

	remove_reply_code(read_buf);
	log_console(2,read_buf);
	
	return 0;
}

int ftp_mkdir(const char* dir_name)
{
	read_after_write(ftp_user->ctrl_fd,read_buf,READ_BUFSIZE,"MKD %s\r\n",dir_name);	

	if(strncmp(read_buf,MKD_OK,3)!=0){
		log_console_v(0,"mkdir %s failed!",dir_name);
		return -1;
	}
	
	return 0;	
}

int ftp_rmdir(const char* dir_name)
{
	read_after_write(ftp_user->ctrl_fd,read_buf,READ_BUFSIZE,"RMD %s\r\n",dir_name);	

	if(strncmp(read_buf,RMD_OK,3)!=0){
		log_console_v(0,"rmdir %s failed!",dir_name);
		return -1;
	}

	return 0;	
}

int ftp_cwd(const char* dir_name)
{
	int ret=check_reply_code(ftp_user->ctrl_fd,CWD_OK,"CWD %s\r\n",dir_name);	
	if(ret!=0){
		log_console_v(0,"change dir to %s failed!",dir_name);
		return -1;
	}

	return 0;	
}

int ftp_cdup(const char* dir_name)
{
	int ret=check_reply_code(ftp_user->ctrl_fd,CDUP_OK,"CDUP %s\r\n",dir_name);	
	if(ret!=0){
		log_console_v(0,"change parent dir to %s failed!",dir_name);
		return -1;
	}

	return 0;
}

int ftp_noop()
{
	int ret=check_reply_code(ftp_user->ctrl_fd,NOOP_OK,"NOOP\r\n");	
	if(ret!=0){
		log_console(0,"noop error!");
		return -1;
	}

	return 0;
}

//return bytes
int ftp_size_file(const char* file_name)
{
	read_after_write(ftp_user->ctrl_fd,read_buf,READ_BUFSIZE,"SIZE %s\r\n",file_name);

	if(compare_reply_code(read_buf,SIZE_OK)!=0){
		return -1;
	}
	
	remove_reply_code(read_buf);
	return atoi(read_buf);
}

int ftp_list(const char* df_name)
{
	if(ftp_user->login_state!=PASV_MODE){
		goto_pasv_mode(ftp_user->ctrl_fd);
		open_data_connect();
	}

	read_after_write(ftp_user->ctrl_fd,read_buf,READ_BUFSIZE,"LIST %s\r\n",df_name);
	log_console(2,read_buf);
	close_data_connect();

	return 0;
}

int ftp_retr_file(const char* file_name)
{
	return 0;
}
