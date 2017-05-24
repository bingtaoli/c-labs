## socket服务端

socket服务端的创建非常基础，虽然日常使用不多。

由于结构体名称很难记，宏名称很难记，往往手写不切实际，所以需要记录下来。

```cpp
// SOCK_STREAM: 提供双向连续且可信赖的数据流, 即TCP.
// AF_INET: Ipv4 网络协议
int listenfd = socket(AF_INET, SOCK_STREAM, 0);
exitif(listenfd <0, "socket failed");
struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
// htons: 转为网络序，网络序是大端字节存储
addr.sin_port = htons(port);
addr.sin_addr.s_addr = INADDR_ANY;
bind(listenfd, (struct sockaddr *)&addr, sizeof(struct sockadd));
listen(listenfd, 20);
```

经过上面，一个监听port端口的服务就起来了，目前是阻塞的，可以while循环一直等待accept。提下`htons`函数，TCP规定网络序是大端存储，即高位地址存储高位数据。

## epoll

在`handy`项目中的epoll主要步骤如下:

```cpp
// raw_examples
int epollfd = epoll_create(1);
exitif(epollfd<0, "epoll create failed");
int listenfd = socket(AF_INET, SOCK_STREAM, 0);
exitif(listenfd <0, "socket failed");
struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_port = htons(port);
addr.sin_addr.s_addr = INADDR_ANY;
bind(listenfd, (struct sockaddr *)&addr, sizeof(struct sockadd));
listen(listenfd, 20);
setNonBlock(listenfd);
//add listenfd to epoll
struct epoll_event ev;
memset(&ev, 0, sizeof(ev));
ev.events =EPOLLIN;
ev.data.fd = fd;
epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);
//loop
for (;;){
    const int kMaxEvents = 20;
    struct epoll_events activeEvs[100];
    int n = epoll_wait(efd, activeEvs, kMaxEvents, 10000); //10000代表ms
    if (int i = 0; i < n; i++){
        int fd = activeEvs[i].data.fd;
        int events = activeEvs[i].events;
        if (events & (EPOLLIN | EPOLLERR)){ //可读
            //什么时候fd会等于listenfd呢？见epoll目录中的文档
            if (fd == listenfd){ 
                handleAccept(efd, fd);
            } else {
                handleRead(efd, fd);
            }
        } else if (events & EPOLLOUT){
            handleWrite(efd, fd); //可写
        } else {
            exif(1, "unknow event");
        }
    }
}
```

