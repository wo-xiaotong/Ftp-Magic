#include<stdio.h>

int main()
{
	printf("%d\n",strncmp("1","12",1));

	char c[256];
	while(1){
		char*s=fgets(c,256,stdin);
		printf("%d %s\n",strlen(c),s);
		fputs(c,stdout);
	}
	return 0;
}

