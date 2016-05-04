#ifndef FTP_FUNC_H
#define FTP_FUNC_H

#include<stdio.h>
#define READ_BUF_SIZE BUFSIZ
#define WRITE_BUF_SIZE BUFSIZ

#define LOGIN_NEED_PASS "331"
#define LOGIN_RIGHT "230"
#define QUIT_OK "221"
#define PASV_OK "227"

int login_ftp(const char* ip,int port,const char* user,const char* pass);
int logout_ftp(const int sock_fd);
int query_login_state();

int goto_PASV(const int ctrl_fd);
int connect_data(const char* IP,int d_port);
int close_data(const int data_fd);

typedef enum state{
	LOGIN,
	LOGOUT
}user_state;

#endif
