#include<stdio.h>
#include<string.h>
#include"log_manage.h"

char *log_level[3]={
	"ERROR",
	"USELESS",
	"NORMAL"
};

const char * LOG_FILE="1111";

int log_2_file(const char* file,const int level,const char* msg)
{
	if(level>2 || level<0)
		return -1;

	FILE *fp=fopen(file,"a");
	if(fp==NULL){
		printf("open %s error\n",file);
		return -1;
	}
	
	char buff[BUFSIZ];
	int n=fprintf(fp,"%s: %s\n",log_level[level],msg);
	fclose(fp);

	return n; 
}

int log_2_console(int level,const char* msg)
{
	if(level>2 || level<0)
		return -1;

	printf("%s: %s",log_level[level],msg);

	return strlen(msg);
}
