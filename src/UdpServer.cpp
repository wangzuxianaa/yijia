#include "UdpServer.h"

UdpServer::UdpServer()
{
    sockfd = 0;
    memset(RecvBuff, 0, 1024);
}

UdpServer::~UdpServer()
{

}

bool UdpServer::InitSocket()
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd == -1) {
        return false;
        printf("Failed to create socket\n");
    }
    printf("InitSocket Successfully\n");
    return true;
}

bool UdpServer::BindAddrAndPort(int port) 
{
    // 配置地址ip和端口号
    struct sockaddr_in addr;

    socklen_t addr_len = sizeof(addr);

    memset(&addr, 0, sizeof(addr));
    // 协议簇-ipv4
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 设置端口号
    addr.sin_port = htons(port);

    // 使用本地端口给套接字命名
    if(bind(sockfd, (struct sockaddr*) &addr, addr_len) == -1) {
        printf("Failed to bind socket on port %d\n", port);
        close(sockfd);
        return false;
    }
    printf("Succeed to bind socket on port %d\n", port);
    return true;
}

void UdpServer::SetAGVInfo(AGVInfo& msg)
{
    AGV_Info.Vx = msg.Vx;
    AGV_Info.Vz = msg.Vz;
    AGV_Info.AccX = msg.AccX;
    AGV_Info.AccY = msg.AccY;
    AGV_Info.AccZ = msg.AccZ;
    AGV_Info.GyrX = msg.GyrX;
    AGV_Info.GyrY = msg.GyrY;
    AGV_Info.GyrZ = msg.GyrZ;
    AGV_Info.Voltage = msg.Voltage;
    AGV_Info.State = msg.State;
    AGV_Info.Light12 = msg.Light12;
    AGV_Info.Light34 = msg.Light34;
}

std::vector<int> UdpServer::DecToBit(int num)
{
    std::vector<int> res(16, 0);
    int i = 0;
    while(num > 0) {
        res[i] = num % 2;
        i++;
        num /= 2;
    }

    return res;
}

