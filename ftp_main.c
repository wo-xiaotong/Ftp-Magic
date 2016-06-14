#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ftp_func.h"
#include "utils.h"

#define IP "127.0.0.1"
#define C_PORT 21

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
	printf(">>>q or Q to close ftp client,help to show order \n\n");
}

static int get_order_num(char *order,char* para,int size)
{
	int ret=1000;
	memset(para,0,size);
	if(order[0]=='Q' || order[0]=='q')ret=-1;
	else if(strncmp(order,"noop",4)==0)ret=0;
	else if(strncmp(order,"pwd",3)==0)ret=1;
	else if(strncmp(order,"cd",2)==0)ret=2;
	else if(strncmp(order,"ls",2)==0)ret=3;
	else if(strncmp(order,"mkdir",5)==0)ret=4;
	else if(strncmp(order,"rmdir",5)==0)ret=5;
	else if(strncmp(order,"help",4)==0)ret=6;
	else if(strncmp(order,"rm",2)==0)ret=7;
	else if(strncmp(order,"cp",2)==0)ret=8;

	int len=strlen(order);
	if(ret==2 || ret==7 || ret==8)strncpy(para,order+2,size);
	if(ret==3)strncpy(para,order+3,size);
	if(ret==4 || ret==5)strncpy(para,order+5,size);
	remove_space(para);

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
		printf("Enter your order > ");
		char order[256];
		fgets(order,256,stdin);
		order[strlen(order)-1]='\0';
		remove_space(order);

		//check whether connection is ok
		if(ftp_check_connect()==0)ftp_login(IP,C_PORT,"ftpuser","ftptest");
		char para[256];
		int num=get_order_num(order,para,sizeof(para));
		switch (num){
			case 0:ftp_noop();break;
			case 1:ftp_pwd();break;
			case 2:ftp_cwd(para);break;
			case 3:ftp_list(para);break;
			case 4:ftp_mkdir(para);break;
			case 5:ftp_rmdir(para);break;
			case 6:show_order();break;
			case 7:ftp_delete_file(para);break;
			case -1:loop=0;break;
			default:printf("> bad order!\n");break;
		}
	}

	ftp_logout();
	return 0;
}

