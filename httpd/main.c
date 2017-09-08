#include <sys/epoll.h>
#include <sys/types.h>
#include "httpd.h"
#ifdef pthread
#include <pthread.h>

void usage(const char* arg)
{
	printf("Usage: %s   [local_ip]  [local_port]\n",arg);
}

int main(int argc,const char* argv[])
{
	
	if(argc != 3)
	{
		usage(argv[0]);
		return 1;
	}
	
	int listen_sock = startup(argv[1],atoi(argv[2]));

	while(1)
	{
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		int new_fd = accept(listen_sock,(struct sockaddr*)&client,&len);
		if(new_fd < 0)
		{
			print_log(strerror(errno),WARNING);
			continue;
		}

		printf("get a new client , %s:%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));

		pthread_t id;
		pthread_create(&id,NULL,handler_request,(void*)new_fd);
		pthread_detach(id);
	}
	
	return 0;
}
#else

#define SIZE_BUF 10240
typedef struct epoll_buff
{
	int fd;
	char buf[SIZE_BUF];
}epoll_buff_t,*epoll_buff_p;

static void* alloc_buff(int fd)
{
	epoll_buff_p tmp = malloc(sizeof(epoll_buff_t));
	if(!tmp)
	{
		perror("malloc");
		exit(3);
	}
	tmp->fd = fd;
	return tmp;
}
void usage(const char* arg)
{
	printf("Usage: %s   [local_ip]  [local_port]\n",arg);
}

int main(int argc,const char* argv[])
{
	
	if(argc != 3)
	{
		usage(argv[0]);
		return 1;
	}
	
	int listen_sock = startup(argv[1],atoi(argv[2]));
	int epfd = epoll_create(256);
	if(epfd < 0)
	{
		perror("epoll_create");
		return 2;
	}
	
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.ptr = alloc_buff(listen_sock);
	epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&ev);
	struct epoll_event revs[64];
	int size = sizeof(revs)/sizeof(revs[0]);
	int nums = 0;
	int timeout = -1;
	while(1)
	{
		switch((nums = epoll_wait(epfd,revs,size,timeout)))
		{
			case -1:
				perror("epoll_wait");
				break;
			case 0:
				printf("timeout...\n");
				break;
			default:
				{
					int i = 0;
					for(;i < nums;++i)
					{
						epoll_buff_p p = (epoll_buff_p)revs[i].data.ptr;
						int fd = p->fd;
						if(fd == listen_sock && (revs[i].events & EPOLLIN))
						{
							struct sockaddr_in client;
							socklen_t len = sizeof(client);
							int new_sock = accept(fd,(struct sockaddr*)&client,&len);
							if(new_sock < 0)
							{
								perror("accept");
								continue;
							}
							
							ev.events = EPOLLIN;
							ev.data.ptr = alloc_buff(new_sock);
							epoll_ctl(epfd,EPOLL_CTL_ADD,new_sock,&ev);
						}
						else if( fd != listen_sock)
						{
							if(revs[i].events & EPOLLIN)
							{
								ssize_t s = get_line(fd,p->buf,sizeof(p->buf));
								if(s > 0)
								{
									(p->buf)[s] = 0;
									handler_request1(fd,p->buf,sizeof(p->buf));
								}
							}
						}

					}
				}
		}
	}
	
	return 0;
}
#endif
