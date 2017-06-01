#include <stdio.h>
#include <string.h>
#if defined(WIN32)
    #include <windows.h>
#elif __linux__
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/ioctl.h>
#endif

int set_nonblock(int fd) {
    int flags;
#if defined(O_NONBLOCK)
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    return ioctl(fd,FIONBIO, &flags);
#endif
}

int TCP_Server_init(unsigned short port, char* host)
{
#if defined(WIN32)
    WSADATA WsaData;
    int err = WSAStartup (0x0303, &WsaData);
    if (err == SOCKET_ERROR) {
        printf ("WSAStartup() failed: %ld\n", GetLastError ());
        return 1;
    }
#endif
    struct sockaddr_in sa;
    memset(&sa,0,sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);

    #if defined(WIN32)
        sa.sin_addr.s_addr = inet_addr(host);
    #elif __linux__
        inet_pton(AF_INET, host, &sa.sin_addr);
    #endif

    int fd,r;
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1){
        perror("socket");
    }
    r = bind(fd,(struct sockaddr*)&sa,sizeof(sa));
    if(r == -1){
        perror("bind");
    }
    return fd;
}

int TCP_Client_init(unsigned short port, char* host) {
#if defined(WIN32)
    WSADATA WsaData;
    int err = WSAStartup (0x0303, &WsaData);
    if (err == SOCKET_ERROR) {
        printf ("WSAStartup() failed: %ld\n", GetLastError ());
        return 1;
    }
#endif
    struct sockaddr_in sa;
    memset(&sa,0,sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);

    #if defined(WIN32)
        sa.sin_addr.s_addr = inet_addr(host);
    #elif __linux__
        inet_pton(AF_INET, host, &sa.sin_addr);
    #endif

    int fd,r;
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1){
        perror("socket");
    }
    r = connect(fd,(struct sockaddr*)&sa,sizeof(sa));
    if(r == -1){
        perror("connect");
    }
    return fd;
}

int Socket_accept(int fd){
    int r;
    r = listen(fd,2);
    if(r == -1){
        perror("listen");
        close(fd);
    }
    printf("listen %d\n",r);
    fd = accept(fd,0,0);
    if(fd == -1){
        perror("accept");
        close(fd);
    }
    printf("accept %d\n",r);
    return fd;
}

int Socket_recv(int fd, void *data, int len)
{
    char* buf=(char*)data;
    int r;

    while(len)
    {
        r=recv(fd,buf,len,0);
        if(r==0 ||r==-1)
            return -1;

        len-=r;
        buf+=r;
    }

    return 0;
}
int Socket_send(int fd, const void *data, int len)
{
    const char* buf=(char*)data;
    int r;

    while(len)
    {
        r=send(fd,buf,len,MSG_NOSIGNAL);
        if(r==0 ||r==-1)
            return -1;

        len-=r;
        buf+=r;
    }

    return 0;
}
