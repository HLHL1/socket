#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

int sum = 0;
void* thread_start(void* arg){
	int i;
	int len = *(int *)arg;
	for (int i = 0; i < len; i++){
		sum = i;
		sleep(1);
		printf("sub thread:sum=%d\n", sum);
	}
	printf("sub thread:exit\n");
	//pthread_exit(NULL);
}
int main(){
	pthread_t tid;
	int arg = 10;
	pthread_create(&tid, NULL, thread_start, (void*)&arg);
	sleep(5);
	pthread_cancel(tid);
	printf("main thread:sum=%d\n", sum);
	printf("main thread exit\n");
	return 0;
}