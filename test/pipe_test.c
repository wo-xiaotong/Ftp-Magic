#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main()
{
	char buff[1024];
	memset(buff,'\0',sizeof(buff));
	FILE *fp=popen("ps -ax","r");

	if(fp!=NULL){
		int char_read=fread(buff,sizeof(char),1024,fp);
		while(char_read>0){
			buff[char_read-1]='\0';
			printf("Output is:\n%s\n",buff);
			char_read=fread(buff,sizeof(char),1024,fp);
		}
		pclose(fp);
		exit(EXIT_SUCCESS);
	}

	exit(EXIT_FAILURE);
}
