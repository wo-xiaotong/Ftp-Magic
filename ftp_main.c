#include<stdio.h>

extern int init_socket(const char* ,const int);
extern int close_socket(int);


int main()
{
	int fd=init_socket("127.0.0.1",21);
	close_socket(fd);
	return 0;
}


