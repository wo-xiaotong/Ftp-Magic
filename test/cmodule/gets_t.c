#include<stdio.h>

int main()
{
	char c[256];
	
	if(-1)printf("csssssssss\n");

	while(1){
		fgets(c,256,stdin);
		printf("%d\n",strlen(c));
		fputs(c,stdout);
	}
	return 0;
}

