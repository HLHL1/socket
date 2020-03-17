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
>结构体==epoll_event== 被用于注册所感兴趣的事件和回传所发生待处理的事件。
>events字段是表示感兴趣的事件和被触发的事件可能的取值为：
>-EPOLLIN ：表示对应的文件描述符可以读；
-EPOLLOUT：表示对应的文件描述符可以写；
-EPOLLPRI：表示对应的文件描述符有紧急的数据可读
-EPOLLERR：表示对应的文件描述符发生错误；
-EPOLLHUP：表示对应的文件描述符被挂断；
-EPOLLET：表示对应的文件描述符设定为edge模式；
>==epoll_data== 联合体用来保存触发事件的某个文件描述符相关的数据。例如一个client连接到服务器，服务器通过调用accept函数可以得到于这个client对应的socket文件描述符，可以把这文件描述符赋给epoll_data的fd字段以便后面的读写操作在这个文件描述符上进行。

注意：
epoll有两种模式,Edge Triggered(简称ET) 和 Level Triggered(简称LT)。在采用这两种模式时要注意的是,如果采用ET模式,那么仅当状态发生变化时才会通知,而采用LT模式类似于原来的select/poll操作,只要还有没有处理的事件就会一直通知.。
ET（Edge Triggered）与LT（Level Triggered）的主要区别可以从下面的例子看出
eg：
1． 标示管道读者的文件句柄注册到epoll中；
2． 管道写者向管道中写入2KB的数据；
3． 调用epoll_wait可以获得管道读者为已就绪的文件句柄；
4． 管道读者读取1KB的数据
5． 一次epoll_wait调用完成
如果是ET模式，管道中剩余的1KB被挂起，再次调用epoll_wait，得不到管道读者的文件句柄，除非有新的数据写入管道。如果是LT模式，只要管道中有数据可读，每次调用epoll_wait都会触发。
另一点区别就是设为ET模式的文件句柄必须是非阻塞的。
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


后续还会继续添加epoll的深层原理~~