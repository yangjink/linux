#include <stdio.h>
#include <signal.h>

void printsigset(sigset_t* set)
{
	int i = 1;
	for(;i<32;i++)
	{
		if(sigismember(set,i))
		{
			printf("1");
		}
		else
		{
			printf("0");
		}
	}
	printf("\n");
}
int main()
{
	sigset_t set,oset,pset;

	sigemptyset(&set);
	sigemptyset(&oset);

	sigaddset(&set,2);

	sigprocmask(SIG_BLOCK,&set,&oset);

	while(1)
	{
		sigpending(&pset);
		printsigset(&pset);
		sleep(1);
	}
	return 0;
}
