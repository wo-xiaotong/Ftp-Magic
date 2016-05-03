#ifndef FTP_FUNC_H
#define FTP_FUNC_H

#include<stdio.h>
#define READ_BUF_SIZE BUFSIZ
#define WRITE_BUF_SIZE BUFSIZ

#define LOGIN_NEED_PASS "331"
#define LOGIN_RIGHT "230"
#define QUIT_OK "200"

int login_ftp(int control_fd,const char* user,const char* pass);
int connect_ftp(const char* ip,const int port);
int disconnect_ftp(const int control_fd);

enum state{
	LOGIN,
	LOGOUT
};

#endif
