#pragma once
class elevator_new
{
public:
	elevator_new(int n,int k=0);
	~elevator_new();
	int ask_floor();
	int ask_for_state();
	int update_state(int floor);//到达某一层标记
	int downdate_state(int floor);//清除到达某一层标记
	void run();
private:
	void get(int people);
	void out(int people);
	void MoveUp();
	void MoveDowm();
	int aim[50];//电梯运行的目的标记楼层
	int state;//电梯运行状态 1空载2有客
	int people_num;//电梯内乘客数量
	int elevator_place;//电梯当前位置
	int timer;//电梯当前时间
};

