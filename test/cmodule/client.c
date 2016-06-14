#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	struct sockaddr_in address;
	address.sin_family=AF_INET;
	address.sin_addr.s_addr=inet_addr("127.0.0.1");
	address.sin_port=htons(9999);

	int server_fd=socket(AF_INET,SOCK_STREAM,0);
	int ret=connect(server_fd,(struct sockaddr*)&address,sizeof(address));
	if(ret==-1){
		printf("connect failed!\n");
		exit(-1);
	}

	while(1){
		char ch[128];
		int n=read(server_fd,ch,128);
		printf("n=%d ch=%s\n",n,ch);
		
		int ret=write(server_fd,ch,strlen(ch));
		printf("ret=%d,ch=%s\n",ret,ch);
	}
	

	int c=close(server_fd);
	printf("c=%d\n",c);
	exit(0);
}


