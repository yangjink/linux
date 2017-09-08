#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#incl  ude <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>


int main(int argc,char* argv[])
{
	if(argc != 3)//judge input
	{              
		printf("tcp/_server + ip + port_number\n");
		exit(1);
	}
	int sock = socket(AF_INET,SOCK_STREAM,0);//creat socket
	if(sock < 0)
	{
		printf("creat socket error\n");
		exit(2);
	}
	
	struct sockaddr_in server_socket;//init sockaddr_in
	struct sockaddr_in client_socket;
	server_socket.sin_family = AF_INET;
	server_socket.sin_addr.s_addr = inet_addr(argv[1]);
	server_socket.sin_port = htons(atoi(argv[2]));
	
	if(bind(sock,(struct sockaddr*)&server_socket,sizeof(server_socket)) < 0)//bind sock
	{
		printf("bind error!\n");
		exit(3);
	}

	if(listen(sock,5) < 0)//listen sock
	{
		printf("listen error\n");
		exit(4);
	}

	while(1)
	{
		int len = 0;
		int client_sock = accept(sock,(struct sockaddr*)&client_socket,&len);//accept sock
		if(client_sock < 0)
		{
			printf("accept error!\n");
			continue;
		}

		printf("a new connect from %s  %d\n",inet_ntoa(client_socket.sin_addr),client_socket.sin_port);
		while(1)
		{//data processing
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


	return 0;
}
