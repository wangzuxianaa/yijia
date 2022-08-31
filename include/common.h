#ifndef __COMMON_H__
#define __COMMON_H__


// 线程数
const int Num_Thread = 3;

struct PositionVector{
    double x, y, z;
};

// 任务点
struct TaskPoint {
	double x, y, z, angle;
	int  f1, f2, mode;
};

// 指令点
struct CommandPoint {
	int facade; // 立面
	int layer; // 层数
	int location; // 库位
};

// 地图点
struct MapPoint {
	double x, y, z, angle;
};

// 设置底盘数据
struct BottData {
	bool PowerSwitch;
	bool ChargerSwitch;
	bool LockMotor;
	float Linear;
	float Angular;
	bool DisinCmd;
	bool ResetOdom;
	bool ResetImu;
};

struct AGVPose
{
	float x;
	float y;
	float yaw;
};

// 底盘信息回传
struct AGVInfo
{
	int Vx;
	float Vz;
	float AccX;
	float AccY;
	float AccZ;
	float GyrX;
	float GyrY;
	float GyrZ;
	int16_t Voltage;
	int16_t State;
	int16_t Light12;
	int16_t Light34;
};

struct RequestInfo
{
	bool PowerState;
	bool Button1State;
	bool ButtonStop;
	bool ChargeState;
	bool MotorLockState;
	int Position;
	int Rotation;
	int Twist;
	int AngularVel;
	int LinearAcc;
	int PerCent;
	int Voltage;
	int Current;
};



#endif