#include <Thread_pool_server.h>


void * thread_customer(void * arg)
{
		pool_t * ptr = (pool_t *)arg;
		task_t tmp;
		printf("[Customer 0x%x] Wait Business...\n",(unsigned int)pthread_self());
		while(ptr->thread_shutdown){//如果用if判断的话，一定要加锁，shutdown是全局资源
			pthread_mutex_lock(&ptr->lock);
			while(ptr->queue_cur == 0){//注意：一定要用while,因为如果是多核，将唤醒多个，避免误判端
				pthread_cond_wait(&ptr->not_empty,&ptr->lock);
				if(ptr->thread_exitcode > 0){
						--(ptr->thread_alive);
						--(ptr->thread_exitcode);
						pthread_mutex_unlock(&ptr->lock);
						pthread_exit(NULL);
				}
			}
			//判断开关
			if(ptr->thread_shutdown == 0){
					--(ptr->thread_alive);//该消费者线程将关闭，线程池中线程存活数-1
					pthread_mutex_unlock(&ptr->lock);
					pthread_exit(NULL);
			}
			//获取任务并执行
			tmp.business = ptr->queue_list[ptr->queue_rear].business;
			tmp.arg = ptr->queue_list[ptr->queue_rear].arg;//从头放，从尾拿，位追头
			--(ptr->queue_cur);
			ptr->queue_rear = (ptr->queue_rear + 1) % ptr->queue_max;
			++(ptr->thread_busy);
			pthread_mutex_unlock(&ptr->lock);
			//执行任务前要把锁先解除
			(tmp.business)(tmp.arg);
			pthread_mutex_lock(&ptr->lock);//任务执行完毕，立即加锁，好操作条件变量
			--(ptr->thread_busy);
			pthread_mutex_unlock(&ptr->lock);
			pthread_cond_signal(&ptr->not_full);//唤醒生产者
		}
		pthread_exit(NULL);
}
