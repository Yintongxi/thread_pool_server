#include <Thread_pool_server.h>



int thread_epoll_listen(pool_t *ptr,int sockfd){

	int readycode;
	struct epoll_event readyArray[_EPOLLMAX];
	int flag;
	task_t task;
	
	printf("[Listen...] Thread Epoll Listen...\n");
	while(ptr->thread_shutdown){
		flag=0;
		if((readycode = epoll_wait(epfd,readyArray,_EPOLLMAX,-1))==-1)
		  thread_str_error("thread_epoll_listen Error,epoll_wait call Failed",1,0);
		while(readycode){
			//辨别就绪，添加对应的绑定业务
			if(readyArray[flag].data.fd == sockfd){
				task.business = Business_Accept;
				task.arg = (void *)&sockfd;
				producer_add_task(ptr,task);//添加连接服务
				printf("[Producer 0x%x ,Add Business_Accept to Container\n",(unsigned int)pthread_self());

			}else{
				task.business = Business_Response;
				task.arg = (void *)&(readyArray[flag].data.fd);
				producer_add_task(ptr,task);
				printf("[Producer 0x%x ,Add Business_Response to Container\n",(unsigned int)pthread_self());

			}
			++flag;
			--readycode;

		}
	}
	close(sockfd);
	return 0;

}
