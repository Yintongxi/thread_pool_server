#include <Thread_pool_server.h>



int Server_StartUp()
{
	pool_t *ptr = NULL;
	int serverFd;
	printf("Thread POOL Server [EPOLL] Version 1.10 Running...\n");
	pthread_mutex_init(&accept_lock,NULL);//初始化连接锁
	ptr = thread_pool_create(300,10,1000);//线程池创建初始化
	serverFd = thread_pool_netinit();//网络初始化
	thread_epoll_init(serverFd);//Epoll创建初始化，设置第一个sock监听
	thread_epoll_listen(ptr,serverFd);//生产者EPOLL监听事件并添加任务
	return 0;
}
