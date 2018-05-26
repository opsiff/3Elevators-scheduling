#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include"elevator.h"
#include"control_table.h"
using namespace std;
int floor_num = 3;
int start_floor = 0;
//时间从零开始，有n个人
int Global_time = 0, n;
control_table control1;
elevator elevator1,elevator2,elevator3;
ifstream in;
ofstream out1,out2,out3;
int file_num;
char ** file;
struct passenger {
	int req_time, get_time, req_place_fr, req_place_to;
	int req_arrive_time;
	int use, leave;
};
struct passenger passenger2[501];
void begin(int file_command_num, char *command_string[]) {
	if (file_command_num > 1) {
			in.open(command_string[1], ios::in);
			if (strcmp(command_string[1], "h") == 0 || strcmp(command_string[1], "help") == 0 || strcmp(command_string[1], "/h") == 0 || strcmp(command_string[1], "/help") == 0) {
				cout << endl;
				cout << "**************Elevator Runner Command Help***************" << endl;
				cout << "1.With no command ,it will read the input.txt and write from the current directory " << endl;
				cout << "2.With a pieces of command,eg:D:\input.txt" << endl;
				cout << "3.With 4 pieces of command,eg:D:\input.txt  D:\output1.txt D:\output2.txt D:\output3.txt" << endl;
				cout << "*********************************************************" << endl;

				exit(0);
			}
	}
	else {
		in.open("input.txt", ios::in);
	}
	if (in.is_open())
	{
		in >> n;
		for (int i = 0; i < n + 1; i++)
			passenger2[i].use = 0, passenger2[i].leave = 0;
		for (int i = 1; i <= n; i++)
			in >> passenger2[i].req_time >> passenger2[i].req_place_fr >> passenger2[i].req_place_to;
		in.close();
	}
	else {
		in.close();
		cout << "file can not be open" << endl;
		exit(0);
	}
}
//检查请求是否完成
int check_queue_use() {
	for (int i = 1; i <= n; i++) 
		if (passenger2[i].leave == 0) 
			return i;
	return 0;
}
//寻找最佳待完成请求
int find_passenger() {
	//去找当前第一个请求
	if (check_queue_use() != 0) {
		int passengermin = 100000;
		int passengerminn = -1;
		for (int i = 1; i <= n; i++) {
			if (passenger2[i].req_time<passengermin&&passenger2[i].use == 0) {
				passengermin = passenger2[i].req_time;
				passengerminn = i;
			}
		}
		return passengerminn;
	}
	else 
		return -1;
}
int find_aim_place(int floor) {//寻找目的地停靠
	int temp = 100, tmp = 0;
	for (int i = start_floor; i <= floor_num; i++) {
		if (control1.aim[i] == 1) {
			if (abs(floor - i)<temp) {
				tmp = i;
				temp = abs(floor - i);
			}
		}
	}
	return tmp;
}
int check_get() {
	int passengerT;
	passengerT = find_passenger();
	//当请求的时间比目前早，而且没有响应，而且刚好在这层
	if (passengerT>0)
		if (passenger2[passengerT].req_time <= Global_time) {
			if (passenger2[passengerT].use == 0) {
				if (passenger2[passengerT].req_place_fr == elevator1.state()) {
					//cout << passengerT << endl;
					return 1;
				}
			}
		}
	return 0;
}
int put_out() {
	bool state = 0;//是否有人下电梯
	for (int i = 1; i <= n; i++) {
		//当到了对应楼层的时候，寻找所有上了电梯可以下的乘客
		if (passenger2[i].req_place_to == elevator1.state()) {
			if (passenger2[i].use == 1 && passenger2[i].leave == 0) {
				passenger2[i].leave = 1;
				passenger2[i].req_arrive_time = Global_time;
				elevator1.out(1);
				control1.downdate_state(elevator1.state());
				state = 1;
			}
		}
	}
	if (state == 1) {
		if (file_num == 1) {
			out2.open("output2.txt", ios::app | ios::out);
		}
		if (file_num == 2) {
			out2.open("output2.txt", ios::app | ios::out);
		}
		if (file_num == 5) {
			out2.open(file[3], ios::app | ios::out);
		}
		if (out2.is_open())
		{
			out2 << Global_time << " " << elevator1.state() << "\n";
			out2.close();
		}
		else {
			out2.close();
			cout << "command error ! please use command help see some helps" << endl;
			exit(0);
		}
		return 1;
	}
	else {
		return 0;
	}
}
//核心调度部分
void run() {
	int person,/*person_number=1,*/floor_stop = 0;
	person = find_passenger();//找第一个人
	if (person != -1) {
		//请求时间未到不处理
		if (Global_time<passenger2[find_passenger()].req_time) {
			bool k = 0;
			for (int i = 1; i <= n; i++) {
				if (passenger2[i].use == 1 && passenger2[i].leave == 0) 
					k = 1;
			}
			if (k == 0) 
				Global_time = passenger2[find_passenger()].req_time;
		}
		else {
			control1.update_state(passenger2[person].req_place_fr);
		}
	}
	//移动中途+中途接客/下客
	while (elevator1.state() != find_aim_place(elevator1.state())) {
		//中途下人
		floor_stop = put_out();
		while (check_get()) {//移动到中途接人
			floor_stop = 1;//停下时间
			int temp_passenger = find_passenger();//防止重复调用
			passenger2[temp_passenger].use = 1;//已经响应
			passenger2[temp_passenger].get_time = Global_time + 1;//上梯时间
			control1.update_state(passenger2[temp_passenger].req_place_to);//按下目的楼层按钮
			elevator1.get(1);//电梯加人
		}
		control1.downdate_state(elevator1.state()); //清理该楼请求
		if (elevator1.state()<find_aim_place(elevator1.state())) {
			elevator1.MoveUp();
			Global_time = Global_time + floor_stop + 1;
			floor_stop = 0;
		}
		else {
			elevator1.MoveDowm();
			Global_time = Global_time + floor_stop + 1;
			floor_stop = 0;
		}
	}
	//下人
	floor_stop = put_out();
	while (check_get()) {//接人
		floor_stop = 1;//停下时间
		int temp_passenger = find_passenger();//防止重复调用
		passenger2[temp_passenger].use = 1;//已经响应
		passenger2[temp_passenger].get_time = Global_time + 1;//上梯时间
		control1.update_state(passenger2[temp_passenger].req_place_to);//按下目的楼层按钮
		elevator1.get(1);//电梯加人
	}
	Global_time = Global_time + floor_stop;
	control1.downdate_state(elevator1.state()); //清理该楼请求
}
int main(int file_command_num, char *command_string[]) {
	file = command_string;
	file_num = file_command_num;
	begin(file_command_num,command_string);
	//int ff = 0;
	//如果还有人的请求没被满足，电梯就继续开着
	if (file_command_num == 1) {
	out1.open("output1.txt", ios::app | ios::out);
	out3.open("output3.txt", ios::app | ios::out);
	}
	if (file_command_num == 2) {
		out1.open("output1.txt", ios::app | ios::out);
		out3.open("output3.txt", ios::app | ios::out);
		in.close();
	}
	if (file_command_num == 5) {
		out1.open(command_string[2], ios::app | ios::out);
		out3.open(command_string[4], ios::app | ios::out);
	}
	if (file_command_num != 1 && file_command_num != 2 && file_command_num != 5) {
		cout << "command error" << endl;
		return 0;
	}
	while (check_queue_use() != 0) 
		run();
	out1.close();
	out3.close();
	return 0;
}