#include<signal.h>
#include<stdio.h>
#include<unistd.h>

static struct sigaction oact;

void ouch(int sig)
{
	printf("signal num=%d\n",sig);
	sigaction(SIGINT,&oact,0);
}


int main()
{
	struct sigaction act;
	act.sa_handler=ouch;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;

	sigaction(SIGINT,&act,&oact);
	
	while(1){
		printf("hello world\n");
		sleep(1);
	}

	return 0;
}
