#include<stdio.h>
#include<ctype.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>

int HTTP_SERVER_PORT = 9008;
int MAX_QUEUE_SIZE = 1000;

int main()
{

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
	servaddr.sin_port = htons(HTTP_SERVER_PORT);

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

	printf("http server start on port %d\n", HTTP_SERVER_PORT );


	for(;;)
	{
		clilen = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
		if( connfd < 0 )
		{
			perror("accept error");
			exit(-1);
		}
		if( (childpid = fork()) == 0 )
		{
			close(listenfd);
			printf("do sth on child pthread");
			exit(0);
		}
		close(connfd);

	}


	return 0;
}
