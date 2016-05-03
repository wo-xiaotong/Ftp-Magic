#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include "connect_func.h"
#include "log_manage.h"

int init_socket(const char* ip,const int port)
{
	struct sockaddr_in serv_addr;
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(ip);
	serv_addr.sin_port=htons(port);

	int ret=connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	if(ret==-1){
		return -1;
	}
	
	return sockfd;
}

int close_socket(int sock_fd)
{
	return close(sock_fd);
}

int read_socket(int sock_fd,char* read_buf,int read_size)
{
	return read(sock_fd,read_buf,read_size);
}

int write_socket(int sock_fd,char* write_buf)
{
	return write(sock_fd,write_buf,strlen(write_buf)); 
}
