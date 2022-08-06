#include <Thread_pool_server.h>


/*线程池创建初始化*/

pool_t * thread_pool_create(int tmax,int tmin,int qmax)
{
	pool_t * ptr = NULL;
	if((ptr = (pool_t *)malloc(sizeof(pool_t)))==NULL)
		thread_str_error("thread_pool_create ERROR, Malloc Pool failed",1,0);
	ptr->thread_max = tmax;
	ptr->thread_min = tmin;
	ptr->thread_busy = 0;
	ptr->thread_alive = 0;
	ptr->thread_shutdown = _TRUE;
	ptr->thread_exitcode = 0;
	ptr->queue_max = qmax;
	ptr->queue_cur = 0;
	ptr->queue_front = 0;
	ptr->queue_rear = 0;
	if((ptr->queue_list = (task_t *)malloc(qmax * sizeof(task_t)))==NULL)
		thread_str_error("thread_pool_create ERROR, Malloc queue list failed",1,0);
	if((ptr->Ctids = (pthread_t *)malloc(tmax * sizeof(pthread_t)))==NULL)
		thread_str_error("thread_pool_create ERROR, Malloc Ctids failed",1,0);
	bzero(ptr->Ctids,sizeof(pthread_t));//初始化Ctids

	/*初始化锁和条件变量*/
	if(pthread_mutex_init(&ptr->lock,NULL)!=0 || pthread_cond_init(&ptr->not_full,NULL)!=0 || pthread_cond_init(&ptr->not_empty,NULL)!=0){
		printf("thread_pool_create ERROR ,init lock or cond Failed\n");
		exit(1);
	}
	
	//预创建线程，按最小值
	int err;
	for(int i = 0;i < tmin;i++){
		if((err = pthread_create(&ptr->Ctids[i],NULL,thread_customer,(void*)ptr))>0)
			thread_str_error("thread_pool_create Error,create customer thread failed",2,err);
		++ptr->thread_alive;//消费者创建成功，存活量+1
	}
	if((err = pthread_create(&ptr->Mtid,NULL,thread_manager,(void*)ptr))>0)
			thread_str_error("thread_pool_create Error,create manager thread failed",2,err);

	printf("[Initializer...] Thread Pool Create Success..\n");
	return ptr;
}
