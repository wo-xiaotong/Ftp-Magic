#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ftp_func.h"
#include "utils.h"
#include "log_manage.h"

#define IP "127.0.0.1"
// #define IP "115.156.165.48"
#define C_PORT 21
#define USER "ftpuser"
#define PASS "ftptest"
			
static void show_order()
{
	printf("\n1.noop:Send noop \n");
	printf("2.pwd:Print current working directory \n");
	printf("3.cd dir:Change working directory\n");
	printf("4.ls dir/file:Print dir or file info \n");
	printf("5.mkdir dir:Create directory \n");
	printf("6.rmdir dir:Remove directory \n");
	printf("7.rm filename:Remove file \n");
	printf("8.cp filename:Copy file\n");
	printf("9.load filename:upload file\n");
	printf(">>>q or Q to close ftp client,help to show order \n\n");
}

static int get_order_num(char *order, char* para, int size)
{
	char order_pre[16]={0};
	char* cp=strtok(order," ");
	if(cp!=NULL)strncpy(order_pre,cp,sizeof(order_pre));	
	cp=strtok(NULL," ");
	if(cp!=NULL)strncpy(para,cp,size);	

	int ret = 1000;
	if (strlen(order_pre) == 0)ret = -2;
	else if (strcmp(order_pre, "q") == 0)ret = -1;
	else if (strcmp(order_pre, "Q") == 0)ret = -1;
	else if (strcmp(order_pre, "noop") == 0)ret = 0;
	else if (strcmp(order_pre, "pwd") == 0)ret = 1;
	else if (strcmp(order_pre, "cd") == 0)ret = 2;
	else if (strcmp(order_pre, "ls") == 0)ret = 3;
	else if (strcmp(order_pre, "mkdir") == 0)ret = 4;
	else if (strcmp(order_pre, "rmdir") == 0)ret = 5;
	else if (strcmp(order_pre, "help") == 0)ret = 6;
	else if (strcmp(order_pre, "rm") == 0)ret = 7;
	else if (strcmp(order_pre, "cp") == 0)ret = 8;
	else if (strcmp(order_pre, "load") == 0)ret = 9;

	return ret;
}


int main(int argc, const char* argv[])
{
	int ret = ftp_login(IP, C_PORT,USER,PASS);
	if (ret == -1)exit(-1);
	else ftp_get_user();

	show_order();
	int loop = 1;
	char path[1024]="/";
	while (loop) {
		ftp_check_connect();
		ftp_pwd(path,sizeof(path));
		printf("%s> ",path);
		char order[256];
		fgets(order, 256, stdin);
		order[strlen(order) - 1] = '\0';

		//check whether connection is ok
		ftp_check_connect();
		char para[256] = {0};
		int num = get_order_num(order, para, sizeof(para));
		switch (num) {
		case 0: ftp_noop(); break;
		case 1: ftp_pwd(0,0); break;
		case 2: ftp_cwd(para); break;
		case 3: ftp_list(para); break;
		case 4: ftp_mkdir(para); break;
		case 5: ftp_rmdir(para); break;
		case 6: show_order(); break;
		case 7: ftp_delete_file(para); break;
		case 8: ftp_retr_file(para); break;
		case 9: ftp_stor_file(para); break;
		case -1: loop = 0; break;
		case -2: break;
		default: log_console(0,"bad order!"); break;
		}
	}

	ftp_logout();
	return 0;
}

