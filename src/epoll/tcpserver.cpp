#include<stdlib.h>
#include<pthread.h>
#include"XTcp.h"
#include <string.h>
#include<stdio.h>
#include<sys/epoll.h>


int main(int argc, char* argv[]){

	unsigned short port = 8080;
	if (argc > 1){
		port = atoi(argv[1]);
	}
	XTcp server;
	server.CreateSocket();
	server.Bind(port);

	//��·����
	//����epoll
	int epfd = epoll_create(256);//����256���׽���

	//ע���¼�
	epoll_event ev;
	ev.data.fd = server.sock;
	ev.events = EPOLLIN | EPOLLET;//�Զ�Ӧ���ļ��������ɶ�����Ӧ���ļ��������趨Ϊedgeģʽ������״̬�����仯ʱ�Ż�֪ͨ��
	epoll_ctl(epfd,EPOLL_CTL_ADD, server.sock, &ev);
	epoll_event events[20];
	char buf[1024]={ 0 };
	const char* msg = "HTTP/1.1 200 OK\r\nContent-Length:1\r\n\r\nX";
	int size = strlen(msg);
	server.SetBlock(false);//������ģʽ
	for (;;){
		int count=epoll_wait(epfd, events, 20, 500);//�ڶ�������������
		if (count <= 0)continue;
		for (int i = 0; i < count; i++){

			if (events[i].data.fd == server.sock){

				for (;;){
					XTcp client = server.Accept();
					if (client.sock <= 0)break;
					ev.data.fd = client.sock;
					ev.events = EPOLLIN | EPOLLET;
					epoll_ctl(epfd, EPOLL_CTL_ADD, client.sock, &ev);
				}
			}
			else{
				XTcp client;
				client.sock = events[i].data.fd;
				client.Recv(buf, 1024);
				client.Send(msg,size);
				epoll_ctl(epfd, EPOLL_CTL_DEL, client.sock, &ev);
				client.Close();
			}
		}
	
	}
	server.Close();
}