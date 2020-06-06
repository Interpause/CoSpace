//in cm
#define WHEEL_RADIUS 5
#define WHEEL_DIST 10
//speed
#define LINE_SPEED_NORMAL 80.
//PID values for linetracking
#define LINE_P 5.
#define LINE_I 0.0001 //tuned
#define LINE_D 10000000. //discrete line => INTMAX
//Multiplier for IR sensor values (assume linear)
#define K_IR (float[5]){1,2,3,4,5}
//probably wont use
#define K_SLOWDOWN 0