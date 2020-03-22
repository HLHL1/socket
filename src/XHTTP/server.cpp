#include<stdlib.h>
#include<pthread.h>
#include"XTcp.h"
#include <string.h>
#include<stdio.h>
#include<string>
#include<regex>
using namespace std;

void* thread_start(void* arg){

	XTcp client = *(XTcp*)arg;//获取客户端socket

	//接收客户端请求
	char buf[10000] = { 0 };//保存接收到的数据

	for (;;){
		int recvlen = client.Recv(buf, sizeof(buf)-1);
		if (recvlen <= 0){
			client.Close();
		}
		buf[recvlen] = '\0';
		printf("=======recv=================\n%s=======================\n", buf);

		string src = buf;
		string pattern = "^([A-Z]+) (.+) HTTP/1";//正则表达式
		regex r(pattern);
		smatch mas;
		regex_search(src, mas, r);
		if (mas.size() == 0){

			printf("%s failed!\n", pattern.c_str());
			client.Close();
		}
		string type = mas[1];//??不是从0开始？
		string path = mas[2];
		if (type != "GET"){

			client.Close();
		}
		string filename = path;
		//没有文件路径，给默认
		if (path == "/"){
			filename = "/index.html";
		}

		string filepath = "www";
		filepath += filename;
		FILE *fp = fopen(filepath.c_str(), "rb");
		if (fp == NULL){
			client.Close();
		}

		//回应http GET请求
		//获取文件大小
		fseek(fp, 0, SEEK_END);
		int filesize = ftell(fp);
		fseek(fp, 0, 0);//??
		printf("file size is %d\n", filesize);
		string rmsg = "";
		rmsg = "HTTP/1.1 200 OK\r\n";//响应行
		rmsg += "Server:XHttp\r\n";//服务器类型
		rmsg += "Content-Type:text/html\r\n";//服务器支持的类型
		rmsg += "Content-Length:";//响应内容大小
		char bsize[128] = { 0 };
		sprintf(bsize, "%d", filesize);
		rmsg += bsize;
		rmsg += "\r\n\r\n";
		//发送消息头
		int sendSize = client.Send(rmsg.c_str(), rmsg.size());
		printf("size=%d\n", sendSize);
		printf("=======send=================\n%s\n=======================\n", rmsg.c_str());
		//发送正文
		for (;;){
			int len = fread(buf, 1, sizeof(buf), fp);
			if (len <= 0)break;
			int re = client.Send(buf, len);
			if (re <= 0)break;
		}
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
		XTcp client = server.Accept();
		//创建线程
		pthread_t tid;
		pthread_create(&tid, NULL, thread_start, (void*)&client);
		//将主线程与子线程分离，子线程执行完毕后自动释放资源
		pthread_detach(tid);

	}
	server.Close();
}
