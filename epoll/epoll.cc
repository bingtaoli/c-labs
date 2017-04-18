/*
 * 编译：c++ -o epoll epoll.cc
 * 运行： ./epoll
 * 测试：curl -v localhost
 */
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <signal.h>
using namespace std;


bool output_log = false;

#define exit_if(r, ...) if(r) {printf(__VA_ARGS__); printf("%s:%d error no: %d error msg %s\n", __FILE__, __LINE__, errno, strerror(errno)); exit(1);}

void setNonBlock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    exit_if(flags<0, "fcntl failed");
    int r = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    exit_if(r<0, "fcntl failed");
}

void updateEvents(int efd, int fd, int events, int op) {
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = events;
    ev.data.fd = fd;
    int r = epoll_ctl(efd, op, fd, &ev);
    exit_if(r, "epoll_ctl failed");
}

void handleAccept(int efd, int fd) {
    struct sockaddr_in raddr;
    socklen_t rsz = sizeof(raddr);
    int cfd = accept(fd,(struct sockaddr *)&raddr,&rsz);
    exit_if(cfd<0, "accept failed");
    sockaddr_in peer;
    socklen_t alen = sizeof(peer);
    int r = getpeername(cfd, (sockaddr*)&peer, &alen);
    exit_if(r<0, "getpeername failed");
    printf("accept new connection from %s\n", inet_ntoa(raddr.sin_addr));
    setNonBlock(cfd);
    updateEvents(efd, cfd, EPOLLIN, EPOLL_CTL_ADD);
}

struct Con {
    string readed;
    size_t written;
    bool writeEnabled;
    Con(): written(0), writeEnabled(false) {}
};

map<int, Con> cons;

char httpRes[8192];
void sendRes(int efd, int fd) {
    Con& con = cons[fd];
    size_t left = strlen(httpRes) - con.written;
    printf("httpRes length is: %zu\n", strlen(httpRes));
    int wd = 0;
    while (left > 0){
        if ((wd = ::write(fd, httpRes + con.written, left)) <= 0){
            if (errno == EINTR)  /* interrupted by sig handler return */
                wd = 0;          /* and call write() again */
            else {
                printf("write error, exit sendRes\n");
                return; 
            }
        }
        con.written += wd;
        left -= wd;
    }
    return;
}

void handleRead(int efd, int fd) {
    char buf[4096];
    int n = 0;
    while ((n=::read(fd, buf, sizeof buf)) > 0) {
        string& readed = cons[fd].readed;
        readed.append(buf, n);
        if (readed.length()>4) {
            if (readed.substr(readed.length()-2, 2) == "\n\n" || readed.substr(readed.length()-4, 4) == "\r\n\r\n") {
                //当读取到一个完整的http请求，测试发送响应
                sendRes(efd, fd);
            }
        }
    }
    if (n<0 && (errno == EAGAIN || errno == EWOULDBLOCK))
        return;
    //实际应用中，n<0应当检查各类错误，如EINTR
    if (n < 0) {
        printf("fd: %d read error, errno: %d %s\n", fd, errno, strerror(errno));
    }
    printf("ready to close socket file\n");
    close(fd);
    cons.erase(fd);
}

void loop_once(int efd, int lfd, int waitms) {
    const int kMaxEvents = 20;
    struct epoll_event activeEvs[100];
    int n = epoll_wait(efd, activeEvs, kMaxEvents, waitms);
    for (int i = 0; i < n; i ++) {
        int fd = activeEvs[i].data.fd;
        int events = activeEvs[i].events;
        if (events & (EPOLLIN | EPOLLERR)) {
            if (fd == lfd) {
                handleAccept(efd, fd);
            } else {
                handleRead(efd, fd);
            }
        } else {
            exit_if(1, "unknown event");
        }
    }
}

int main(int argc, const char* argv[]) {
    if (argc > 1) { output_log = false; }
    // 按照SIG_IGN来处理SIGPIPE，可以防止write的报错
    ::signal(SIGPIPE, SIG_IGN);
    sprintf(httpRes, "HTTP/1.1 200 OK\r\n");
    sprintf(httpRes, "%sConnection: Keep-Alive\r\nContent-Type: text/html; charset=UTF-8\r\n", httpRes);
    char body[100] = "hello, world\n";
    sprintf(httpRes, "%sContent-Length: %zu\r\n\r\n%s", httpRes, strlen(body), body);
    short port = 80;
    int epollfd = epoll_create(1);
    exit_if(epollfd < 0, "epoll_create failed");
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    exit_if(listenfd < 0, "socket failed");
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    int r = ::bind(listenfd,(struct sockaddr *)&addr, sizeof(struct sockaddr));
    exit_if(r, "bind to 0.0.0.0:%d failed %d %s", port, errno, strerror(errno));
    r = listen(listenfd, 20);
    exit_if(r, "listen failed %d %s", errno, strerror(errno));
    printf("fd %d listening at %d\n", listenfd, port);
    setNonBlock(listenfd);
    updateEvents(epollfd, listenfd, EPOLLIN, EPOLL_CTL_ADD);
    for (;;) { //实际应用应当注册信号处理函数，退出时清理资源
        loop_once(epollfd, listenfd, 10000);
    }
    return 0;
}
