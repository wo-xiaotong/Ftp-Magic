#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "ftp_func.h"
#include "utils.h"
#include "log_manage.h"

char* order[] = {
	"\n1.noop:Send noop ",
	"2.pwd:Print current working directory ",
	"3.cd dir:Change working directory",
	"4.ls dir/file:Print dir or file info ",
	"5.mkdir dir:Create directory ",
	"6.rmdir dir:Remove directory ",
	"7.rm filename:Remove file ",
	"8.cp filename:Copy file",
	"9.load filename:upload file",
	">>>q or Q to close ftp client,help to show order \n",
};

char* help[] = {
	"--help -h: show args help",
	"--ip -i: set ip,default=localhost",
	"--user -u: set user,default=anonymous",
	"--pass -p: set pass,default=anonymous",
};

struct option long_options[] = {
	{"help", no_argument, NULL, 'h'},
	{"ip", required_argument, NULL, 'i'},
	{"user", required_argument, NULL, 'u'},
	{"pass", required_argument, NULL, 'p'},
	{NULL, 0, NULL, 0}
};
const char* short_opts = "hi:u:p:";

void printArr(char **arr, int size);
void show_order();
int get_order_num(char *order, char* para, int size);

char *IP = "127.0.0.1";
char *USER = "anonymous";
char *PASS = "anonymous";
int C_PORT = 21;

int main(int argc, char* argv[])
{
	int opt;
	while ((opt = getopt_long(argc, argv, short_opts, long_options, NULL)) != -1) {
		switch (opt) {
			case 'h':
				printArr(help, 4);
				return 0;
			case 'i':
				IP = optarg;
				break;
			case 'u':
				USER = optarg;
				break;
			case 'p':
				PASS = optarg;
				break;
		}
	}

	int ret = ftp_login(IP, C_PORT, USER, PASS);
	if (ret == -1)exit(-1);
	else ftp_get_user();

	show_order();
	int loop = 1;
	char path[1024] = "/";
	while (loop) {
		ftp_check_connect();
		ftp_pwd(path, sizeof(path));
		printf("%s> ", path);
		char order[256];
		fgets(order, 256, stdin);
		order[strlen(order) - 1] = '\0';

		//check whether connection is ok
		ftp_check_connect();
		char para[256] = {0};
		int num = get_order_num(order, para, sizeof(para));
		switch (num) {
		case 0: ftp_noop(); break;
		case 1: ftp_pwd(0, 0); break;
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
		default: log_console(0, "bad order!"); break;
		}
	}

	ftp_logout();
	return 0;
}


void printArr(char **arr, int size)
{
	for (int i = 0; i < size; ++i)
		printf("%s\n", arr[i]);
}
void show_order()
{
	printArr(order,10);
}

int get_order_num(char *order, char* para, int size)
{
	char order_pre[16] = {0};
	char* cp = strtok(order, " ");
	if (cp != NULL)strncpy(order_pre, cp, sizeof(order_pre));
	cp = strtok(NULL, " ");
	if (cp != NULL)strncpy(para, cp, size);

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