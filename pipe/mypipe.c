#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>


void handle(int sig)
{
	printf("sig is %d\n",sig);
}
int main()
{

	void(*f)(int);
	f = handle;
	signal(13,f);
	int _pipe[2];
	int ret = pipe(_pipe);
	if(ret == -1)
	{
		printf("pipe apply for error!");
		return 1;
	}

	pid_t id = fork();
	if(id < 0)
	{
		printf("fork creation fails!");
		return 2;
	}
	else if(id == 0)
	{//child
		int i = 1;
		close(_pipe[0]);
		while(i)
		{
			if(i)
			{
				char* str = "yang";
				write(_pipe[1],str,strlen(str));
				printf("%d\n",i);
			}
			
			i++;
		}
		close(_pipe[1]);
	}
	else
	{//father
		long long j = 1;
		
		close(_pipe[1]);
		char str[10]={0};
		while(j)
		{
			if(j>222222222)
			{
				int ret=read(_pipe[0],str,sizeof(str));
				printf("ret=%d  %s\n",ret,str);
				sleep(1);
			}
			j++;
		}
		close(_pipe[0]);
		sleep(10);


		int status = 0;
		pid_t ret1=waitpid(id,&status,0);

	}
	return 0;
}

