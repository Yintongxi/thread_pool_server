#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <ctype.h>

#define _BACKLOG 128
#define _EPOLLMAX 100000
#define _BUFSIZE 1500
#define _TRUE 1
#define _FALSE 0
#define _DEFCODE 10 //线程扩容和缩减用的，一次扩容10个或者一次缩减10个
#define _IPSIZE 16
//#define _IP "192.168.31.26"
#define _IP "192.168.31.30"
#define _PORT 8080
#define _TIMEOUT 2 //1s钟管理者扫描下线程池内情况

/*高并发低活跃 IO复用+线程池*/

int epfd;//监听树描述符（全局），epoll自带线程安全，epoll_ctl函数内部使用了互斥锁，不需要担心冲突问题
pthread_mutex_t accept_lock;//连接锁

/*任务类型*/
typedef struct{
	void * (*business)(void *);
	void * arg;
}task_t;


/*线程池类型*/
typedef struct{
	int thread_max;//线程最大数
	int thread_min;//线程最小数
	int thread_busy;//繁忙线程数
	int thread_alive;//有效线程数
	int thread_shutdown;//线程池开关
	int thread_exitcode;//缩减码
	int queue_max;//任务队列最大数
	int queue_cur;//任务队列当前最大任务数
	int queue_front;//任务队列头索引
	int queue_rear;//任务队列尾索引
	task_t * queue_list;//任务队列地址,也就是环形队列数组
	pthread_mutex_t lock;//互斥锁
	pthread_cond_t not_full;//生产者条件变量
	pthread_cond_t not_empty;//消费者条件变量
	pthread_t * Ctids;//储存消费者线程id的数组
	pthread_t Mtid;//储存管理者线程

}pool_t;

void thread_str_error(const char* errstr,int exitcode, int flag);//错误处理函数，既可以处理系统函数的错误（perror）,也兼容线程错误处理（strerror）
pool_t * thread_pool_create(int tmax, int tmin, int qmax);//线程池创建初始化
int thread_pool_destroy(pool_t * ptr);
int producer_add_task(pool_t * ptr,task_t task);//生产者任务（添加）
void * thread_customer(void *);//消费者任务
void * thread_manager(void *);//管理者任务
int  test_thread_alive(pthread_t tid);//测试线程是否存活

void * Business_Accept(void *);//连接业务实现
void * Business_Response(void *);//数据请求业务实现

int thread_pool_netinit(void);//网络创建初始化
int thread_epoll_init(int sockfd);//epoll创建初始化
int thread_epoll_listen(pool_t * ptr,int sockfd);//Epoll监听
int Server_StartUp(void);






