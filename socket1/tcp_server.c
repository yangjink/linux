#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>


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
		
		int id = fork();
		if(id < 0 )
		{
			printf("fork  error!\n");
			exit(5);
		}
		else if(id == 0)
		{
			
			while(1)
			{
				int id = fork();
				if(id > 0 )
				{
					exit(0);
				}
				char buf[1024];
				ssize_t s = read(client_sock,buf,sizeof(buf));
				if(s>0)
				{
					buf[s] = 0;
					printf("client :# %s\n",buf);
				}
				else
				{
					printf("read down ..........break;");
					break;
				}
				write(client_sock,buf,sizeof(buf)-1);
			}
		}
		else
		{

		}
		
	}


	return 0;
}
