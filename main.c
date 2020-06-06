#define CsBot_AI_H//DO NOT delete this line
#ifndef CSBOT_REAL
#include <windows.h>
#include <stdio.h>
#include <math.h>
#define DLL_EXPORT extern __declspec(dllexport)
#define false 0
#define true 1
#endif
//Find robot CID from your CoSpace Robot label or CoSpace program download GUI.
char AI_MyID[6] = {'1','2','3','4','5','6'};

int Duration = 0;
int CurAction = -1;
int CurGame = 0;
int MyState_1 = 0;
int US_Front = 0;
int IR_L3 = 0;
int IR_L2 = 0;
int IR_L1 = 0;
int IR_R1 = 0;
int IR_R2 = 0;
int IR_R3 = 0;
int CS_R = 0;
int CS_G = 0;
int CS_B = 0;
int RotationX = 0;
int RotationY = 0;
int RotationZ = 0;
int Time = 0;
int WheelLeft = 0;
int WheelRight = 0;
int LED_1 = 0;
int AI_SensorNum = 14;
int AI_TeamID = 1; //Robot Team ID. 1:Blue Ream; 2:Red Team.
#define CsBot_AI_C//DO NOT delete this line

DLL_EXPORT void SetGameID(int GameID)
{
    CurGame = GameID;
}

DLL_EXPORT int GetGameID()
{
    return CurGame;
}

#ifndef CSBOT_REAL

char info[3000];
DLL_EXPORT char* GetDebugInfo()
{
    sprintf(info, "Duration=%d;CurAction=%d;CurGame=%d;MyState_1=%d;US_Front=%d;IR_L3=%d;IR_L2=%d;IR_L1=%d;IR_R1=%d;IR_R2=%d;IR_R3=%d;CS_R=%d;CS_G=%d;CS_B=%d;RotationX=%d;RotationY=%d;RotationZ=%d;Time=%d;WheelLeft=%d;WheelRight=%d;LED_1=%d;",Duration,CurAction,CurGame,MyState_1,US_Front,IR_L3,IR_L2,IR_L1,IR_R1,IR_R2,IR_R3,CS_R,CS_G,CS_B,RotationX,RotationY,RotationZ,Time,WheelLeft,WheelRight,LED_1);
    return info;
}
 
DLL_EXPORT char* GetTeamName()
{
     return "Interpause";
}

DLL_EXPORT int GetCurAction()
{
    return CurAction;
}

#endif ////CSBOT_REAL

DLL_EXPORT void SetDataAI(volatile int* packet, volatile int *AI_IN)
{

    int sum = 0;

    MyState_1 = AI_IN[0]; packet[0] = MyState_1; sum += MyState_1;
    US_Front = AI_IN[1]; packet[1] = US_Front; sum += US_Front;
    IR_L3 = AI_IN[2]; packet[2] = IR_L3; sum += IR_L3;
    IR_L2 = AI_IN[3]; packet[3] = IR_L2; sum += IR_L2;
    IR_L1 = AI_IN[4]; packet[4] = IR_L1; sum += IR_L1;
    IR_R1 = AI_IN[5]; packet[5] = IR_R1; sum += IR_R1;
    IR_R2 = AI_IN[6]; packet[6] = IR_R2; sum += IR_R2;
    IR_R3 = AI_IN[7]; packet[7] = IR_R3; sum += IR_R3;
    CS_R = AI_IN[8]; packet[8] = CS_R; sum += CS_R;
    CS_G = AI_IN[9]; packet[9] = CS_G; sum += CS_G;
    CS_B = AI_IN[10]; packet[10] = CS_B; sum += CS_B;
    RotationX = AI_IN[11]; packet[11] = RotationX; sum += RotationX;
    RotationY = AI_IN[12]; packet[12] = RotationY; sum += RotationY;
    RotationZ = AI_IN[13]; packet[13] = RotationZ; sum += RotationZ;
    Time = AI_IN[14]; packet[14] = Time; sum += Time;
    packet[15] = sum; //some primitive checksum it seems

}
DLL_EXPORT void GetCommand(int *AI_OUT)
{
    AI_OUT[0] = WheelLeft;
    AI_OUT[1] = WheelRight;
    AI_OUT[2] = LED_1;
}

#include "pid.h"
#include "config.h"
static int is_init = false;
static pid_t line_pid;
/**************
 * github.com/Interpause
 * Left is negative, Right is positive
 **************/
void init()
{
	if(is_init) return;
	is_init = true;
	line_pid = pid_init(0.,LINE_P,LINE_I,LINE_D);
};

float line_value()
{
	//IR values 0:black, 1:white, bool. This implementation should? be fairly color independent
	float a = K_IR3*(IR_R3-IR_L3);
	float b = K_IR2*(IR_R2-IR_L2);
	float c = K_IR1*(IR_R1-IR_L1);
	return abs(a) > abs(b)? (abs(a) > abs(c)? a: c): (abs(b) > abs(c)? b: c);
};

void setSteering(float x,float speed)
{
	//Inspired by ev3dev2 moveSteering
	WheelLeft = speed;
	WheelRight = speed;
	float factor = (50.-abs(x))/50.;
	if(x>=0) WheelRight *= factor;
	else WheelLeft *= factor;
}

void loop(unsigned short dt)
{
	init();
    float value = line_value();
    line_pid = pid_update(line_pid,value,dt);
    printf("PID: IN=%f, OUT=%f, PE=%f, PI=%f\n",value,line_pid.v,line_pid.ep,line_pid.ip);
	setSteering(line_pid.v,LINE_SPEED_NORMAL-abs(K_SLOWDOWN)*line_pid.ep);
    LED_1=0;
};

DLL_EXPORT void OnTimer()
{
    switch (CurGame)
    {
        case 9:
            break;
        case 10:
            WheelLeft=0;
            WheelRight=0;
            LED_1=0;
            break;
        case 0:
            loop(25); //fixed dt of 25ms
            break;
        default:
            break;
    }
}