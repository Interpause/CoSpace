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

#define COMP_P 1.6
#define COMP_I 0
#define COMP_D 64.
#define ANG_MARGIN 2
#define TURN_SPEED_CAP 65

#define LINE_SPEED_NORMAL 40.
#define LINE_SPEED_SLOW 10.
#define LINE_P 8.2
#define LINE_I 0.00005
#define LINE_D 1000.
#define K_IR (float[5]){1,2,3,5,7}
#define IS_LINE_BLACK 0
#define US_TRIGGER 20

#define ORANGE (int[6]){185,215,105,135,0,15}
#define RED (int[6]){240,255,0,15,0,15}
#define CYAN (int[6]){0,15,240,255,240,255}
#define BLUE (int[6]){0,15,0,15,240,255}

#define FORWARD_EXTRA_SPEED 40
#define FORWARD_EXTRA_DURATION 240