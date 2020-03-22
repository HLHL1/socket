#include<stdlib.h>
#include<pthread.h>
#include"XTcp.h"
#include <string.h>
#include<stdio.h>
#include<string>
#include<regex>
using namespace std;

void* thread_start(void* arg){

	XTcp client = *(XTcp*)arg;//��ȡ�ͻ���socket

	//���տͻ�������
	char buf[10000] = { 0 };//������յ�������

	for (;;){
		int recvlen = client.Recv(buf, sizeof(buf)-1);
		if (recvlen <= 0){
			client.Close();
		}
		buf[recvlen] = '\0';
		printf("=======recv=================\n%s=======================\n", buf);

		string src = buf;
		string pattern = "^([A-Z]+) (.+) HTTP/1";//������ʽ
		regex r(pattern);
		smatch mas;
		regex_search(src, mas, r);
		if (mas.size() == 0){

			printf("%s failed!\n", pattern.c_str());
			client.Close();
		}
		string type = mas[1];//??���Ǵ�0��ʼ��
		string path = mas[2];
		if (type != "GET"){

			client.Close();
		}
		string filename = path;
		//û���ļ�·������Ĭ��
		if (path == "/"){
			filename = "/index.html";
		}

		string filepath = "www";
		filepath += filename;
		FILE *fp = fopen(filepath.c_str(), "rb");
		if (fp == NULL){
			client.Close();
		}

		//��Ӧhttp GET����
		//��ȡ�ļ���С
		fseek(fp, 0, SEEK_END);
		int filesize = ftell(fp);
		fseek(fp, 0, 0);//??
		printf("file size is %d\n", filesize);
		string rmsg = "";
		rmsg = "HTTP/1.1 200 OK\r\n";//��Ӧ��
		rmsg += "Server:XHttp\r\n";//����������
		rmsg += "Content-Type:text/html\r\n";//������֧�ֵ�����
		rmsg += "Content-Length:";//��Ӧ���ݴ�С
		char bsize[128] = { 0 };
		sprintf(bsize, "%d", filesize);
		rmsg += bsize;
		rmsg += "\r\n\r\n";
		//������Ϣͷ
		int sendSize = client.Send(rmsg.c_str(), rmsg.size());
		printf("size=%d\n", sendSize);
		printf("=======send=================\n%s\n=======================\n", rmsg.c_str());
		//��������
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
		//�����߳�
		pthread_t tid;
		pthread_create(&tid, NULL, thread_start, (void*)&client);
		//�����߳������̷߳��룬���߳�ִ����Ϻ��Զ��ͷ���Դ
		pthread_detach(tid);

	}
	server.Close();
}
