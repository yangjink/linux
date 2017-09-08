#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mymath(char* arg)
{
	//传进来的参数 data1=100&data2=200
	char *argv[3];
	int i=0;
	char *start=arg;
	while(*start)//提取每个参数的数值
	{
		if(*start == '=')
		{
			start++;
			argv[i++]=start;
			continue;
		}
		if(*start == '&')
		{
			*start='\0';
		}
		start++;
	}
	argv[i] = NULL;
	int data1=atoi(argv[0]);
	int data2=atoi(argv[1]);

	printf("<html><body>");
	printf("%d+%d=%d<br>",data1,data2,data1+data2);
	printf("</body></html>");
}
int main()
{
	char* method=NULL;
	char* string_arg=NULL;
	char* query_string=NULL;
	int content_len=-1;
	char buf[1024];
	if(method = getenv("METHOD"))
	{
		if(strcasecmp(method,"GET") == 0)
		{
			if(query_string = getenv("QUERY_STRING"))
			{
				string_arg=query_string;
			}
		}
		else
		{//POST
			if(getenv("CONTENT_LEN"))
			{
				content_len=atoi(getenv("CONTENT_LEN"));
				int i= 0;
				for(;i < content_len;i++)//from content arguments
				{
					read(0,&buf[i],1);
				}
				buf[i] = '\0';
				string_arg=buf;
			}
		}
	}

	mymath(string_arg);
	return 0;
}
