#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

void* thread_start(void* arg){

	int arg_data = *((int*)arg);
	for (int i = 0; i < 10; i++){
		//����Ҫע��һ��
		//������һ���߳�֮��,���̺߳����߳�һ��ִ�У������߳���ǰ�˳��ˣ�thread_start����̻߳�û�����ü���ӡҲ����Ϊ���߳��˳����˳���
		for (int j = 0; j < 5000000; j++);
		printf("Receive arg %d=%d\n", arg_data, arg_data);
		pthread_exit(NULL);
	}
}
int main(void){

	pthread_t tid;
	int arg1 = 1;
	int arg2 = 2;
	//�����߳�
	int ret = pthread_create(&tid, NULL, thread_start, (void*)&arg1);
	if (ret != 0){
		printf("Create pthread error!\n");
		exit(EXIT_FAILURE);
	}
	thread_start((void*)&arg2);
	return 0;
}
