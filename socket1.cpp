#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
int main(){

	for (int i = 0; i < 10; i++){
		int sock = socket(AF_INET, SOCK_STREAM, 0);//创建socket
		if (sock == -1){
			printf("create socket failed!\n");
		}
		printf("%d\n", sock);
		close(sock);//关闭socket
	}
	
	return 0;
}
