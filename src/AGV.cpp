#include "AGV.h"
#include <cpr/cpr.h>
#include <iostream>

AGV::AGV()
{

}

AGV::~AGV()
{

}

void AGV::AddTask(std::string start, std::string goal)
{
    Json::Value val;
    Json::StyledWriter writer;
    // 传递的Json数据
    val["start"] = start;
    val["goal"] = goal;
    val["start_action"] = 0;
    val["goal_action"] = 0;
    std::cout << val.toStyledString() << std::endl;
    Json::String RequestParam = writer.write(val);
    // 发起Post请求
    cpr::Response response = cpr::Post(cpr::Url{"http://192.168.3.220:9999/api/v1/task/add_task"},
                                        cpr::Header{{"Content-Type", "application/json"}},
                                        cpr::Body(RequestParam));
    std::cout << response.status_code << std::endl;
    // 输出回应文本
    std::cout << response.text << std::endl;
}

void AGV::GetStatus()
{
    Json::Reader reader;
    Json::Value val;
    cpr::Response response = cpr::Get(cpr::Url{"http://192.168.3.220:9999/api/v1/monitor/get_all_status"});
    std::cout << response.status_code << std::endl;
    // 输出回应文本
    reader.parse(response.text, val);
    std::cout << val.toStyledString() << std::endl;
}

int main()
{
    AGV AGVInstance;

    AGVInstance.GetStatus();

    // AGVInstance.AddTask("A2", "A2");


}