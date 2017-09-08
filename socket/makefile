all:tcp_server tcp_client

tcp_server:tcp_server.c
	gcc -o $@ $^ -lstatic
tcp_client:tcp_client.c
.PHONY:clean
clean:
	rm -f tcp_server tcp_client
