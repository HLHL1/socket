#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>


void* thread_start(void* arg){
	int client = *(int*)arg;
	//保存接收到的数据
	char buf[1024] = { 0 };
	for (;;){
		int recvlen = recv(client, buf, sizeof(buf)-1, 0);
		if (recvlen <= 0)break;
		buf[recvlen] = '\0';
		if (strstr(buf, "quit") != NULL){

			char re[] = "quit success!\n";
			send(client, re, strlen(re) + 1, 0);
			break;
		}
		int sendlen = send(client, "ok\n", 4, 0);
		printf("recv %s\n", buf);
	}
	close(client);
}

int main(int argc, char* argv[]){


	//创建socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1){
		printf("create socket failed!\n");
		return -1;
	}

	//绑定一个端口
	unsigned short port = 9999;
	if (argc > 1){
		port = atoi(argv[1]);//将字符串转换为一个整数
	}
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);//将主机字节顺序转换为网络字节顺序【默认大端】(处理大端小端问题)
	saddr.sin_addr.s_addr = htonl(0);
	if (bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0){
		printf("bind port %d failed!\n", saddr.sin_port);
		return -2;
	}
	printf("bind port %d success!\n", port);

	listen(sock, 10);//在一段时间内最大可以让多少个链接进来

	for (;;){
		//接受客户端请求
		sockaddr_in caddr;
		socklen_t len = sizeof(caddr);
		//int client=accept(sock, 0, 0);
		int client = accept(sock, (sockaddr*)&caddr, &len);
		if (client <= 0)break;
		printf("accept client %d \n", client);
		char* ip = inet_ntoa(caddr.sin_addr);
		unsigned short cport = ntohs(caddr.sin_port);
		printf("client ip is %s,port is %d\n", ip, cport);

		//创建线程
		pthread_t tid;
		pthread_create(&tid, NULL, thread_start, (void*)&client);
		//将主线程与子线程分离，子线程执行完毕后自动释放资源
		pthread_detach(tid);
	}

	close(sock);
	return 0;
}
