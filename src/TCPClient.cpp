#include "TCPClient.h"

TCPClient::TCPClient()
{

}

TCPClient::~TCPClient()
{

}

bool TCPClient::Init(std::string path, int port)
{
    // 创建socket
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cout << "Failed to create socket!" << std::endl;
        return false;
    }

    // 连接
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = port;
    serveraddr.sin_addr.s_addr = inet_addr(path.c_str());
    int ret = connect(sockfd,
                  (struct sockaddr *)&serveraddr, //是一个地址
                  sizeof(struct sockaddr));
    if (ret < 0)
    {
        //connect error
        std::cout << "Failed to connect !" << std::endl;
        return false;
    }
    return true;
}

void TCPClient::RecvData()
{
    int ret = recv(sockfd, RecvBuff, 1024, 0);
    if(ret < 0)
    {
        std::cout << "Receive Failed" << std::endl;
        return;
    }
    else
    {
        RecvBuff[ret] = '\0';
        std::cout << "recv:" << RecvBuff << std::endl;
    }
}