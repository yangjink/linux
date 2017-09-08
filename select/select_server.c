#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>




int fds[1024];
int fdw[1024];
void usage(const char*proc)
{
	printf("Usage:%s [local_ip] [local_port]",proc);
}
 
int  startup(const char* _ip,int _port)
{
	int sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0)
	{
		perror("socket");
		exit(2);
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_port = htons(_port);
	local.sin_addr.s_addr = htons(inet_addr(_ip));

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
	
	int listen_sock = startup(argv[1],atoi(argv[2]));
	
	int nums = sizeof(fds)/sizeof(fds[0]);
	int maxfd = -1;
	int i = 1;
	for(;i < nums;i++)
	{
		fds[i] = -1;
	}
	int z = 1;
	int nums2 = sizeof(fdw)/sizeof(fdw[0]);
	for(;z < nums2;z++)
	{
		fdw[z] = -1;
	}
	fdw[0] = listen_sock;
	fds[0] = listen_sock;
	while(1)
	{
		struct timeval timeout={5,0};
		fd_set rfds;
		fd_set wfds;
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		maxfd = -1;
		for(i = 0;i < nums;i++)
		{
			if(fds[i] > 0)
			{
				FD_SET(fds[i],&rfds);
				if(maxfd < fds[i])
				{
					maxfd = fds[i];
				}
			}
		}
		for(i = 0;i < nums2;i++)
		{
			if(fdw[i] > 0)
			{
				FD_SET(fdw[i],&wfds);
				if(maxfd < fdw[i])
				{
					maxfd = fdw[i];
				}
			}
		}
	
	switch(select(maxfd+1,&rfds,&wfds,NULL,NULL))
	{
		case 0:
			printf("timeout..........\n");
			break;
		case -1:
			perror("select");
			break;
		default:
			{

				char buf[1024];
				for(i = 0;i < nums;i++)
				{
					if(fds[i] < 0)
					{
						continue;
					}

					if(i == 0 && FD_ISSET(listen_sock,&rfds))
					{
						struct sockaddr_in client;
						socklen_t len = sizeof(client);
						int new_sock = accept(listen_sock,(struct sockaddr*)&client,&len);
						if(new_sock < 0)
						{
							perror("accept");
							continue;
						}

						printf("get new client: %s:%d",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

						int j = 1;
						for(;j < nums;j++)
						{
							if(fds[j] == -1)
							{
								break;
							}
						}
					
						if(j == nums)
						{
							printf("server full!\n");
							close(new_sock);
						}
						else
						{
							fds[j] = new_sock;
						}
						for(j = 1;j < nums2;j++)
						{
							if(fdw[j] == -1)
							{
								break;
							}
						}

						if(j == nums2)
						{
							printf("server full\n");
							close(new_sock);
						}
						else
						{
							fdw[j] = new_sock; 
						}
					}
					else if(i > 0 && FD_ISSET(fds[i],&rfds))
					{
						ssize_t s = read(fds[i],buf,sizeof(buf)-1);
						if(s > 0)
						{
							buf[s] = 0;
							printf("client say# %s\n",buf);
						}
						else if(s == 0)
						{
							printf("client quit!\n");
							close(fds[i]);
							fds[i] = -1;
						}
						else
						{
							perror("read");
						}
					}
					else if(i > 0 && FD_ISSET(fdw[i],&wfds))
					{
						write(fdw[i],buf,sizeof(buf) - 1);	
						
					}
					else
					{}
				}
			}
	}
	break;
	}

	return 0;
}
