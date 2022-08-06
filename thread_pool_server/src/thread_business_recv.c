#include <Thread_pool_server.h>

void * Business_Response(void * arg)
{
	int clientFd = *(int *)arg;
	time_t tp;
	ssize_t recvlen;
	char buffer[_BUFSIZE];
	char time_buffer[1024];
	int flag;
	bzero(time_buffer,1024);
	bzero(buffer,_BUFSIZE);
	while((recvlen = recv(clientFd,buffer,sizeof(buffer),MSG_DONTWAIT))>0){//非阻塞读取IO
		if((strcmp(buffer,"timeofday\n"))==0){
			tp = time(NULL);
			ctime_r(&tp,time_buffer);
			send(clientFd,time_buffer,strlen(time_buffer),MSG_NOSIGNAL);
			bzero(time_buffer,1024);
		}else{
			flag = 0;
			while(recvlen > flag){
				buffer[flag] = toupper(buffer[flag]);
				flag++;
			}
			send(clientFd,buffer,strlen(buffer),MSG_NOSIGNAL);//忽略信号，防止服务器在回客户端时，客户端关闭，内核发信号SIGNPIPE将服务器杀死
		}
		printf("[Customer 0x%x] Exec Business Response,Exec Data return Reaponse to Client!\n",(unsigned int)pthread_self());
	}
	if(recvlen == -1){
		thread_str_error("Business_Response ERROR,recv Requst Failed",1,0);
	}
	if(recvlen == 0){
		printf("[Customer 0x%x]Client exiting ,Close ClientFd [%d]\n",(unsigned int)pthread_self(),clientFd);
		epoll_ctl(epfd,EPOLL_CTL_DEL,clientFd,NULL);//删除监听节点
		close(clientFd);	
	}
	return NULL;

}
