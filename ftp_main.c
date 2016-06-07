#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ftp_func.h"

#define IP "127.0.0.1"
#define C_PORT 21

static void show_order()
{
	printf("\n\tnoop:Send noop \n");
	printf("\tpwd:Print current working directory \n");
	printf("\tcwd dir:Change working directory\n");
	printf("\tlist dir/file:Print dir or file info \n");
	printf("\tmkdir dir:Create directory \n");
	printf("\trmdir dir:Remove directory \n");
	printf("\n\tq or Q to close ftp client,help to show order \n");
}

static int get_order_num(char *order,char* para)
{
	int ret=1000;
	if(order[0]=='Q' || order[0]=='q')ret=-1;
	else if(strncmp(order,"noop",4)==0)ret=0;
	else if(strncmp(order,"pwd",3)==0)ret=1;
	else if(strncmp(order,"cwd ",4)==0)ret=2;
	else if(strncmp(order,"list ",5)==0)ret=3;
	else if(strncmp(order,"mkdir ",6)==0)ret=4;
	else if(strncmp(order,"rmdir ",6)==0)ret=5;
	else if(strncmp(order,"help",4)==0)ret=6;

	int len=strlen(order);
	if(ret==2)strncpy(para,order+4,len-4);
	if(ret==3)strncpy(para,order+5,len-5);
	if(ret==4 || ret==5)strncpy(para,order+6,len-6);

	return ret;
}


int main(int argc,const char* argv[])
{
	int ret=ftp_login(IP,C_PORT,"ftpuser","ftptest");
	if(ret==-1)exit(-1);
	else ftp_get_user();

	show_order();
	int loop=1;
	while(loop){
		char order[256];
		fgets(order,256,stdin);
		order[strlen(order)-1]='\0';

		char para[256];
		memset(para,0,256);
		int num=get_order_num(order,para);
		switch (num){
			case 0:ftp_noop();break;
			case 1:ftp_pwd();break;
			case 2:ftp_cwd(para);break;
			case 3:ftp_list(para);break;
			case 4:ftp_mkdir(para);break;
			case 5:ftp_rmdir(para);break;
			case 6:show_order();break;
			case -1:loop=0;break;
			default:printf("bad order!\n");break;
		}
	}

	ftp_logout();
	return 0;
}

