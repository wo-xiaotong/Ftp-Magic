#ifndef FTP_FUNC_H
#define FTP_FUNC_H

#include<stdio.h>

#define LOGIN_NEED_PASS "331"
#define LOGIN_RIGHT "230"
#define QUIT_OK "221"
#define PASV_OK "227"
#define PWD_OK "257"
#define MKD_OK "257"
#define RMD_OK "250"
#define SYST_OK "215"
#define CWD_OK "250"
#define CDUP_OK "250"
#define NOOP_OK "200"
#define STAT_OK "213"

#define READ_BUFSIZE BUFSIZ


int ftp_login(const char* ip,int port,const char* user,const char* pass);
int ftp_logout(const int sock_fd);

int query_login_state();
int goto_pasv_mode(const int ctrl_fd);
int open_data_connect(const char* IP,int d_port);
int close_data_connect(const int data_fd);

int ftp_mkdir(const int ctrl_fd,const char* dir_name);
int ftp_rmdir(const int ctrl_fd,const char* dir_name);
int ftp_cwd(const int ctrl_fd,const char* dir_name);
int ftp_cdup(const int ctrl_fd,const char* dir_name);
int ftp_noop(const int ctrl_fd);
int ftp_syst(const int ctrl_fd);
int ftp_pwd(const int ctrl_fd);


typedef enum state{
	LOGOUT,
	LOGIN,
	PASV_MODE
}user_state;

#endif
