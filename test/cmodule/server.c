#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	struct sockaddr_in serv_addr;
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	serv_addr.sin_port=htons(9999);

	int server_fd=socket(AF_INET,SOCK_STREAM,0);
	bind(server_fd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
	listen(server_fd,5);

	printf("wait for client...\n");
	struct sockaddr_in client_addr;
	int c_len=sizeof(client_addr);
	int client_fd=accept(server_fd,(struct sockaddr*)&client_addr,&c_len);
	
	int n=write(client_fd,"hello",5);
	printf("%d\n",n);
	n=write(client_fd,"world",6);
	printf("%d\n",n);


	sleep(5);
	int c=close(client_fd);
	int s=close(server_fd);
	printf("c=%d s=%d",c,s);
	exit(0);
}


