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
	close(1);
	char *start = "file";
	char* argv[4];
	argv[0] = start;

	open(argv[0],O_RDWR);

	printf("zhoangguodjfklasdjflj");
	return 0;
}
