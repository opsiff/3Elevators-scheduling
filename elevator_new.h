#pragma once
class elevator_new
{
public:
	elevator_new(int n,int k=0);
	~elevator_new();
	int ask_floor();
	int ask_for_state();
	int update_state(int floor);//����ĳһ����
	int downdate_state(int floor);//�������ĳһ����
	void run();
private:
	void get(int people);
	void out(int people);
	void MoveUp();
	void MoveDowm();
	int aim[50];//�������е�Ŀ�ı��¥��
	int state;//��������״̬ 1����2�п�
	int people_num;//�����ڳ˿�����
	int elevator_place;//���ݵ�ǰλ��
	int timer;//���ݵ�ǰʱ��
};

