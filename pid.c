#include "pid.h"
#include <stdio.h>

pid_t pid_init(float tgt_x,float p,float i,float d)
{
	pid_t pid;
	pid.kp = p;
	pid.ki = i;
	pid.kd = d;
	pid.x = tgt_x;
	
	pid.v = 0.;
	pid.ep = 0.;
	pid.ip = 0.;
	return pid;
};

pid_t pid_update(pid_t pid,float cur_x,float dt)
{
	float e = pid.x - cur_x;
	pid.ip += e*dt;
	pid.v = pid.kp*e + pid.ki*pid.ip + pid.kd*(e-pid.ep)/dt;
	pid.ep = e;
	return pid;
};

void pid_print(pid_t pid)
{
	printf("pid:(%f,%f,%f),tgt:%f,err:%f,val:%f\n",pid.kp,pid.ki,pid.kd,pid.x,pid.ep,pid.v);
};