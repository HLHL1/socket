#include<stdlib.h>
#include<pthread.h>
#include"XTcp.h"
#include <string.h>
#include<stdio.h>

void* thread_start(void* arg){
	XTcp client = *(XTcp*)arg;
	//������յ�������
	char buf[1024] = { 0 };
	for (;;){
		int recvlen = client.Recv(buf, sizeof(buf)-1);
		if (recvlen <= 0)break;
		buf[recvlen] = '\0';
		if (strstr(buf, "quit") != NULL){

			char re[] = "quit success!\n";
			client.Send(re, strlen(re) + 1);
			break;
		}
		int sendlen = client.Send( "ok\n", 4);
		printf("recv %s\n", buf);
	}
	client.Close();
}

int main(int argc, char* argv[]){

	unsigned short port = 8080;
	if (argc > 1){
		port = atoi(argv[1]);
	}
	XTcp server;
	server.CreateSocket();
	server.Bind(port);
	for (;;){
		XTcp client=server.Accept();
		//�����߳�
		pthread_t tid;
		pthread_create(&tid, NULL, thread_start, (void*)&client);
		//�����߳������̷߳��룬���߳�ִ����Ϻ��Զ��ͷ���Դ
		pthread_detach(tid);
	}
	server.Close();
}