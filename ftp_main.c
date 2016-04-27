#include<stdio.h>
#include"connect_func.h"

int main()
{
	int fd=init_socket("127.0.0.1",21);
	printf("%d\n",fd);
	close_socket(fd);
	return 0;
}


