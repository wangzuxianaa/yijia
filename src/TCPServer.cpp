#include <iostream>
//accept
#include <sys/socket.h>
#include <netinet/in.h>
//close
#include <unistd.h>

//使用宏，定义一个端口
#define PORT 8888

//使用宏，定义一个buffer接收消息的长度
#define MESSAGE_LEN 1024

int main(int argc, char const *argv[])
{
    //定义用于连接，和接收的socket
    int socket_fd,accept_fd;
    int ret = -1;
    int on = 1;
    //毫秒级并发10个
    int backlog = 10;

    struct sockaddr_in localaddr, remoteaddr;

    // 定义一个消息的缓冲区，是一个数组，长度为MESSAGE_LEN,初始值为0
    char in_buff[MESSAGE_LEN] = {
        0,
    };

    /// Step1: 创建Socket //
    //PF_INET         Internet version 4 protocols,
    //SOCK_STREAM TCP
    //SOCK_DGRAM UDP
    socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        std::cout << "Failed to create socket!" << std::endl;
        exit(-1);
    }

    // 为当前Socket设置选项
    // int
    // setsockopt(int socket, int level, int option_name,
    // const void *option_value, socklen_t option_len);
    // SOL_SOCKET 层级的level
    // on = 1 起作用
    ret = setsockopt(socket_fd,
                     SOL_SOCKET,
                     SO_REUSEADDR,
                     &on,
                     sizeof(on));

    if (ret == -1)
    {
        // 选项出错，不影响使用
        std::cout << "Failed to set socket options!" << std::endl;
    }

    /// Step2: 参数绑定地址与端口 
    // int
    // bind(int socket, const struct sockaddr *address, socklen_t address_len);

    //定义结构体
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = PORT;
    //服务端有多块网卡，把所有网卡的ip地址都侦听这个端口，此处设置为0
    localaddr.sin_addr.s_addr = INADDR_ANY;

    ret = bind(socket_fd, (struct sockaddr *)&localaddr, sizeof(struct sockaddr));
    if (ret == -1)
    {
        std::cout << "Failed to bind addr" << std::endl;
        exit(-1);
    }

    /// Step3: 侦听 
    //缓冲区不用很大：毫秒级并发10个就可，故，backlog设置为10
    ret = listen(socket_fd, backlog);
    if (ret == -1)
    {
        std::cout << "Failed to listen socket" << std::endl;
        exit(-1);
    }

     // 接收用户的请求
    // int accept(int socket,
    // struct sockaddr *restrict address, //客户端地址
    //        socklen_t *restrict address_len);
    // accept_fd：新的返回值   
    socklen_t addr_len = sizeof(struct sockaddr);
    accept_fd = accept(socket_fd,
                        (struct sockaddr *)&remoteaddr,
                        &addr_len);

    /// Step4: 收发数据 
    //会有很多，所以要一个死循环
    for (;;)
    {
        // 从 accept_fd 接收数据
        // ssize_t recv(int socket, void *buffer, size_t length, int flags);
        // size_t length：buffer 的长度
        // ret = recv(accept_fd, (void *)in_buff, MESSAGE_LEN, 0);
        // if (ret == 0)
        // {
        //     // 远端未发送数据，与我们断开连接，否则会一直发送数据
        //     break;
        // }
        // //输出客户端消息
        // std::cout << "recv:" << in_buff << std::endl;

        char sendbuf[MESSAGE_LEN];
        sprintf(sendbuf, "%d %d %d %d\n", 1, 20, 44, 20);
        //向客户端发送数据
        //与recv一样，只是函数名字变了
        //使用accept_fd，内容是in_buff，长度是：MESSAGE_LEN
        std::cout << sendbuf << std::endl;
        send(accept_fd, (void *)sendbuf, MESSAGE_LEN, 0);

        //关闭接收数据的连接
        close(accept_fd);
    }

    //关闭初始的
    close(socket_fd);

    return 0;
}