在网络编程中客户端和服务端所需要的函数如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200313224013540.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200313224045822.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
# 1.创建socket
```cpp
#include<sys/types.h>
#include<sys/socket.h>
int socket(int domain, int type, int protocol);
```
>这个函数建立一个协议族为domain、协议类型为type、协议编号为protocol的套接字文件描述符。如果函数调用成功，会返回一个标识这个套接字的文件描述符，失败的时候返回-1。

## domain
>函数socket()的参数domain用于设置网络通信的域，函数socket()根据这个参数选择通信协议的族。通信协议族在文件sys/socket.h中定义。

如下，domain的值及含义

![在这里插入图片描述](https://img-blog.csdnimg.cn/2020031322515798.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
## type
>函数socket()的参数type用于设置套接字通信的类型，主要有SOCKET_STREAM（流式套接字）、SOCK——DGRAM（数据包套接字）等。

如下type的值及含义

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200313225240953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
并不是所有的协议族都实现了这些协议类型，例如，AF_INET协议族就没有实现SOCK_SEQPACKET协议类型。
## protocol
>函数socket()的第3个参数protocol用于制定某个协议的特定类型，即type类型中的某个类型。通常某协议中只有一种特定类型，这样protocol参数仅能设置为0；但是有些协议有多种特定的类型，就需要设置这个参数来选择特定的类型。

类型为SOCK_STREAM的套接字表示一个双向的字节流，与管道类似。流式的套接字在进行数据收发之前必须已经连接，连接使用connect()函数进行。一旦连接，可以使用read()或者write()函数进行数据的传输。流式通信方式保证数据不会丢失或者重复接收，当数据在一段时间内任然没有接受完毕，可以将这个连接人为已经死掉。
SOCK_DGRAM和SOCK_RAW 这个两种套接字可以使用函数sendto()来发送数据，使用recvfrom()函数接受数据，recvfrom()接受来自制定IP地址的发送方的数据。
SOCK_PACKET是一种专用的数据包，它直接从设备驱动接受数据。
## errno
>函数socket()并不总是执行成功，有可能会出现错误，错误的产生有多种原因，可以通过errno获得：

如下 errno的值及含义

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200313225648981.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
示例,建立一个流式套接字：

```cpp
int sock = socket(AF_INET, SOCK_STREAM, 0);
```
# 2.命名socket
socket() 函数用来创建套接字，确定套接字的各种属性，然后服务器端要用 bind() 函数将套接字与特定的IP地址和端口绑定起来，只有这样，流经该IP地址和端口的数据才能交给套接字处理；而客户端要用 connect() 函数建立连接。
## sockaddr_in
这个结构体用来处理网络通信的地址。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200314212011850.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
## bind
```cpp
int bind(int sock, struct sockaddr *addr, socklen_t addrlen);  
```
>-sock:文件描述符
>-addr:sockaddr结构体变量的指针
>-addrlen:addr变量的大小，可用sizeof()计算

返回值：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200314213141667.png)
代码演示：
将创建的套接字与IP地址127.0.0.1、端口8888绑定。

```cpp
//创建套接字
int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
//创建sockaddr_in结构体变量
struct sockaddr_in serv_addr;
memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
serv_addr.sin_family = AF_INET;  //使用IPv4地址
serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
serv_addr.sin_port = htons(8888);  //端口
//将套接字和IP、端口绑定
bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
```
>-inet_addr 将字符串形式的IP地址 -> 网络字节顺序  的整型值
-inet_ntoa 网络字节顺序的整型值 ->字符串形式的IP地址
-网络字节顺序与本地字节顺序之间的转换函数：
    htonl()--"Host to Network Long"
    ntohl()--"Network to Host Long"
    htons()--"Host to Network Short"
    ntohs()--"Network to Host Short"   
## connect() 
```cpp
int connect(int sock, struct sockaddr *serv_addr, socklen_t addrlen); 
```
# 3.监听socket
socket被命名之后还不能马上接受客户的连接，需要创建一个监听队列存放待处理的客户连接。

```cpp
#include <sys/socket.h>
int listen(int sockfd, int backlog);
```
>-sockfd:指定被监听的socket
>-backlog:提示内核监听队列的最大长度。监听队列的长度如果超过backlog，服务器将不受理新的客户端连接，客户端也将收到ECONNREFUSED错误信息。
>返回值：成功返回0，失败返回-1

==请求队列==：当套接字正在处理客户端请求时，如果有新的请求进来，套接字是没法处理的，只能把它放进缓冲区，待当前请求处理完毕后，再从缓冲区中读取出来处理。如果不断有新的请求进来，它们就按照先后顺序在缓冲区中排队，直到缓冲区满。这个缓冲区，就称为请求队列（Request Queue）。

# 4.接收连接

```cpp
#include <sys/types.h>
#include <sys/socket.h>
int accept(int sockfd,struct sockaddr *addr, socklen *addrlen);
```
>-sockfd:执行过listen系统调用的监听socket
>-addr:获取被接受连接的远端socket地址
>-返回值：失败返回-1
>
accpet()成功时返回一个新的连接socket，该socket唯一的表示了被接受的这个连接，服务器可通过读写该socket来与被接受连接对应的客户端通信。
# 5.发起连接（暂时了解）
客户端通过connect调用主动的与服务器建立连接。

```cpp
#include <sys/types.h>
#include <sys/socket.h>
int connect(int sockfd,const struct sockaddr *serv_addr,socklen_t addrlen);
```

![在这里插入图片描述](https://img-blog.csdnimg.cn/20200314220451399.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
# 6.关闭连接
## close()
```cpp
#include <unistd.h>
int close(int fd);
```
>-fd:待关闭的socket的描述符

将fd的引用计数减一只有当引用计数为0的时候才真正的关闭连接。一次fock将是的父进程当中打开的socket引用计数加一。因此必须都关闭才会真正的将连接关闭。
## showdown()
调用立即终止连接。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200314221027230.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)

## 7.TCP数据读写

```cpp
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recv(int sockfd,void *buf,size_t len,int flags);
ssize_t send(int sockfd,const void *buf,size_t len,int flags);
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200314221159897.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20200314221524260.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)