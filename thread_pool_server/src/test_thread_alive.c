#include <Thread_pool_server.h>



int test_thread_alive(pthread_t tid)
{
		pthread_kill(tid,0);//测试线程是否存活，返回的是系统的errno,有头文件直接用
		if(errno == ESRCH)
			return _FALSE;
		else
			return _TRUE;
}
