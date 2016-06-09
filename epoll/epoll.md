
### 数据结构

```cpp
typedef union epoll_data {
   void        *ptr;
   int          fd;
   uint32_t     u32;
   uint64_t     u64;
} epoll_data_t;

struct epoll_event {
   uint32_t     events;      /* Epoll events */
   epoll_data_t data;        /* User data variable */
};
```

其中events代表可读可写事件，data代表要监听的数据，一般是socket fd。

### events可以是以下几个宏的集合

```
EPOLLIN：触发该事件，表示对应的文件描述符上有可读数据。(包括对端SOCKET正常关闭)；
EPOLLOUT：触发该事件，表示对应的文件描述符上可以写数据；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里。
```

### epoll\_wait

等待事件的产生，类似于select()调用。参数events用来从内核得到事件的集合，maxevents告之内核这个events有多大(数组成员的个数)，**这个maxevents的值不能大于创建epoll\_create()时的size**，参数timeout是超时时间（毫秒，0会立即返回，-1将不确定，也有说法说是永久阻塞）。

该函数返回需要处理的事件数目，如返回0表示已超时。

返回的事件集合在events数组中，数组中实际存放的成员个数是函数的返回值。返回0表示已经超时。

### 通用epoll框架

http://jimmee.iteye.com/blog/2215622

```cpp
for( ; ; )  
{  
    nfds = epoll_wait(epfd,events,20,500);  
    for(i=0;i<nfds;++i)  
    {  
        if(events[i].data.fd==listenfd) //有新的连接  
        {  
            connfd = accept(listenfd,(sockaddr *)&clientaddr, &clilen); //accept这个连接  
            ev.data.fd=connfd;  
            ev.events=EPOLLIN|EPOLLET;  
            epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev); //将新的fd添加到epoll的监听队列中  
        }  
        else if( events[i].events&EPOLLIN ) //接收到数据，读socket  
        {  
            n = read(sockfd, line, MAXLINE)) < 0    //读  
            ev.data.ptr = md;     //md为自定义类型，添加数据  
            ev.events=EPOLLOUT|EPOLLET;  
            epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev);//修改标识符，等待下一个循环时发送数据，异步处理的精髓  
        }  
        else if(events[i].events&EPOLLOUT) //有数据待发送，写socket  
        {  
            struct myepoll_data* md = (myepoll_data*)events[i].data.ptr;    //取数据  
            sockfd = md->fd;  
            send( sockfd, md->ptr, strlen((char*)md->ptr), 0 );        //发送数据  
            ev.data.fd=sockfd;  
            ev.events=EPOLLIN|EPOLLET;  
            epoll_ctl(epfd,EPOLL_CTL_MOD,sockfd,&ev); //修改标识符，等待下一个循环时接收数据  
        }  
        else  
        {  
            //其他的处理  
        }  
    }  
}  
```
