#ifndef _HTTPD_
#define _HTTPD_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>

#define SUCCESS 0
#define NOTICE  1
#define WARNING 2
#define ERROR   3
#define FATAL   4
#define SIZE  1024

int startup(const char *ip,int port);
void print_log(const char* msg,int level);
void* handler_request(void* arg);
void* handler_request1(int sock,char* buf,int len);
void exe_cgi(int sock,char *method,char *path,char *query_string);
int get_line(int sock,char line[],int size);
#endif



