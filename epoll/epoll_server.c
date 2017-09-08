#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <string.h>
#include <sys/types.h>

#define SIZE 10240

int mywrite()
{

}
int myread()
{

}
int myaccept()
{

}

typedef struct epoll_buff
{
	int fd;
	char buf[SIZE];
}epoll_buff_t,*epoll_buff_p;


static void* alloc_buff(int fd)
{
	epoll_buff_p tmp = malloc(sizeof(epoll_buff_t));
	if(!tmp)
	{
		perror("malloc");
		exit(6);
	}
	tmp->fd = fd;
	return tmp;
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
	local.sin_addr.s_addr = inet_addr(ip);

	if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0)
	{
		perror("bind");
		exit(3);
	}

	if(listen(sock,5) < 0)
	{
		perror("listen");
		exit(4);
	}
	
	return sock;
}

int main(int argc,const char* argv[])
{
	if(argc < 3)
	{
		printf("%s  +  ip  +  port \n",argv[0]);
		return 1;
	}

	int listen_sock = startup(argv[1],atoi(argv[2]));

	int epfd = epoll_create(256);
	if(epfd < 0)
	{
		perror("epoll_create");
		return 5;
	}
	
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.ptr = alloc_buff(listen_sock);
	epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&ev);

	struct epoll_event revs[64];
	int size = sizeof(revs)/sizeof(revs[0]);
	int nums = 0;
	int timeout = 4000;
	while(1)
	{
		//char buf[1024];
		switch((nums = epoll_wait(epfd,revs,size,timeout)))
		{
			case -1:
				perror("epoll_wait");
				break;
			case 0:
				printf("timeout....\n");
				break;
			default:
			{
				int i = 0;
				for(;i < nums;i++)
				{
					epoll_buff_p p = (epoll_buff_p)(revs[i].data.ptr);
					int fd = p->fd;

					if(fd == listen_sock && (revs[i].events & EPOLLIN))
					{
						//printf("aaaaaaaaaaaaaaaaaaaaa\n");
						struct sockaddr_in client;
						socklen_t len = sizeof(client);
						int new_sock = accept(fd,(struct sockaddr*)&client,&len);
						if(new_sock < 0)
						{
							perror("accept");
							continue;
						}
						printf("a new client: %s,%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

						ev.events = EPOLLIN;
						ev.data.ptr = alloc_buff(new_sock);
						epoll_ctl(epfd,EPOLL_CTL_ADD,new_sock,&ev);
						
					}
					else if(fd != listen_sock)
					{
						if(revs[i].events & EPOLLIN)
						{
							ssize_t s = read(fd,p->buf,SIZE-1);

							if(s > 0)
							{
								(p->buf)[s] = 0;
								printf("%s\n",p->buf);
								ev.events = EPOLLOUT;
								epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev);
							}
							else
							{
								printf("client quit %d\n",s);
								//sleep(2);
	 							close(fd);
								epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
								perror("read");
							}
						}
						else if(revs[i].events & EPOLLOUT)
						{
							write(fd,p->buf,SIZE-1);
							//close(fd);
							//epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
						
						}
					}
				}//for

			}//default
			break;
		}//switch
	}//while

	return 0;
}
