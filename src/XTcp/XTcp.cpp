#include"XTcp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<pthread.h>

//���캯��
XTcp::XTcp(){

}

//����socket
int XTcp::CreateSocket(){

	sock = socket(AF_INET, SOCK_STREAM, 0);//????
	if (sock == -1){
		printf("create socket failed!\n");
	}
	return sock;
}

bool XTcp::Bind(unsigned short port){
	
	if (sock <= 0){
		CreateSocket();
	}
	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);//�������ֽ�˳��ת��Ϊ�����ֽ�˳��Ĭ�ϴ�ˡ�(������С������)
	saddr.sin_addr.s_addr = htonl(0);
	if (bind(sock, (sockaddr*)&saddr, sizeof(saddr)) != 0){
		printf("bind port %d failed!\n", saddr.sin_port);
		return false;
	}
	printf("bind port %d success!\n", port);
	listen(sock, 10);//��һ��ʱ�����������ö��ٸ����ӽ���
	return true;
}

XTcp XTcp::Accept(){
	XTcp tcp;
	//���ܿͻ�������
	sockaddr_in caddr;
	socklen_t len = sizeof(caddr);
	int client = accept(sock, (sockaddr*)&caddr, &len);
	if (client <= 0) return tcp;
	printf("accept client %d \n", client);
	tcp.ip = inet_ntoa(caddr.sin_addr);
	tcp.port = ntohs(caddr.sin_port);
	tcp.sock = client;
	printf("client ip is %s,port is %d\n", tcp.ip.c_str(), tcp.port);
	return tcp;
}

int XTcp::Recv(char*buf, int bufsize){
	return recv(sock, buf, bufsize, 0);
}
int XTcp::Send(const char*buf, int size){
	int s = 0;
	while(s!=size){
		int len = send(sock, buf + s, size - s, 0);
		if (len <= 0)break;
		s += len;
	}
	return s;

}

void XTcp::Close(){
	if (sock <= 0)return;
	close(sock);
}
XTcp::~XTcp()
{
}
