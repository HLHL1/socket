���������пͻ��˺ͷ��������Ҫ�ĺ�������ͼ��
![���������ͼƬ����](https://img-blog.csdnimg.cn/20200313224013540.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
![���������ͼƬ����](https://img-blog.csdnimg.cn/20200313224045822.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
# 1.����socket
```cpp
#include<sys/types.h>
#include<sys/socket.h>
int socket(int domain, int type, int protocol);
```
>�����������һ��Э����Ϊdomain��Э������Ϊtype��Э����Ϊprotocol���׽����ļ�������������������óɹ����᷵��һ����ʶ����׽��ֵ��ļ���������ʧ�ܵ�ʱ�򷵻�-1��

## domain
>����socket()�Ĳ���domain������������ͨ�ŵ��򣬺���socket()�����������ѡ��ͨ��Э����塣ͨ��Э�������ļ�sys/socket.h�ж��塣

���£�domain��ֵ������

![���������ͼƬ����](https://img-blog.csdnimg.cn/2020031322515798.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
## type
>����socket()�Ĳ���type���������׽���ͨ�ŵ����ͣ���Ҫ��SOCKET_STREAM����ʽ�׽��֣���SOCK����DGRAM�����ݰ��׽��֣��ȡ�

����type��ֵ������

![���������ͼƬ����](https://img-blog.csdnimg.cn/20200313225240953.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
���������е�Э���嶼ʵ������ЩЭ�����ͣ����磬AF_INETЭ�����û��ʵ��SOCK_SEQPACKETЭ�����͡�
## protocol
>����socket()�ĵ�3������protocol�����ƶ�ĳ��Э����ض����ͣ���type�����е�ĳ�����͡�ͨ��ĳЭ����ֻ��һ���ض����ͣ�����protocol������������Ϊ0��������ЩЭ���ж����ض������ͣ�����Ҫ�������������ѡ���ض������͡�

����ΪSOCK_STREAM���׽��ֱ�ʾһ��˫����ֽ�������ܵ����ơ���ʽ���׽����ڽ��������շ�֮ǰ�����Ѿ����ӣ�����ʹ��connect()�������С�һ�����ӣ�����ʹ��read()����write()�����������ݵĴ��䡣��ʽͨ�ŷ�ʽ��֤���ݲ��ᶪʧ�����ظ����գ���������һ��ʱ������Ȼû�н�����ϣ����Խ����������Ϊ�Ѿ�������
SOCK_DGRAM��SOCK_RAW ��������׽��ֿ���ʹ�ú���sendto()���������ݣ�ʹ��recvfrom()�����������ݣ�recvfrom()���������ƶ�IP��ַ�ķ��ͷ������ݡ�
SOCK_PACKET��һ��ר�õ����ݰ�����ֱ�Ӵ��豸�����������ݡ�
## errno
>����socket()��������ִ�гɹ����п��ܻ���ִ��󣬴���Ĳ����ж���ԭ�򣬿���ͨ��errno��ã�

���� errno��ֵ������

![���������ͼƬ����](https://img-blog.csdnimg.cn/20200313225648981.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
ʾ��,����һ����ʽ�׽��֣�

```cpp
int sock = socket(AF_INET, SOCK_STREAM, 0);
```
# 2.����socket
socket() �������������׽��֣�ȷ���׽��ֵĸ������ԣ�Ȼ���������Ҫ�� bind() �������׽������ض���IP��ַ�Ͷ˿ڰ�������ֻ��������������IP��ַ�Ͷ˿ڵ����ݲ��ܽ����׽��ִ������ͻ���Ҫ�� connect() �����������ӡ�
## sockaddr_in
����ṹ��������������ͨ�ŵĵ�ַ��
![���������ͼƬ����](https://img-blog.csdnimg.cn/20200314212011850.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
## bind
```cpp
int bind(int sock, struct sockaddr *addr, socklen_t addrlen);  
```
>-sock:�ļ�������
>-addr:sockaddr�ṹ�������ָ��
>-addrlen:addr�����Ĵ�С������sizeof()����

����ֵ��
![���������ͼƬ����](https://img-blog.csdnimg.cn/20200314213141667.png)
������ʾ��
���������׽�����IP��ַ127.0.0.1���˿�8888�󶨡�

```cpp
//�����׽���
int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
//����sockaddr_in�ṹ�����
struct sockaddr_in serv_addr;
memset(&serv_addr, 0, sizeof(serv_addr));  //ÿ���ֽڶ���0���
serv_addr.sin_family = AF_INET;  //ʹ��IPv4��ַ
serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //�����IP��ַ
serv_addr.sin_port = htons(8888);  //�˿�
//���׽��ֺ�IP���˿ڰ�
bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
```
>-inet_addr ���ַ�����ʽ��IP��ַ -> �����ֽ�˳��  ������ֵ
-inet_ntoa �����ֽ�˳�������ֵ ->�ַ�����ʽ��IP��ַ
-�����ֽ�˳���뱾���ֽ�˳��֮���ת��������
    htonl()--"Host to Network Long"
    ntohl()--"Network to Host Long"
    htons()--"Host to Network Short"
    ntohs()--"Network to Host Short"   
## connect() 
```cpp
int connect(int sock, struct sockaddr *serv_addr, socklen_t addrlen); 
```
# 3.����socket
socket������֮�󻹲������Ͻ��ܿͻ������ӣ���Ҫ����һ���������д�Ŵ�����Ŀͻ����ӡ�

```cpp
#include <sys/socket.h>
int listen(int sockfd, int backlog);
```
>-sockfd:ָ����������socket
>-backlog:��ʾ�ں˼������е���󳤶ȡ��������еĳ����������backlog�����������������µĿͻ������ӣ��ͻ���Ҳ���յ�ECONNREFUSED������Ϣ��
>����ֵ���ɹ�����0��ʧ�ܷ���-1

==�������==�����׽������ڴ���ͻ�������ʱ��������µ�����������׽�����û������ģ�ֻ�ܰ����Ž�������������ǰ��������Ϻ��ٴӻ������ж�ȡ������������������µ�������������ǾͰ����Ⱥ�˳���ڻ��������Ŷӣ�ֱ������������������������ͳ�Ϊ������У�Request Queue����

# 4.��������

```cpp
#include <sys/types.h>
#include <sys/socket.h>
int accept(int sockfd,struct sockaddr *addr, socklen *addrlen);
```
>-sockfd:ִ�й�listenϵͳ���õļ���socket
>-addr:��ȡ���������ӵ�Զ��socket��ַ
>-����ֵ��ʧ�ܷ���-1
>
accpet()�ɹ�ʱ����һ���µ�����socket����socketΨһ�ı�ʾ�˱����ܵ�������ӣ���������ͨ����д��socket���뱻�������Ӷ�Ӧ�Ŀͻ���ͨ�š�
# 5.�������ӣ���ʱ�˽⣩
�ͻ���ͨ��connect������������������������ӡ�

```cpp
#include <sys/types.h>
#include <sys/socket.h>
int connect(int sockfd,const struct sockaddr *serv_addr,socklen_t addrlen);
```

![���������ͼƬ����](https://img-blog.csdnimg.cn/20200314220451399.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)
# 6.�ر�����
## close()
```cpp
#include <unistd.h>
int close(int fd);
```
>-fd:���رյ�socket��������

��fd�����ü�����һֻ�е����ü���Ϊ0��ʱ��������Ĺر����ӡ�һ��fock���ǵĸ����̵��д򿪵�socket���ü�����һ����˱��붼�رղŻ������Ľ����ӹرա�
## showdown()
����������ֹ���ӡ�
![���������ͼƬ����](https://img-blog.csdnimg.cn/20200314221027230.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)

## 7.TCP���ݶ�д

```cpp
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recv(int sockfd,void *buf,size_t len,int flags);
ssize_t send(int sockfd,const void *buf,size_t len,int flags);
```
![���������ͼƬ����](https://img-blog.csdnimg.cn/20200314221159897.png)
![���������ͼƬ����](https://img-blog.csdnimg.cn/20200314221524260.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0hMX0hMSEw=,size_16,color_FFFFFF,t_70)