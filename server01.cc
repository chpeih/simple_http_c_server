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
using namespace std;
int HTTP_SERVER_PORT = 9008;
int MAX_QUEUE_SIZE = 1000;
int BUFF_SIZE = 1000;

void sig_chld(int signo )
{
	pid_t pid;
	int stat;
	while( ( pid = waitpid( -1, &stat, WNOHANG )) > 0 )
	  printf("child %d terminateed\n", pid);
	return;
}
typedef void Sigfunc(int);
Sigfunc * signal( int signo, Sigfunc *func )
{
	struct sigaction act, oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if( signo == SIGALRM ){
#ifdef SA_INTERRUPT
		act.sa_flags |= SA_INTERRUPT;
#endif
	}else{
#ifdef SA_RESTART
		act.sa_flags |= SA_RESTART;
#endif
	}
	if ( sigaction( signo, &act, &oact ) < 0 )
	  return (SIG_ERR);
	return (oact.sa_handler);
}

Sigfunc*
Signal( int signo, Sigfunc *func )
{
	Sigfunc *sigfunc;
	if( ( sigfunc = signal( signo, func )) == SIG_ERR )
	  perror("signal error");
	return (sigfunc);
}

int main( int argc, char *argv[])
{

	if( argc != 2 )
	{
		printf("usage: %s port\n", argv[0]);
		return 1;
	}
	int port = atoi(argv[1]);
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;
	listenfd = socket( AF_INET, SOCK_STREAM, 0);
	if(listenfd == 0)
	{
		perror("socket error!");
		exit(-1);
	}
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);

	if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 )
	{
		perror("bind error");
		exit(-1);
	}

	if(listen(listenfd, MAX_QUEUE_SIZE) < 0 )
	{
		perror("listen error");
		exit(-1);
	}

	Signal( SIGCHLD, sig_chld);

	printf("http server start on port %d\n", port);

	int count = 0;
	for(;;)
	{
		clilen = sizeof(cliaddr);
		if ( (connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen) ) < 0 ) {
			if ( errno == EINTR )
			  continue;
			else{
			  perror("accept error");
			  exit(-1);
			}
		}

		printf("accept a client %d\n", count);
		count++;


		if( (childpid = fork()) == 0 )
		{
			close(listenfd);
			char recv_buff[BUFF_SIZE];
			int recv_len = recv(connfd, recv_buff, BUFF_SIZE, 0);
			printf("reveive %d\n", recv_len);
			printf("buff:%s\n",recv_buff); 
			char send_buff[BUFF_SIZE];
			sprintf(send_buff, "HTTP/1.1 200 OK\r\n");
			send(connfd, send_buff, strlen(send_buff), 0);
			sprintf( send_buff, "Server: hoohackhttpd/0.1.0\r\n");
			send(connfd, send_buff, strlen(send_buff), 0);
			sprintf( send_buff, "Content-Type: text/html\r\n");
			send(connfd, send_buff, strlen(send_buff), 0);
			strcpy(send_buff, "\r\n");
			send(connfd, send_buff, strlen(send_buff), 0);
			sprintf(send_buff, "Hello, this is from sysu http server\r\n");
			send(connfd, send_buff, strlen(send_buff), 0);
			close(connfd);
			exit(1);
		}
		close(connfd);

	}


	return 0;
}
