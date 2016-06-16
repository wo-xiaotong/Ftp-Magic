#include<stdio.h>
#include<getopt.h>
#include<stdlib.h>

char* short_opts="ho:v:";
struct option long_options[]={
	{"help",no_argument,NULL,'h'},
	{"output",required_argument,NULL,'o'},
	{"version",no_argument,NULL,'v'},
	{NULL,0,NULL,0}
};

int main(int argc,char*argv[])
{
	int opt;
	char* l_optarg=0;

	while((opt=getopt_long(argc,argv,short_opts,long_options,NULL))!=-1){
		switch(opt){
			case 'h':
				printf("help\n");
				break;
			case 'o':
				l_optarg=optarg;
				printf("%s\n",l_optarg);
				break;
			case 'v':
				printf("version\n");
				break;
		}
	}

	return 0;
}
