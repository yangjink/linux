#include <stdio.h>
#include <unistd.h>


int main()
{
	int i = fork();
	if(i == 0)
	{
		printf("i an child");
		//exit(0);
	}
	else
	{
		while(1)
		{
			printf(" i am parent progress");
		}
	}
	
	return 0;
}
