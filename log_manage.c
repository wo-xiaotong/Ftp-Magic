#include<stdio.h>
#include<string.h>
#include"log_manage.h"

char *log_level[3]={
	"ERROR",
	"USELESS",
	"NORMAL"
};

int log_file(const char* logfile,int level,char *msg)
{
	if(level>2 || level<0)
		return -1;

	FILE *fp=fopen(logfile,"a");
	if(fp==NULL){
		return -1;
	}
	
	int n=fprintf(fp,"%s:[%s]\n",log_level[level],msg);
	fclose(fp);

	return n;
}

int log_file_debug(const char* logfile,int level,int line,const char* file,const char* func, const char* msg)
{
	if(level>2 || level<0)
		return -1;

	FILE *fp=fopen(file,"a");
	if(fp==NULL){
		return -1;
	}
	
	int n=fprintf(fp,"%s: %s-%d-%s [%s]\n",log_level[level],file,line,func,msg);
	fclose(fp);

	return n; 
}

int log_console(int level,const char* msg)
{
	if(level>2 || level<0)
		return -1;

	int n=printf("%s: [%s]",log_level[level],msg);

	return n;
}

int log_console_debug(int level,int line,const char* file,const char* func, const char* msg)
{
	if(level>2 || level<0)
		return -1;

	FILE *fp=fopen(file,"a");
	if(fp==NULL){
		return -1;
	}
	
	int n=printf("%s: %s-%d-%s [%s]\n",log_level[level],file,line,func,msg);
	fclose(fp);

	return n; 
}

