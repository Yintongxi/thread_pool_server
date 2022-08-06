#include <Thread_pool_server.h>


void * Business_Accept(void * arg)
{
	int Serverfd = *(int *)arg;
	int clientfd;
	char ip[_IPSIZE];
	bzero(ip,_IPSIZE);
	struct epoll_event node;
	struct sockaddr_in clientAddr;
	socklen_t Addrlen;
	Addrlen = sizeof(clientAddr);
	pthread_mutex_lock(&accept_lock);
	if((clientfd = accept(Serverfd,(struct sockaddr *)&clientAddr,&Addrlen))==-1)
		thread_str_error("Business_accept Error, Accept call Failed",1,0);
	pthread_mutex_unlock(&accept_lock);
	node.data.fd = clientfd;
	node.events = EPOLLIN|EPOLLET;
	if((epoll_ctl(epfd,EPOLL_CTL_ADD,clientfd,&node))==-1)
		thread_str_error("Business_accept Error, epoll_ctl add  Failed",1,0);
	printf("[Customer 0x%x] Exec Business Accept Success,New Clientfd[%d] ClientIp[%s] Clientport[%d]\n",(unsigned int)pthread_self(),clientfd,inet_ntop(AF_INET,&clientAddr.sin_addr.s_addr,ip,16),ntohs(clientAddr.sin_port));
	return NULL;
}
