#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		printf("tcp_server + ip + port_number\n");
		exit(1);
	}
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		printf("creat socket error\n");
		exit(2);
	}
	
	struct sockaddr_in client_socket;
	client_socket.sin_family = AF_INET;
	client_socket.sin_addr.s_addr = inet_addr(argv[1]);
	client_socket.sin_port = htons(atoi(argv[2]));
	
	if(connect(sock,(struct sockaddr*)&client_socket,sizeof(client_socket)) < 0)
	{
		printf("connect error!\n");
		return(3);
	}

	while(1)
	{	

		char buf[1024];
		//printf("please enter:");
		fflush(stdout);
		ssize_t s=read(0,buf,sizeof(buf)-1);
		if(s > 0)
		{
			buf[s] = 0;
			dup2(sock,1);
			printf("%s",buf);
		}
		//read(sock,buf,sizeof(buf));
		//printf("%s\n",buf);
		

	}


	return 0;
}
