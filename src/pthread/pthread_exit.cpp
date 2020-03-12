#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

void* thread_start(void* arg){

	int arg_data = *((int*)arg);
	for (int i = 0; i < 10; i++){
		//这里要注意一下
		//当创建一个线程之后,该线程和主线程一起执行，若主线程提前退出了，thread_start这个线程还没有来得及打印也会因为主线程退出而退出了
		for (int j = 0; j < 5000000; j++);
		printf("Receive arg %d=%d\n", arg_data, arg_data);
		pthread_exit(NULL);
	}
}
int main(void){

	pthread_t tid;
	int arg1 = 1;
	int arg2 = 2;
	//创建线程
	int ret = pthread_create(&tid, NULL, thread_start, (void*)&arg1);
	if (ret != 0){
		printf("Create pthread error!\n");
		exit(EXIT_FAILURE);
	}
	thread_start((void*)&arg2);
	return 0;
}
