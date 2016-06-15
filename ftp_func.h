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
#define LIST_OK "150"
#define LIST_OVER "226"
#define SIZE_OK "213"
#define DELE_OK "250"
#define RETR_OK "150"
#define RETR_OVER "226"
#define STOR_OK "150"
#define STOR_OVER "226"

#define READ_BUFSIZE BUFSIZ


int ftp_login(const char* ip,int port,const char* user,const char* pass);
int ftp_logout();
void ftp_get_user();
void ftp_check_connect();

int ftp_mkdir(const char* dir_name);
int ftp_rmdir(const char* dir_name);
int ftp_cwd(const char* dir_name);
int ftp_noop();
int ftp_syst();
int ftp_pwd(char* path,int size);
int ftp_delete_file(const char* file_name);

int ftp_list(const char* fd_name);
int ftp_retr_file(const char* file_name);
int ftp_stor_file(const char* file_name);

typedef struct USRE_INFO{
	char user[256];
	char pass[256];
	char ip[16];
	int ctrl_fd;
	int c_port;
}user_info;

typedef struct FILE_INFO{
	char type;
	char name[256];
}file_info;

#endif
