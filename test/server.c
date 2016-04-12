#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

int main()
{
	struct sockaddr_in server_address;
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=inet_addr("127.0.0.1");
	server_address.sin_port=htons(9999);

	int server_fd=socket(AF_INET,SOCK_STREAM,0);
	bind(server_fd,(struct sockaddr*)&server_address,sizeof(server_address));
	listen(server_fd,5);

	signal(SIGCHLD,SIG_IGN);
	
	while(1){
		struct sockaddr_in client_address;
		int client_fd=accept(server_fd,(struct sockaddr*)&client_address,sizeof(client_address));
		
		if(fork()==0){
			char ch;
			sleep(3);
			read(client_fd,&ch,1);
			printf("recv info from client:%c\n",ch);
			close(client_fd);
			exit(0);
		}
		else{
			close(client_fd);
		}
	}

	close(server_fd);
	exit(0);
}


