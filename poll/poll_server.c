#include <stdio.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


static void usage(const char* proc)
{
	printf("Usage: %s + local_ip + local_port  \n",proc);
}

int startup(const char* ip,int port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(2);
	}
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = htonl(inet_addr(ip));

	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		perror("bind");
		exit(3);
	}
	
	if(listen(sock,5))
	{
		perror("listen");
		exit(4);
	}

	return sock;
}
int main(int argc,char* argv[])
{
	if(argc != 3)
	{
		usage(argv[0]);
		return 1;
	}
	
	int listen_sock = startup(argv[1],atoi[argv[2]]);
	
	struct pollfd fds[30];
	for(int i = 0;i <= 30;i++)
	{
		fds[i].fd = -1;
	}
	fds[0].fd = listen_sock;	
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	
	int size = 1;
	int timeout = 2000;
	char buf[1024];

	while(1)
	{
		switch(poll(fds,size,timeout))
		{	
			case -1:
				perror("poll");
				return 5;
				break;
			case 0:
				printf("poll timeout\n");
				break;
			default:
				{
					while(1)
					{
						if((client[0].revents & POLLIN) )
					}
				}
				break;

		}
	}
	return 0;
}
