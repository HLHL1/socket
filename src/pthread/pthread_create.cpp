#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

//线程一
void* thread_start(void* arg){
	
	int arg_data = *((int*)arg);
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 5000000; j++);
		printf("Receive arg %d=%d\n", arg_data, arg_data);
	}
}
int main(void){

	pthread_t tid ;
	int arg1 = 1;
	int arg2 = 2;
	//创建线程
	int ret = pthread_create(&tid, NULL, thread_start, (void*)&arg1);
	if (ret != 0){
		printf("Create pthread error!\n");
		exit(EXIT_FAILURE);
	}
//	sleep(10);
	thread_start((void*)&arg2);
	return 0;
}
