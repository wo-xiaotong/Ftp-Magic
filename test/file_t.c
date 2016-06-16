#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
/*	FILE *fp=fopen("hello/1","w");
	if(fp==NULL){
		printf("null fp\n");
		return ;
	}
	fclose(fp);
*/
	printf("%d\n",mkdir("1/2",S_IRUSR|S_IWUSR));
	return 0;
}
