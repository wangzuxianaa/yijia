#include<sys/select.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <json/json.h>

int main(){
    Json::Value val;
    Json::StyledWriter writer;

    val["Linear"] = 5.4;
    val["Angular"] = 3.8;

    val["Button_Stop"] = 0;
    for(int i = 0; i < 3; i++)
        val["Final_Pose"].append(0);
    int sockfd;
    int port_in  = 12321;
    int port_out = 7070;
    // 创建socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(-1==sockfd){
        puts("Failed to create socket");
        return 0;
    }

    // 设置地址与端口
    struct sockaddr_in addr;
    socklen_t  addr_len=sizeof(addr);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;       // Use IPV4
    addr.sin_port   = htons(port_in);    
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    // 绑定获取数据的端口，作为发送方，不绑定也行
    if (bind(sockfd, (struct sockaddr*)&addr, addr_len) == -1){
        printf("Failed to bind socket on port %d\n", port_in);
        close(sockfd);
        return false;
    }
    char buf[1024];
    int counter = 0;
    while(1) {
        addr.sin_family = AF_INET;
        addr.sin_port   = htons(port_out);
        addr.sin_addr.s_addr = inet_addr("192.168.2.6");
        std::string SendBuf = writer.write(val);
        std::cout << val.toStyledString() << std::endl;
        sendto(sockfd, SendBuf.c_str(), SendBuf.size(), 0, (sockaddr*)&addr, addr_len);
        printf("Sended %d\n", ++counter);
        struct sockaddr_in tmp;
        socklen_t len = sizeof(tmp);
        int ret = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr*)&tmp, &len);
        if(ret > 0) {
            Json::Value val;
            Json::Reader reader;

            if(reader.parse(buf, val)) {
                std::cout << val.toStyledString() << std::endl;
            }
        }
    }

    close(sockfd);
    return 0;
}