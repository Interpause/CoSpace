#ifndef PID_CONTROLLER
#define PID_CONTROLLER
typedef struct
{
	float v;	//PID Output
	float x;	//PID Target
	
	float kp;	//Proportionality Constant
	float ki;	//Integral Constant
	float kd;	//Derivative Constant
	
	float ep;	//Previous Error
	float ip;	//Accumulated Integral Error
} pid_t;

pid_t pid_init(float tgt_x,float p,float i,float d);

pid_t pid_update(pid_t pid,float cur_x,float dt);

void pid_print(pid_t pid);
#endif