#include <string.h>
#include "reply_deal.h"
#include "log_manage.h"

int remove_reply_code(char* reply)
{
	int n=strlen(reply);
	if(n<4)return -1;

	//remove \r\n
	if(n>=2 && reply[n-1]=='\n' && reply[n-2]=='\r'){
		reply[n-2]='\0';
		n=strlen(reply);
	}

	//remove reply code and space 
	for(int i=4;i<=n;++i){	
		reply[i-4]=reply[i];
	}

	return 0;
}

int split_list_info(char* reply)
{
	char *cp=strtok(reply,"\r\n");
	while(cp!=NULL){
		cp=strtok(NULL,"\r\n");
		printf("%s\n",cp);
	}

	return 0;
}

int get_pasv_port(char* reply)
{
	char* p=strtok(reply," ,.()");
	int cnt=7;
	while (cnt-- && p!=NULL){
		p=strtok (NULL," ,.()");
	}
	char* p1=strtok(NULL," ,.()");
	char* p2=strtok(NULL," ,.()");

	return atoi(p1)*256+atoi(p2);
}
