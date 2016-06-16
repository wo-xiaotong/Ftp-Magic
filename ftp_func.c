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
static user_info *ftp_user=NULL;

//return data port
static int goto_pasv_mode()
{
	read_after_write(ftp_user->ctrl_fd,read_buf,READ_BUFSIZE,"PASV\r\n");	
	if(strncmp(read_buf,PASV_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG("got pasv mode error"));
		return -1;
	}

	int port=get_pasv_port(read_buf);

	return port;
}

int ftp_login(const char* ip,int c_port,const char* user,const char* pass)
{
	static int login_times=0;

	int ctrl_fd=init_socket(ip,c_port);
	if(ctrl_fd==-1){
		return -1;
	}
	read_socket(ctrl_fd,read_buf,READ_BUFSIZE);

	int u=check_reply_code(ctrl_fd,LOGIN_NEED_PASS,"USER %s\r\n",user);	
	int p=check_reply_code(ctrl_fd,LOGIN_RIGHT,"PASS %s\r\n",pass);	
	if(u!=0 || p!=0){
		log_console_debug(0,LOG_DEBUG("login error"));
		return -1;
	}
	
	if(ftp_user==NULL)ftp_user=(user_info*)malloc(sizeof(user_info));
	strncpy(ftp_user->user,user,sizeof(ftp_user->user));
	strncpy(ftp_user->pass,pass,sizeof(ftp_user->pass));
	strncpy(ftp_user->ip,ip,sizeof(ftp_user->ip));
	ftp_user->ctrl_fd=ctrl_fd;
	ftp_user->c_port=c_port;
	
	if(!login_times){
		log_console(2,read_buf);
		log_console(2,"LOGIN OK");
		ftp_syst();
	}
	++login_times;

	return ctrl_fd;
}

void ftp_get_user()
{
	log_console_v(2,"%s %s",ftp_user->user,ftp_user->ip);
}

//by send noop to check if the client still connects to server
void ftp_check_connect()
{
	int ret=check_reply_code(ftp_user->ctrl_fd,NOOP_OK,"NOOP\r\n");	
	if(ret!=0)ftp_login(ftp_user->ip,ftp_user->c_port,ftp_user->user,ftp_user->pass);
}

int ftp_logout()
{
	int ret=check_reply_code(ftp_user->ctrl_fd,QUIT_OK,"QUIT\r\n");
	if(ret!=0){
		log_console_debug(0,LOG_DEBUG("logout error"));
		return -1;
	}

	// ftp_user->login_state=LOGOUT;
	log_console(2,"Quit Ok");

	int n=close_socket(ftp_user->ctrl_fd);
	if(n==-1){
		log_console_debug(0,LOG_DEBUG("close control socket failed!"));
		return -1;
	}
	
	free(ftp_user);
	return 0;
}

//print current working dirtory
int ftp_pwd(char *path,int size)
{
	read_after_write(ftp_user->ctrl_fd,read_buf,READ_BUFSIZE,"PWD\r\n");	
	if(strncmp(read_buf,PWD_OK,3)!=0){
		log_console_debug(0,LOG_DEBUG("pwd error"));
		return -1;
	}

	//remove reply_code and remove ""
	remove_reply_code(read_buf);
	for(int i=1;i<strlen(read_buf)-1;i++)
		read_buf[i-1]=read_buf[i];
	read_buf[strlen(read_buf)-2]='\0';

	if(path!=NULL)strncpy(path,read_buf,size);
	else log_console(2,read_buf);

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
	int ret=check_reply_code(ftp_user->ctrl_fd,MKD_OK,"MKD %s\r\n",dir_name);
	if(ret!=0){
		log_console_v(0,"mkdir %s failed!",dir_name);
	}else{
		log_console_v(2,"mkdir %s ok!",dir_name);
	}
	
	return ret;	
}

int ftp_rmdir(const char* dir_name)
{
	int ret=check_reply_code(ftp_user->ctrl_fd,RMD_OK,"RMD %s\r\n",dir_name);
	if(ret!=0){
		log_console_v(0,"rmdir %s failed!",dir_name);
	}else{
		log_console_v(2,"rmdir %s ok!",dir_name);
	}
	
	return ret;	
}

