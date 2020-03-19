头文件：

```cpp
#include<sys/epoll.h>
```
# 数据结构
epoll函数中所使用到的数据结构：

```cpp
typedef union epoll_data {
	void ptr;
	int fd;
	__uint32_t u32;
	__uint64_t u64;
} epoll_data_t;
 
struct epoll_event {
	__uint32_t events;    / Epoll events /
	epoll_data_t data;    / User data variable /
};
```
>结构体epoll_event 被用于注册所感兴趣的事件和回传所发生待处理的事件。
>events字段是表示感兴趣的事件和被触发的事件可能的取值为：
>-EPOLLIN ：表示对应的文件描述符可以读；
-EPOLLOUT：表示对应的文件描述符可以写；
-EPOLLPRI：表示对应的文件描述符有紧急的数据可读；
-EPOLLERR：表示对应的文件描述符发生错误；
-EPOLLHUP：表示对应的文件描述符被挂断；
-EPOLLET：表示对应的文件描述符设定为edge模式；
>epoll_data 联合体用来保存触发事件的某个文件描述符相关的数据。例如一个client连接到服务器，服务器通过调用accept函数可以得到于这个client对应的socket文件描述符，可以把这文件描述符赋给epoll_data的fd字段以便后面的读写操作在这个文件描述符上进行。

## epoll 的两种触发模式
EPOLLLT：LT（水平触发）是默认的模式，只要这个文件描述符还有数据可读，每次 epoll_wait都会返回它的事件，提醒用户程序去操作。只要有数据都会触发，缓冲区剩余未读尽的数据会导致epoll_wait返回。
EPOLLET：ET（边缘触发）是“高速模式”，在它检测到有 I/O 事件时，通过 epoll_wait 调用会得到有事件通知的文件描述符，对于每一个被通知的文件描述符，如可读，则必须将该文件描述符一直读到空，让 errno 返回 EAGAIN 为止，否则下次的 epoll_wait 不会返回余下的数据，会丢掉事件。如果ET模式不是非阻塞的，那这个一直读或一直写势必会在最后一次阻塞。只有数据到来才触发，不管缓存区中是否还有数据，缓冲区剩余未读尽的数据不会导致epoll_wait返回。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200319222846183.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)

epoll为什么要有EPOLLET触发模式？
如果采用EPOLLLT模式的话，系统中一旦有大量你不需要读写的就绪文件描述符，它们每次调用epoll_wait都会返回，这样会大大降低处理程序检索自己关心的就绪文件描述符的效率.。而采用EPOLLET这种边缘触发模式的话，当被监控的文件描述符上有可读写事件发生时，epoll_wait()会通知处理程序去读写。如果这次没有把数据全部读写完(如读写缓冲区太小)，那么下次调用epoll_wait()时，它不会通知你，也就是它只会通知你一次，直到该文件描述符上出现第二次可读写事件才会通知你！！！这种模式比水平触发效率高，系统不会充斥大量你不关心的就绪文件描述符。

# 函数
## 1.epoll_create()

```cpp
int epoll_create(int size);
```
>该函数生成一个epoll专用的文件描述符，其中的参数是指定生成描述符的最大范围。

## 2.epoll_ctl()

```cpp
int epoll_ctl(int epfd, int op, int fd, struct epoll_event event);
```
>该函数用于控制某个文件描述符上的事件，可以注册事件，修改事件，删除事件。
  参数：
  -epfd：由 epoll_create 生成的epoll专用的文件描述符；
   -op：要进行的操作例如注册事件，可能的取值
EPOLL_CTL_ADD 注册、
EPOLL_CTL_MOD 修改、
EPOLL_CTL_DEL 删除
-fd：关联的文件描述符；
-event：指向epoll_event的指针；
返回值：如果调用成功返回0,不成功返回-1

## 3.epoll_wait()

```cpp
int epoll_wait(int epfd,struct epoll_event   events,int maxevents,int timeout);
```

>该函数用于轮询I/O事件的发生；
参数：
-epfd:由epoll_create 生成的epoll专用的文件描述符；
-epoll_event:用于回传代处理事件的数组；
-maxevents:每次能处理的事件数；
-timeout:等待I/O事件发生的超时值（ms）；-1永不超时，直到有事件产生才触发，0立即返回。
返回值：返回发生事件数，-1有错误。


epoll的深层原理参考：https://www.jianshu.com/p/e6b9481ca754