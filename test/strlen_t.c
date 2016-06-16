#include <stdio.h>
#include <string.h>

int main()
{
//	char s[]={'a','b','c'};
//	char s1[]="abc";
	char s2[13]="dasdqw12";

//	printf("%d %d %s\n",sizeof(s),strlen(s),s);
//	printf("%d %d %s\n",sizeof(s1),strlen(s1),s1);
	printf("%d %d %s\n",sizeof(s2),strlen(s2),s2);

	printf("%d\n",atoi("132 dswdqa"));
	printf("%d\n",atoi("132sdqadqw dswdqa"));
	printf("%d\n",atoi("0x132 dswdqa"));
	printf("%d\n",atoi("1321231321312 dswdqa"));
	printf("%d\n",atoi("132 22dswdqa"));
	return 0;
}
