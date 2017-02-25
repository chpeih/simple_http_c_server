#include<stdio.h>
#include<ctype.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<errno.h>
#include<arpa/inet.h>
#include<iostream>
using namespace std;
#define MAXLINE 1024

int main( int argc, char **argv)
{
	if( argc != 3 )
	{
	  perror("usage client01 <IPaddress> Port");
	  exit(-1);
	}
	struct sockaddr_in servaddr;
	int sockfd[5];
	int port = atoi(argv[2]);
	for( int i = 0; i < 5 ;i++ )
	{
		sockfd[i] = socket( AF_INET, SOCK_STREAM, 0 );
		if( sockfd[i] == 0 )
		{
			perror("socket error ");
			exit(-1);
		}
		memset( &servaddr, 0, sizeof( servaddr ));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(port);
		inet_pton( AF_INET, argv[1], &servaddr.sin_addr);
		if ( connect( sockfd[i], (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		{
			perror("connect error");
		}
	}
	char sendline[MAXLINE], recvline[MAXLINE];
	while( fgets(sendline, MAXLINE,stdin) !=NULL){
		write(sockfd[0], sendline, strlen(sendline));
		if( read(sockfd[0], recvline, MAXLINE ) == 0 )
		  perror("sre_cli: server terminate prematurely");
		fputs(recvline, stdout);
	}
	return 0;
}
