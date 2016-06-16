#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
	FILE *fp=fopen("1.txt","rb");
	if(fp==NULL){
		printf("null fp\n");
		return ;
	}
	fclose(fp);

//	printf("%d\n",mkdir("1/2",S_IRUSR|S_IWUSR));
	
	return 0;
}
