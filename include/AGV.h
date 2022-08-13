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

    void AddTask(std::string start, std::string goal);

    void GetStatus();
};

#endif