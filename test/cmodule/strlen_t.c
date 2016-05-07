#include <stdio.h>
#include <string.h>

int main()
{
	char s[]={'a','b','c','\r','\n'};
	char s1[]="abc\r\n";

	printf("%d %d %s\n",sizeof(s),strlen(s),s);
	printf("%d %d %s\n",sizeof(s1),strlen(s1),s1);

	return 0;
}
