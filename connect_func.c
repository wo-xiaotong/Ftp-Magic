#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "connect_func.h"
#include "log_manage.h"
#include "utils.h"


static char write_buf[WRITE_BUFSIZE];

int init_socket(const char* ip,const int port)
{
	struct sockaddr_in serv_addr;
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(ip);
	serv_addr.sin_port=htons(port);

	int ret=connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	if(ret==-1){
		log_console_v(0,"connect to %s:%d failed!",ip,port);
		return -1;
	}
	
	return sockfd;
}

int close_socket(int sock_fd)
{
	int ret=close(sock_fd);
	if(ret==-1){
		log_console_v(0,"close %d socket fd dailed!",sock_fd);
	}

	return ret;
}

int read_socket(int sock_fd,char* read_buf,int size)
{
	memset(read_buf,0,size);
	int n=read(sock_fd,read_buf,size-1);
	if(n==-1){
		log_console_debug(0,LOG_DEBUG("read socket failed!"));
	}

	return n;
}

int write_socket_v(int sock_fd,const char* format,...)
{
	va_list args;
	va_start(args,format);
	vsnprintf(write_buf,WRITE_BUFSIZE,format,args);
	va_end(args);

	int n=write(sock_fd,write_buf,strlen(write_buf)); 
	if(n==-1){
		log_console_debug(0,LOG_DEBUG("write socket failed!"));
	}

	return n;
}

int write_socket(int sock_fd,char* write_buf)
{
	int n=write(sock_fd,write_buf,strlen(write_buf)); 
	if(n==-1){
		log_console_debug(0,LOG_DEBUG("write socket failed!"));
	}

	return n;
}

int read_after_write(int sock_fd,char* read_buf,int read_size,const char* write_format,...)
{
	va_list args;
	va_start(args,write_format);	
	vsnprintf(write_buf,WRITE_BUFSIZE,write_format,args);
	va_end(args);

	if(write_socket(sock_fd,write_buf)==-1){
		return -1;
	}
	int n=read_socket(sock_fd,read_buf,read_size);

	return n;	
}

int check_reply_code(int sock_fd,const char* reply_code,const char* format,...)
{
	va_list args;
	va_start(args,format);	
	vsnprintf(write_buf,WRITE_BUFSIZE,format,args);
	va_end(args);

	if(write_socket(sock_fd,write_buf)==-1){
		return -1;
	}
	
	char reply[128];
	if(read_socket(sock_fd,reply,128)<3){
		return -1;
	}
	
	if(strncmp(reply,reply_code,3)!=0){
		return -1;
	}
	
	return 0;
}
