#include <Thread_pool_server.h>

int thread_pool_netinit(){
	int sockfd;
	struct sockaddr_in serverAddr;
	bzero(&serverAddr,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(_PORT);
	inet_pton(AF_INET,_IP,&serverAddr.sin_addr.s_addr);
	if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
		thread_str_error("thread_pool_netinit Error,socket creat Failed",1,0);
	if((bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)))==-1)
		thread_str_error("thread_pool_netinit Error,socket bind Failed",1,0);
	if((listen(sockfd,_BACKLOG))==-1){
		thread_str_error("thread_pool_netinit Error,socket LISTEN Failed",1,0);
	}
	printf("[Initializer...] Thread Pool Net Init Success,Serverfd[%d]...\n",sockfd);



	return sockfd;
}