int ftp_cwd(const char* dir_name)
{
	int ret=check_reply_code(ftp_user->ctrl_fd,CWD_OK,"CWD %s\r\n",dir_name);	
	if(ret!=0){
		log_console_v(0,"change dir to %s failed!",dir_name);
	}else{
		log_console_v(2,"change dir to %s ok",dir_name);
	}
	
	return ret;	
}

int ftp_noop()
{
	int ret=check_reply_code(ftp_user->ctrl_fd,NOOP_OK,"NOOP\r\n");	
	if(ret!=0){
		log_console(0,"noop error!");
	}else{
		log_console(2,"noop ok!");
	}
	
	return ret;
}

int ftp_delete_file(const char* file_name)
{
	int ret=check_reply_code(ftp_user->ctrl_fd,DELE_OK,"DELE %s\r\n",file_name);
	if(ret!=0){
		log_console_v(0,"delete %s failed!",file_name);
	}else{
		log_console_v(2,"delete %s ok!",file_name);
	}

	return ret;
}

int ftp_list(const char* df_name)
{
	int d_port=goto_pasv_mode(ftp_user->ctrl_fd);
	int data_fd=-1;
	if((data_fd=init_socket(ftp_user->ip,d_port))==-1){
		return -1;
	}

	if(check_reply_code(ftp_user->ctrl_fd,LIST_OK,"LIST %s\r\n",df_name)==-1){
		log_console_v(0,"list %s failed!",df_name);
		close_socket(data_fd);
		return -1;
	}

	read_socket(data_fd,read_buf,READ_BUFSIZE);
	split_list_info(read_buf);
	close_socket(data_fd);
	
	read_socket(ftp_user->ctrl_fd,read_buf,READ_BUFSIZE);
	if(compare_reply_code(read_buf,LIST_OVER)){
		log_console_v(0,"list %s not over!",df_name);
		return -1;
	}
	return 0;
}

int ftp_retr_file(const char* file_name)
{
	int d_port=goto_pasv_mode(ftp_user->ctrl_fd);
	int data_fd=-1;
	if((data_fd=init_socket(ftp_user->ip,d_port))==-1){
		return -1;
	}
	
	if(check_reply_code(ftp_user->ctrl_fd,RETR_OK,"RETR %s\r\n",file_name)==-1){
		log_console_v(0,"download %s failed!",file_name);
		close_socket(data_fd);
		return -1;
	}
	
	FILE *fp=fopen(file_name,"wb");
	if(fp!=NULL){
		char file_buf[READ_BUFSIZE];
		while(1){
			int cnt=read_socket(data_fd,file_buf,READ_BUFSIZE);
			if(cnt==0)break;
			fwrite(file_buf,sizeof(char),cnt,fp);
		}
		fclose(fp);
	}
	close_socket(data_fd);

	read_socket(ftp_user->ctrl_fd,read_buf,READ_BUFSIZE);
	if(compare_reply_code(read_buf,RETR_OVER)){
		log_console_v(0,"download %s not over!",file_name);
		return -1;
	}
	
	log_console_v(2,"download %s over",file_name);
	return 0;
}

int ftp_stor_file(const char* file_name)
{

	int d_port=goto_pasv_mode(ftp_user->ctrl_fd);
	int data_fd=-1;
	if((data_fd=init_socket(ftp_user->ip,d_port))==-1){
		return -1;
	}
	
	if(check_reply_code(ftp_user->ctrl_fd,STOR_OK,"STOR %s\r\n",file_name)==-1){
		log_console_v(0,"upload %s failed!",file_name);
		close_socket(data_fd);
		return -1;
	}
	
	FILE *fp=fopen(file_name,"rb");
	if(fp!=NULL){
		char file_buf[READ_BUFSIZE];
		while(1){
			int cnt=fread(file_buf,sizeof(char),READ_BUFSIZE,fp);
			if(cnt==0)break;
			write_socket_b(data_fd,file_buf,cnt);
		}
		fclose(fp);
	}
	close_socket(data_fd);

	read_socket(ftp_user->ctrl_fd,read_buf,READ_BUFSIZE);
	if(compare_reply_code(read_buf,STOR_OVER)){
		log_console_v(0,"upload %s not over!",file_name);
		return -1;
	}
	
	log_console_v(2,"upload %s ok",file_name);
	return 0;
}