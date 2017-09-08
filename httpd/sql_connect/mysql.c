//#include <stdio.h>
//#include <pthread.h>
//#include <dlfcn.h>
//#include <my_global.h>
//#include <mysql.h>
//
//int main()
//{
//	printf("MYSQL client version: %s\n",mysql_get_client_info());
//	return 0;
#include <my_global.h> 
#include <mysql.h> 
int main(int argc, char **argv) { printf("MySQL client version: %s\n", mysql_get_client_info()); return 0; }
//}
//
