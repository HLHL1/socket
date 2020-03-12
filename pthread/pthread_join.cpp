#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

void *thread_start(void *arg){

	int i = 0;
	for (i = 0; i < 8; i++){

		printf("Thread working...! %d \n", i);
		sleep(1);
	}
	char* p;
	p= "hello";
	pthread_exit(p);
}

int main(){

	pthread_t tid;
	int ret = pthread_create(&tid, NULL, thread_start, NULL);
	if (ret != 0){
		printf("Create pthread error!\n");
		exit(EXIT_FAILURE);
	}
	void* ans;
	int jret = pthread_join(tid, &ans);
	if (jret != 0){
		printf("Join pthread error!\n");
		exit(EXIT_FAILURE);
	}
	printf("thread return %s\n", (char*)ans);
	printf("thread done! \n");
	return 0;
}
