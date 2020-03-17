
头文件：#include <pthread.h>
在编译时注意加上-lpthread参数，以调用动态链接库。因为pthread并非Linux系统的默认库。
## 1.线程创建
函数声明

```cpp
int pthread_create(pthread_t * thread, const pthread_arrt_t* attr,void*(*start_routine)(void *), void* arg);
```
>-thread：指向线程标识符的指针
-attr:线程的属性。给传递NULL表示设置为默认线程属性
-start_routine:线程执行实体入口
arg：运行函数的参数
-返回值:成功返回0,失败返回错误码
-typedef unsigned long int pthread_t

***线程id的类型是pthread_t,它只在当前进程中保证是唯一的,在不同的系统中pthread_t这个类型有不同的实现，调用pthread_self()可以获得当前线程的id.***

**代码演示：**

```cpp
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

void* thread_start(void* arg){
	
	int arg_data = *((int*)arg);
	for (int i = 0; i < 10; i++){
		//这里要注意一下
		//当创建一个线程之后,该线程和主线程一起执行，若主线程提前退出了，thread_start这个线程还没有来得及打印也会因为主线程退出而退出了
		for (int j = 0; j < 5000000; j++);
		printf("Receive arg %d=%d\n", arg_data, arg_data);
	}
}
int main(void){

	pthread_t tid ;
	int arg1 = 1;
	int arg2 = 2;
	//创建线程
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
运行结果：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200312222203393.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
## 2. 线程等待
函数声明：

```cpp
void pthread_join(pthread_t thread,void ** retval);
```
>-thread：线程标识符
-retval：用户定义的指针，用来存储被等待线程的返回值。
-如果执行成功，将返回0，如果失败则返回一个错误码。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200312212100668.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)


代码演示：

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
## 3.线程分离
函数说明：
>即主线程与子线程分离，子线程结束后，资源自动回收。
>
函数声明：

```cpp
int pthread_detach(pthread_t tid);
```
>-tid:线程标识符
>-返回值：成功返回0，失败返回错误码


### 线程资源回收
1.linux线程执行和windows不同，pthread有两种状态joinable状态和unjoinable状态，如果线程是joinable状态，当线程函数自己返回退出时或pthread_exit时都不会释放线程所占用堆栈和线程描述符。只有当你调用了pthread_join之后这些资源才会被释放。若是unjoinable状态的线程，这些资源在线程函数退出时或pthread_exit时自动会被释放。
2.unjoinable属性可以在pthread_create时指定，或在线程创建后在线程中pthread_detach自己, 如：pthread_detach(pthread_self())，将状态改为unjoinable状态，确保资源的释放。或者将线程置为 joinable,然后适时调用pthread_join.
3.其实简单的说就是在线程函数头加上 pthread_detach(pthread_self())的话，线程状态改变，在函数尾部直接 pthread_exit线程就会自动退出。省去了给线程擦屁股的麻烦。
代码演示：

```cpp
 pthread_t tid;
 int status = pthread_create(&tid, NULL, ThreadFunc, NULL);
 if(status != 0)
 {
  perror("pthread_create error");
 }
 pthread_detach(tid);

```

## 4.线程的终止
>==终止线程的三种方式：==
-从start_routine正常return
-显示调用pthread_exit(),终止自己
-线程被pthread_cancel()取消

### pthread_exit()函数
函数声明：

```cpp
void pthread_exit(void * retval);
```
>-retval:指向线程占用资源的指针。由于一个进程中的多个线程是共享数据段的，因此通常在线程退出之后，退出线程所占用的资源并不会随着线程的终止而得到释放。其它线程可以调用pthread_join获得这个指针。
代码演示：

```cpp
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>

void* thread_start(void* arg){

	int arg_data = *((int*)arg);
	for (int i = 0; i < 10; i++){
		//这里要注意一下
		//当创建一个线程之后,该线程和主线程一起执行，若主线程提前退出了，thread_start这个线程还没有来得及打印也会因为主线程退出而退出了
		for (int j = 0; j < 5000000; j++);
		printf("Receive arg %d=%d\n", arg_data, arg_data);
		pthread_exit(NULL);
	}
}
int main(void){

	pthread_t tid;
	int arg1 = 1;
	int arg2 = 2;
	//创建线程
	int ret = pthread_create(&tid, NULL, thread_start, (void*)&arg1);
	if (ret != 0){
		printf("Create pthread error!\n");
		exit(EXIT_FAILURE);
	}
	thread_start((void*)&arg2);
	return 0;
}

```
运行结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200312224047429.png)
### pthread_cancel()函数
函数声明：

```cpp
int pthread_cancel(pthread_t thread);

```
>-thread:目标线程的标识符。
-成功返回0，失败返回错误码。

代码演示：

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
运行结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2020031223031093.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)