#include "elevator_new.h"



elevator_new::elevator_new(int n,int k=0)//一个有n个连续目的楼层的电梯,起始楼层k;
{
	state = 1;//empty
	for (int i = 0; i <= n; i++) {
		aim[i] = 0;
	}
	elevator_place = k;
}


elevator_new::~elevator_new()
{
}
void elevator_new::MoveDowm() {
	elevator_place--;
}
void elevator_new::MoveUp() {
	elevator_place++;
}
void elevator_new::get(int people) {
	people_num += people;
}
void elevator_new::out(int people) {
	people_num -= people;
}
int elevator_new::ask_floor() {
	return elevator_place;
}
int elevator_new::ask_for_state() {
	return state;
}
int elevator_new::update_state(int floor) {
	if (floor == -1) {
		state = 1;
		return 0;
	}
	else {
		state = 2;
		aim[floor] = 1;
		return 1;
	}
}
int elevator_new::downdate_state(int floor) {
	aim[floor] = 0;
	bool k = 0;
	for (int i = 0; i <= 10; i++) {
		if (aim[i] == 1)
			k = 1;
	}
	if (k == 0)
		state = 1;
	return 1;
}
int elevator_new::run() {
}