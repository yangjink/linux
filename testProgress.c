#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
	int i = fork();
	if(i == 0)
	{
		printf("my pid:%d,parent pid:%d\n",getpid(),getppid());
		sleep(5);
		printf("my pid:%d,parent pid:%d\n",getpid(),getppid());
	}
	else
	{
		printf("my pid:%d,parent pid:%d\n",getpid(),getppid());
		printf("parent progress exit\n");
		sleep(1);
		exit(1);
	}
	return 0;
}
