
ͷ�ļ���#include <pthread.h>
�ڱ���ʱע�����-lpthread�������Ե��ö�̬���ӿ⡣��Ϊpthread����Linuxϵͳ��Ĭ�Ͽ⡣
## 1.�̴߳���
��������

```cpp
int pthread_create(pthread_t * thread, const pthread_arrt_t* attr,void*(*start_routine)(void *), void* arg);
```
>-thread��ָ���̱߳�ʶ����ָ��
-attr:�̵߳����ԡ�������NULL��ʾ����ΪĬ���߳�����
-start_routine:�߳�ִ��ʵ�����
arg�����к����Ĳ���
-����ֵ:�ɹ�����0,ʧ�ܷ��ش�����
-typedef unsigned long int pthread_t

***�߳�id��������pthread_t,��ֻ�ڵ�ǰ�����б�֤��Ψһ��,�ڲ�ͬ��ϵͳ��pthread_t��������в�ͬ��ʵ�֣�����pthread_self()���Ի�õ�ǰ�̵߳�id.***

**������ʾ��**

```cpp
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
	}
}
int main(void){

	pthread_t tid ;
	int arg1 = 1;
	int arg2 = 2;
	//�����߳�
	int ret = pthread_create(&tid, NULL, thread_start, (void*)&arg1);
	if (ret != 0){
		printf("Create pthread error!\n");
		exit(EXIT_FAILURE);
	}
    //	sleep(10);
	thread_start((void*)&arg2);
	return 0;
}

```
���н����

![���������ͼƬ����](https://img-blog.csdnimg.cn/20200312222203393.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
## 2. �̵߳ȴ�
����������

```cpp
void pthread_join(pthread_t thread,void ** retval);
```
>-thread���̱߳�ʶ��
-retval���û������ָ�룬�����洢���ȴ��̵߳ķ���ֵ��
-���ִ�гɹ���������0�����ʧ���򷵻�һ�������롣

![���������ͼƬ����](https://img-blog.csdnimg.cn/20200312212100668.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)


������ʾ��

```cpp
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

void *thread_start(void *arg){

	int i = 0;
	for (i = 0; i < 8; i++){

		printf("Thread working...! %d \n", i);
		sleep(1);
	}
	char* p;
	p= "hello";
	pthread_exit(p);
}

int main(){

	pthread_t tid;
	int ret = pthread_create(&tid, NULL, thread_start, NULL);
	if (ret != 0){
		printf("Create pthread error!\n");
		exit(EXIT_FAILURE);
	}
	void* ans;
	int jret = pthread_join(tid, &ans);
	if (jret != 0){
		printf("Join pthread error!\n");
		exit(EXIT_FAILURE);
	}
	printf("thread return %s\n", (char*)ans);
	printf("thread done! \n");
	return 0;
}

```
## 3.�̷߳���
����˵����
>�����߳������̷߳��룬���߳̽�������Դ�Զ����ա�
>
����������

```cpp
int pthread_detach(pthread_t tid);
```
>-tid:�̱߳�ʶ��
>-����ֵ���ɹ�����0��ʧ�ܷ��ش�����


### �߳���Դ����
1.linux�߳�ִ�к�windows��ͬ��pthread������״̬joinable״̬��unjoinable״̬������߳���joinable״̬�����̺߳����Լ������˳�ʱ��pthread_exitʱ�������ͷ��߳���ռ�ö�ջ���߳���������ֻ�е��������pthread_join֮����Щ��Դ�Żᱻ�ͷš�����unjoinable״̬���̣߳���Щ��Դ���̺߳����˳�ʱ��pthread_exitʱ�Զ��ᱻ�ͷš�
2.unjoinable���Կ�����pthread_createʱָ���������̴߳��������߳���pthread_detach�Լ�, �磺pthread_detach(pthread_self())����״̬��Ϊunjoinable״̬��ȷ����Դ���ͷš����߽��߳���Ϊ joinable,Ȼ����ʱ����pthread_join.
3.��ʵ�򵥵�˵�������̺߳���ͷ���� pthread_detach(pthread_self())�Ļ����߳�״̬�ı䣬�ں���β��ֱ�� pthread_exit�߳̾ͻ��Զ��˳���ʡȥ�˸��̲߳�ƨ�ɵ��鷳��
������ʾ��

```cpp
 pthread_t tid;
 int status = pthread_create(&tid, NULL, ThreadFunc, NULL);
 if(status != 0)
 {
  perror("pthread_create error");
 }
 pthread_detach(tid);

```

## 4.�̵߳���ֹ
>==��ֹ�̵߳����ַ�ʽ��==
-��start_routine����return
-��ʾ����pthread_exit(),��ֹ�Լ�
-�̱߳�pthread_cancel()ȡ��

### pthread_exit()����
����������

```cpp
void pthread_exit(void * retval);
```
>-retval:ָ���߳�ռ����Դ��ָ�롣����һ�������еĶ���߳��ǹ������ݶεģ����ͨ�����߳��˳�֮���˳��߳���ռ�õ���Դ�����������̵߳���ֹ���õ��ͷš������߳̿��Ե���pthread_join������ָ�롣
������ʾ��

```cpp
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

```
���н����
![���������ͼƬ����](https://img-blog.csdnimg.cn/20200312224047429.png)
### pthread_cancel()����
����������

```cpp
int pthread_cancel(pthread_t thread);

```
>-thread:Ŀ���̵߳ı�ʶ����
-�ɹ�����0��ʧ�ܷ��ش����롣

������ʾ��

```cpp
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

int sum = 0;
void* thread_start(void* arg){
	int i;
	int len = *(int *)arg;
	for (int i = 0; i < len; i++){
		sum = i;
		sleep(1);
		printf("sub thread:sum=%d\n", sum);
	}
	printf("sub thread:exit\n");
	//pthread_exit(NULL);
}
int main(){
	pthread_t tid;
	int arg = 10;
	pthread_create(&tid, NULL, thread_start, (void*)&arg);
	sleep(5);
	pthread_cancel(tid);
	printf("main thread:sum=%d\n", sum);
	printf("main thread exit\n");
	return 0;
}
```
���н����
![���������ͼƬ����](https://img-blog.csdnimg.cn/2020031223031093.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)