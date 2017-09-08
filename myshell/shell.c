#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<fcntl.h>

int main()
{
	while(1)
	{
		printf("[myshell@myhonstName test]#");
		fflush(stdout);

		char buf[1024];
		ssize_t s = read(0,buf,sizeof(buf)-1);
		if(s>0)
		{
			buf[s-1] = 0;
		}

		char*_argv[32];
		_argv[0] = buf;
		int i = 1;
		char *start = buf;
		while(*start)
		{
			if(*start == ' ')
			{
				*start = 0;
				start++;
				_argv[i++] = start;
			}
			else
			{
				start++;
			}
		}
		_argv[i] = NULL;

		pid_t id = fork();
		if(id == 0)
		{
			int j = 1;
			while(_argv[j] != NULL)
			{

				if(strcmp(_argv[j],">") == 0)
				{
					close(1);
					open(_argv[j+1],O_RDWR | O_CREAT,644);
					_argv[j]= NULL;	
				}
				j++;
			}
		
			
			
			execvp(_argv[0],_argv);
			exit(1);
		}
		else
		{
			int status = 0;
			pid_t ret = waitpid(id,&status,0);
			if(ret > 0)
			{
				if(WIFEXITED(status))
				{
				}
				else
				{
					printf("child,quit by sigg!\n");
				}

			}
		}

	}
	return 0;
}
