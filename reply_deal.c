#include <string.h>
#include "reply_deal.h"
#include "log_manage.h"

int remove_reply_code(char* reply)
{
	int n=strlen(reply);
	if(n<4)return -1;

	if(n>=2 && reply[n-1]=='\n' && reply[n-2]=='\r'){
		reply[n-2]='\0';
		n=strlen(reply);
	}

	for(int i=4;i<=n;++i){	
		reply[i-4]=reply[i];
	}

	return 0;
}
