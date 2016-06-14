#include<stdio.h>

int main()
{

	char c[256];
	while(1){
		char*s=fgets(c,256,stdin);
		printf("%d %s\n",strlen(c),s);
		fputs(c,stdout);
	}
	return 0;
}

