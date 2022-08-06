#include <Thread_pool_server.h>



void thread_str_error(const char* errstr, int exitcode , int flag)
{
		if(!flag){//flag=0为系统函数
				perror(errstr);
				exit(exitcode);
		}
		printf("%s:%s\n",errstr,strerror(flag));
		exit(exitcode);
}
