#ifndef __AGV__H__
#define __AGV__H__

#include<json/json.h>

class AGV
{
private:
    /* data */
public:
    AGV();

    ~AGV();

    // 添加任务
    void AddTask(std::string start, std::string goal);

    // 获取小车状态
    void GetStatus();
};

#endif