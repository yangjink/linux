#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


void print(int sig)
{
	printf("signal  is  %d\n",sig);
}



int main()
{
	void(*f)(int x);
	int i = 1;
	f = print;
	for(;i < 31;i++)
	{
		signal(i,f);
	}

	while(1);
	return 0;
}
