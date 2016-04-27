#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int count=10;
	while(count--){
		struct sockaddr_in address;
		address.sin_family=AF_INET;
		address.sin_addr.s_addr=inet_addr("127.0.0.1");
		address.sin_port=htons(9999);

		int server_fd=socket(AF_INET,SOCK_STREAM,0);
		int ret=connect(server_fd,(struct scokaddr*)&address,sizeof(address));
		if(ret==-1){
			printf("connect failed!\n");
			exit(-1);
		}

		char ch='A';
		write(server_fd,&ch,1);
		close(server_fd);
	}
	exit(0);
}


