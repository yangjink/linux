#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


void* handlerRequest(void* arg)
{
	int new_fd = (int)arg;
	while(1)
	{
		char buf[1024];
		ssize_t s= read(new_fd,buf,sizeof(buf)-1);
		if(s > 0)
		{
			buf[s] = 0;
			printf("client:%s\n",buf);
			write(new_fd,buf,strlen(buf));
		}
		else
		{
			printf("read done....\n");
			break;
		}
	}
}
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
	int op = 1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&op,sizeof(op));

	struct sockaddr_in server_socket;
	struct sockaddr_in client_socket;
	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = inet_addr(argv[1]);
	server_socket.sin_port = htons(atoi(argv[2]));
	
	if(bind(sock,(struct sockaddr*)&server_socket,sizeof(server_socket)) < 0)
	{
		printf("bind error!\n");
		exit(3);
	}

	if(listen(sock,5) < 0)
	{
		printf("listen error\n");
		exit(4);
	}

	while(1)
	{
		int len = 0;
		int client_sock = accept(sock,(struct sockaddr*)&client_socket,&len);
		if(client_sock < 0)
		{
			printf("accept error!\n");
			continue;
		}

		printf("a new connect from %s  %d\n",inet_ntoa(client_socket.sin_addr),client_socket.sin_port);
		
		pthread_t id;
		pthread_create(&id,NULL,handlerRequest,(void*)client_sock);
		pthread_detach(id);
		
	}
	return 0;
}
