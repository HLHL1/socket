#include<stdio.h>
#include"XTcp.h"

int main(){

	XTcp client;
	client.CreateSocket();
	client.Connect("192.168.0.104", 8080);
	client.Send("client", 6);
	char buf[1024] = { 0 };
	client.Recv(buf, sizeof(buf));
	printf("%s\n", buf);
	return 0;
}