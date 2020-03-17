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

	//多路复用
	//创建epoll
	int epfd = epoll_create(256);//最多放256个套接字

	//注册事件
	epoll_event ev;
	ev.data.fd = server.sock;
	ev.events = EPOLLIN | EPOLLET;//对对应的文件描述符可读，对应的文件描述符设定为edge模式（仅当状态发生变化时才会通知）
	epoll_ctl(epfd,EPOLL_CTL_ADD, server.sock, &ev);
	epoll_event events[20];
	char buf[1024]={ 0 };
	const char* msg = "HTTP/1.1 200 OK\r\nContent-Length:1\r\n\r\nX";
	int size = strlen(msg);
	server.SetBlock(false);//非阻塞模式
	for (;;){
		int count=epoll_wait(epfd, events, 20, 500);//第二个参数有问题
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