#define LINE_SPEED_NORMAL 80.
//PID values for linetracking
#define LINE_P 7.5
#define LINE_I 0.0001
#define LINE_D 2000.
//Multiplier for IR sensor values, 3 being the furthest out
#define K_IR3 3.
#define K_IR2 2.
#define K_IR1 1.
//Real problem... 3 variables PID.
#define K_SLOWDOWN 0

//47s, stable: P=7.5,I=0.0001,D=2000,Speed=80,IR3=3,IR2=2,IR1=1, no slowdown