void UdpServer::RecvJsonData()
{
    
    socklen_t client_len = sizeof(client);

    int r = recvfrom(sockfd, RecvBuff, 4096, 0, (sockaddr*) &client, &client_len);

    if(r < 0) {
        perror("receive fail");
        return;
    }
    else {
        Json::Value val;
        Json::Value jsonwriter;
        Json::Reader reader;
        
        // 解析json数据
        if(reader.parse(RecvBuff, val)) {
            std::cout << val.toStyledString() << std::endl;

            Json::Value::Members member = val.getMemberNames();

            std::string str = member[0];

            if(str == "Linear") {
                bottdata.Linear = val["Linear"].asFloat();
            }
            else if(str == "Angular") {
                bottdata.Angular = val["Angular"].asFloat();
            }
            else if(str == "Charger_Switch") {
                bottdata.ChargerSwitch = val["Charger_Switch"].asBool();
            }

            if(str == "Final_Pose") {
                const Json::Value FinalPose = val["Final_Pose"];
                agvpose.x = FinalPose[0].asFloat();
                agvpose.y = FinalPose[1].asFloat();
                agvpose.yaw = FinalPose[2].asFloat();
            }
            
            if(str == "Power_State") {
                std::cout << "eeeeeeeeeeeeee" << std::endl;
                if(val["Power_State"].asInt() == 0) {
                    jsonwriter["Power_State"] = true;
                    SendJsonData(jsonwriter);
                } 
            }
            else if(str == "Button_Stop") {
                std::cout << "hhhhh" << std::endl;
                if(val["Button_Stop"].asInt() == 0) {
                    jsonwriter["Button_Stop"] = false;
                    SendJsonData(jsonwriter);
                }
            }
            else if(str == "Charge_State") {
                if(val["Charge_State"].asInt() == 0) {
                    jsonwriter["Charge_State"] = false;
                    SendJsonData(jsonwriter);
                }
            }
            else if(str == "Motor_Lock_State") {
                if(val["Motor_Lock_State"].asInt() == 0) {
                    jsonwriter["Motor_Lock_State"] = false;
                    SendJsonData(jsonwriter);
                }
            }
            else if(str == "Position") {
                if(val["Position"].asInt() == 0) {
                    jsonwriter["Position"].append(agvpose.x);
                    jsonwriter["Position"].append(agvpose.y);
                    std::cout << jsonwriter.toStyledString() << std::endl;
                    SendJsonData(jsonwriter);
                }
            }
            else if(str == "Rotation") {
                if(val["Rotation"].asInt() == 0) {
                    jsonwriter["Rotation"] = agvpose.yaw;
                    std::cout << jsonwriter.toStyledString() << std::endl;
                    SendJsonData(jsonwriter);
                }
            }
            else if(str == "Twist") {
                if(val["Twist"].asInt() == 0) {
                    jsonwriter["Twist"].append((float)AGV_Info.Vx / 1000.0);
                    jsonwriter["Twist"].append((float)AGV_Info.Vz / 1000.0);
                    std::cout << jsonwriter.toStyledString() << std::endl;
                    SendJsonData(jsonwriter);
                }
            }
            else if(str == "Percent") {
                if(val["Percent"].asInt() == 0) {
                    jsonwriter["Percent"] = 50.0;
                    SendJsonData(jsonwriter);
                }
            }
            else if(str == "Voltage") {
                if(val["Voltage"].asInt() == 0) {
                     jsonwriter["Voltage"] = (float)AGV_Info.Voltage / 10.0;
                     std::cout << jsonwriter.toStyledString() << std::endl;
                    SendJsonData(jsonwriter);
                }
            }
            else if(str == "Current") {
                if(val["Current"].asInt() == 0) {
                    jsonwriter["Current"] = 20.0;
                    SendJsonData(jsonwriter);
                }
            }

            // // 线速度和角速度数据
            // if(val.isMember("Linear")) {
            //     bottdata.Linear = val["Linear"].asFloat();
            // }
            // else if(val.isMember("Angular")) {
            //     bottdata.Angular = val["Angular"].asFloat();
            // }
            // else if(val.isMember("Charger_Switch")) {
            //     bottdata.ChargerSwitch = val["Charger_Switch"].asBool();
            // }
            

            // // 定位数据
            // if(val.isMember("Final_Pose")) {
            //     const Json::Value FinalPose = val["Final_Pose"];
            //     agvpose.x = FinalPose[0].asFloat();
            //     agvpose.y = FinalPose[1].asFloat();
            //     agvpose.yaw = FinalPose[2].asFloat();
            // }
            
            // // 请求底盘数据
            // // std::cout << "dddddddddddddddddddd" << val.isMember("Power_State") << std::endl;
            // if(val.isMember("Button_Stop")) {
            //     // std::vector<int> state = DecToBit(AGV_Info.State);
            //     // if(state[0] == 0) {
            //     //     jsonwriter["Button_Stop"] = false;
            //     // }
            //     // else {
            //     //     jso
            //     std::cout << "hhhhh" << std::endl;
            //     jsonwriter["Button_Stop"] = false;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val.isMember("Power_State")) {
            //     std::cout << "eeeeeeeeeeeeee" << std::endl;
            //     jsonwriter["Power_State"] = true;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            
            // else if(val.isMember("Charge_State")) {
            //     jsonwriter["Charge_State"] = false;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val.isMember("Motor_Lock_State")) {
            //     jsonwriter["Motor_Lock_State"] = false;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val.isMember("Position")) {
            //     jsonwriter["Position"].append(agvpose.x);
            //     jsonwriter["Position"].append(agvpose.y);
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val.isMember("Rotation")) {
            //     jsonwriter["Rotation"] = agvpose.yaw;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val.isMember("Twist")) {
            //     jsonwriter["Twist"].append(bottdata.Linear);
            //     jsonwriter["Twist"].append(bottdata.Angular);
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val.isMember("Angular_Vel")) {
            //     jsonwriter["Angular_Vel"].append(AGV_Info.GyrX);
            //     jsonwriter["Angular_vel"].append(AGV_Info.GyrY);
            //     jsonwriter["Angular_vel"].append(AGV_Info.GyrZ);
            //     SendJsonData(jsonwriter);
            // }
            // else if(val.isMember("Linear_Acc")) {
            //     jsonwriter["Linear_Acc"].append(AGV_Info.AccX);
            //     jsonwriter["Linear_Acc"].append(AGV_Info.AccY);
            //     jsonwriter["Linear_Acc"].append(AGV_Info.AccZ);
            //     SendJsonData(jsonwriter);
            // }
            // else if(val.isMember("Percent")) {
            //     jsonwriter["Percent"] = 50.0;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val.isMember("Voltage")) {
            //     jsonwriter["Voltage"] = (float)AGV_Info.Voltage / 10.0;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val.isMember("Current")) {
            //     jsonwriter["Current"] = 20.0;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }

            // if(val["Button_Stop"].asInt() == 0) {
            //     // std::vector<int> state = DecToBit(AGV_Info.State);
            //     // if(state[0] == 0) {
            //     //     jsonwriter["Button_Stop"] = false;
            //     // }
            //     // else {
            //     //     jso
            //     std::cout << "hhhhh" << std::endl;
            //     jsonwriter["Button_Stop"] = false;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val["Power_State"].asInt() == 0) {
            //     std::cout << "eeeeeeeeeeeeee" << std::endl;
            //     jsonwriter["Power_State"] = true;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            
            // else if(val["Charge_State"].asInt() == 0) {
            //     jsonwriter["Charge_State"] = false;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val["Motor_Lock_State"].asInt() == 0) {
            //     jsonwriter["Motor_Lock_State"] = false;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val["Position"].asInt() == 0) {
            //     jsonwriter["Position"].append(agvpose.x);
            //     jsonwriter["Position"].append(agvpose.y);
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val["Rotation"].asInt() == 0) {
            //     jsonwriter["Rotation"] = agvpose.yaw;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val["Twist"].asInt() == 0) {
            //     jsonwriter["Twist"].append(bottdata.Linear);
            //     jsonwriter["Twist"].append(bottdata.Angular);
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val["Percent"].asInt() == 0) {
            //     jsonwriter["Percent"] = 50.0;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val["Voltage"].asInt() == 0) {
            //     jsonwriter["Voltage"] = (float)AGV_Info.Voltage / 10.0;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            // else if(val["Current"].asInt() == 0) {
            //     jsonwriter["Current"] = 20.0;
            //     std::cout << jsonwriter.toStyledString() << std::endl;
            //     SendJsonData(jsonwriter);
            // }
            
        }
    }
    
}

void UdpServer::SendJsonData(Json::Value& val)
{
    Json::StyledWriter writer;

    socklen_t client_len = sizeof(client);

    std::string SendBuf = writer.write(val);

    sendto(sockfd, SendBuf.c_str(), SendBuf.size(), 0, (sockaddr*) &client, client_len);

}
