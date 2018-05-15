#include<iostream>
#include<fstream>
#include<cstdio>
#include"elevator.h"
#include"control_table.h"
using namespace std;
int floor_num = 3;
int start_floor = 0;
//ʱ����㿪ʼ����n����
int Global_time = 0, n;
control_table control1;
elevator elevator1,elevator2,elevator3;
ofstream out1;
ofstream out2;
ofstream out3;
struct passenger {
	int req_time, get_time, req_place_fr, req_place_to;
	int req_arrive_time;
	int use, leave;
};
struct passenger passengerL[501];
void begin() {
	ifstream in("input.txt");
	in >> n;
	for (int i = 0; i < n + 1; i++) 
		passengerL[i].use = 0,passengerL[i].leave = 0;
	for (int i = 1; i <= n; i++) 
		in >> passengerL[i].req_time >> passengerL[i].req_place_fr >> passengerL[i].req_place_to;
}
//��������Ƿ����
int check_queue_use() {
	for (int i = 1; i <= n; i++) 
		if (passengerL[i].leave == 0) 
			return i;
	return 0;
}
//Ѱ����Ѵ��������
int find_passenger() {
	//ȥ�ҵ�ǰ��һ������
	if (check_queue_use() != 0) {
		int passengermin = 100000;
		int passengerminn = -1;
		for (int i = 1; i <= n; i++) {
			if (passengerL[i].req_time<passengermin&&passengerL[i].use == 0) {
				passengermin = passengerL[i].req_time;
				passengerminn = i;
			}
		}
		return passengerminn;
	}
	else 
		return -1;
}
int find_aim_place(int floor) {//Ѱ��Ŀ�ĵ�ͣ��
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
	//�������ʱ���Ŀǰ�磬����û����Ӧ�����Ҹպ������
	if (passengerT>0)
		if (passengerL[passengerT].req_time <= Global_time) {
			if (passengerL[passengerT].use == 0) {
				if (passengerL[passengerT].req_place_fr == elevator1.state()) {
					//cout << passengerT << endl;
					return 1;
				}
			}
		}
	return 0;
}
int put_out() {
	bool state = 0;//�Ƿ������µ���
	for (int i = 1; i <= n; i++) {
		//�����˶�Ӧ¥���ʱ��Ѱ���������˵��ݿ����µĳ˿�
		if (passengerL[i].req_place_to == elevator1.state()) {
			if (passengerL[i].use == 1 && passengerL[i].leave == 0) {
				passengerL[i].leave = 1;
				passengerL[i].req_arrive_time = Global_time;
				elevator1.out(1);
				control1.downdate_state(elevator1.state());
				state = 1;
			}
		}
	}
	if (state == 1) {
		out2.open("output2.txt", ios::app | ios::out);
		out2 << Global_time << " " << elevator1.state() << "\n";
		out2.close();
		return 1;
	}
	else {
		return 0;
	}
}
//���ĵ��Ȳ���
void run() {
	int person,/*person_number=1,*/floor_stop = 0;
	person = find_passenger();//�ҵ�һ����
	if (person != -1) {
		//����ʱ��δ��������
		if (Global_time<passengerL[find_passenger()].req_time) {
			bool k = 0;
			for (int i = 1; i <= n; i++) {
				if (passengerL[i].use == 1 && passengerL[i].leave == 0) 
					k = 1;
			}
			if (k == 0) 
				Global_time = passengerL[find_passenger()].req_time;
		}
		else {
			control1.update_state(passengerL[person].req_place_fr);
		}
	}
	//�ƶ���;+��;�ӿ�/�¿�
	while (elevator1.state() != find_aim_place(elevator1.state())) {
		//��;����
		floor_stop = put_out();
		while (check_get()) {//�ƶ�����;����
			floor_stop = 1;//ͣ��ʱ��
			int temp_passenger = find_passenger();//��ֹ�ظ�����
			passengerL[temp_passenger].use = 1;//�Ѿ���Ӧ
			passengerL[temp_passenger].get_time = Global_time + 1;//����ʱ��
			control1.update_state(passengerL[temp_passenger].req_place_to);//����Ŀ��¥�㰴ť
			elevator1.get(1);//���ݼ���
		}
		control1.downdate_state(elevator1.state()); //�����¥����
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
	//����
	floor_stop = put_out();
	while (check_get()) {//����
		floor_stop = 1;//ͣ��ʱ��
		int temp_passenger = find_passenger();//��ֹ�ظ�����
		passengerL[temp_passenger].use = 1;//�Ѿ���Ӧ
		passengerL[temp_passenger].get_time = Global_time + 1;//����ʱ��
		control1.update_state(passengerL[temp_passenger].req_place_to);//����Ŀ��¥�㰴ť
		elevator1.get(1);//���ݼ���
	}
	Global_time = Global_time + floor_stop;
	control1.downdate_state(elevator1.state()); //�����¥����
}
int main() {
	begin();
	//int ff = 0;
	//��������˵�����û�����㣬���ݾͼ�������
	out1.open("output1.txt", ios::app | ios::out);
	out3.open("output3.txt", ios::app | ios::out);
	while (check_queue_use() != 0) 
		run();
	out1.close();
	out2.close();
	out3.close();
	return 0;
}