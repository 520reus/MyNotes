#include <iostream>
using namespace std;
double G_targetLocation=1.0;	//目标追踪位置
double G_presentLocation = 0.0;	//目标当前位置

float Kp(0), Ki(0), Kd(0);
int main()
{
	double error_p=0;		//kp误差
	double error_i = 0;		//ki误差
	double error_d = 0;		//kd误差
	double error_dp = 0;	//上一次的kp误差、用于计算kd误差

	cout << "请输入目标追踪值" << endl;
	cin >> G_targetLocation;
	cout << "请输入kp" << endl;
	cin >> Kp;
	cout << "请输入ki" << endl;
	cin >> Ki;
	cout << "请输入kd" << endl;
	cin >> Kd;
	error_dp= G_targetLocation - G_presentLocation;	//整个系统第一次计算kd时、 e(k)-e(k-1)、
													//error_dp表示e(k-1)
	while (1) {
		error_p = G_targetLocation - G_presentLocation;
		error_i += error_p;
		error_d = error_p - error_dp;
		error_dp = error_p;
		G_presentLocation += Kp * error_p + Ki * error_i + Kd * error_d;
		cout << "当前位置为：" << G_presentLocation << "当前的误差为" << G_targetLocation - G_presentLocation<<endl;
        
	} 
}
