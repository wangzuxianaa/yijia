#include "TCPClient.h"

TCPClient::TCPClient()
{

}

TCPClient::~TCPClient()
{

}

bool TCPClient::Init(const char* path, int port)
{
    // 创建socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cout << "Failed to create socket!" << std::endl;
        return false;
    }

    // 连接
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(path);
    int ret = connect(sockfd,
                  (struct sockaddr *)&serveraddr, //是一个地址
                  sizeof(serveraddr));
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
    std::string str;
    int index = 0;
    if(ret < 0)
    {
        std::cout << "Receive Failed" << std::endl;
        return;
    }
    else
    {
        std::cout << RecvBuff << std::endl;
        for(int i = 0; i < ret - 2; i++) {
            if(RecvBuff[i] == ' ' || i == ret - 3) {
                if(i == ret - 3)
                {
                    str += RecvBuff[i];
                }
                int str_int = std::stoi(str);
                switch (index)
                {
                case 0:
                    flag = str_int;
                    break;
                case 1:
                    RelativePosX = str_int;
                    break;
                case 2:
                    RelativePosY = str_int;
                    break;
                case 3:
                    RelativePosZ = str_int;
                    break;
                default:
                    break;
                }
                index++;
                str.clear();
                continue;
            }
            str += RecvBuff[i];
        }
    }
}


int main(int argc, char** argv)
{
    TCPClient tcp;
    if(!tcp.Init("192.168.3.19", 8090))
        return -1;
    while(true)
    {
        tcp.RecvData();

        printf("Flag : %d, x : %d, y : %d, z : %d\n", tcp.GetFlag(), tcp.GetRelaPosX(), tcp.GetRelaPosY(), tcp.GetRelaPosZ());
    }
    tcp.CloseSocket();
}