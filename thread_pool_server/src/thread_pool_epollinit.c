#include <Thread_pool_server.h>


//创建监听树，并添加第一个监听
int thread_epoll_init(int sockfd){

	if((epfd = epoll_create(_EPOLLMAX))==-1)
		thread_str_error("thread_epoll_init Error,epoll_create call Failed",1,0);
	struct epoll_event node;
	node.data.fd = sockfd;
	node.events = EPOLLIN|EPOLLET;
	if((epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&node))==-1)
		thread_str_error("thread_epoll_init Error,epoll first add Serverfd Failed",1,0);

	printf("[Initializer...] THREAD EPOLL CREATE Success epfd[%d]\n",epfd);
	return 0;
}
