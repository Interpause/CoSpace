/**
 * The Gyro can be very useful:
 * - detect curves & apply slower speed+PID, allows speed to be higher for straight portions
 * - detect slopes up/down for similar mitigations
 * - increase detection interval else detection impossible
 * 
 * Hardcode detection & strategy change using time ranges? but is this wrong?
 * Track segmentation using gyro & runtime & track features for strategy
 */

//unused
#define WHEEL_RADIUS 5
#define WHEEL_DIST 10
#define K_SLOWDOWN 0 //probably wont use

#define WAIT_DURATION 2000
#define RESUME_DURATION 300
#define RESUME_SPEED 50

#define COMP_P .6
#define COMP_I 0
#define COMP_D 12.

//#define CARNIVAL
//#define SPEEDWAY
//#define SMARTCITY
#define ECOGARDEN

#ifdef SMARTCITY
#define LINE_SPEED_NORMAL 50.
#define LINE_SPEED_SLOW 10.
#define LINE_P 8.2
#define LINE_I 0.00005
#define LINE_D 600.
#define K_IR (float[5]){1,2,3,5,7}
#define IS_LINE_BLACK 1
#define PINK (int[6]){230,255,0,30,230,255}
#define ORANGE (int[6]){230,255,150,180,0,30}
#define YELLOW (int[6]){230,255,230,255,0,30}
#define GREEN (int[6]){80,120,230,255,80,120}
#define CYAN (int[6]){0,30,230,255,230,255}
#define US_TRIGGER 21

#define FORWARD_SKIP_ZIGZAG 2000
#define FORWARD_EXTRA_DURATION 210
#define FORWARD_EXTRA_SPEED 40
#endif

#ifdef SPEEDWAY
#define LINE_SPEED_NORMAL 60.
#define LINE_SPEED_SLOW 40.
#define LINE_P 7.5
#define LINE_I 0.00005
#define LINE_D 10000000.
#define K_IR (float[5]){1,2,3,5,7}
#define IS_LINE_BLACK 0
#define PURPLE (int[6]){105,135,10,40,145,175}
#endif

#ifdef CARNIVAL
#define LINE_SPEED_NORMAL 85.
#define LINE_P 5.0
#define LINE_I 0.0001 //tuned
#define LINE_D 10000000. //discrete line => INTMAX
#define K_IR (float[5]){1,2,3,4,6} //assumed linear
#define IS_LINE_BLACK 1
#endif