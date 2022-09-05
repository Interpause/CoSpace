#include "pid.h"
#include "config.h"
static int is_init = 0;
static pid_t line_pid;
static pid_t comp_pid;

int onColor = 0;
int CS_Y = 0;
int A_R,A_G,A_B = 0;
int RBuf[100],GBuf[100],BBuf[100];
int BufLen = 0;

int tgt_ang_abs = -1;
int tgt_ang_err = 0;

int checkpoint = 0;
int isPast = 0;
int resume_duration = RESUME_DURATION; //set to -1 to temporarily disable resume sequence
int is_line_black = IS_LINE_BLACK;

/**************
 * github.com/Interpause
 * Left is negative, Right is positive
 **************/

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

float line_value()
{   
    //IR values 0:black, 1:white, bool. There was no better way to exploit between-ness
    int IR[6] = {IR_L3,IR_L2,IR_L1,IR_R1,IR_R2,IR_R3};
    if(is_line_black) for(int i=0;i<6;i++) IR[i] = !IR[i];
    if(memcmp((int[6]){1,0,0,0,0,0},IR,sizeof(int[6]))==0) return K_IR[4];
    if(memcmp((int[6]){1,1,0,0,0,0},IR,sizeof(int[6]))==0) return K_IR[3];
    if(memcmp((int[6]){0,1,0,0,0,0},IR,sizeof(int[6]))==0) return K_IR[2];
    if(memcmp((int[6]){0,1,1,0,0,0},IR,sizeof(int[6]))==0) return K_IR[1];
    if(memcmp((int[6]){0,0,1,0,0,0},IR,sizeof(int[6]))==0) return K_IR[0];
    if(memcmp((int[6]){0,0,1,1,0,0},IR,sizeof(int[6]))==0) return 0.;
    if(memcmp((int[6]){0,0,0,1,0,0},IR,sizeof(int[6]))==0) return -K_IR[0];
    if(memcmp((int[6]){0,0,0,1,1,0},IR,sizeof(int[6]))==0) return -K_IR[1];
    if(memcmp((int[6]){0,0,0,0,1,0},IR,sizeof(int[6]))==0) return -K_IR[2];
    if(memcmp((int[6]){0,0,0,0,1,1},IR,sizeof(int[6]))==0) return -K_IR[3];
    if(memcmp((int[6]){0,0,0,0,0,1},IR,sizeof(int[6]))==0) return -K_IR[4];
    return 0.;
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

int turn_abs(int tgt){
    int e = tgt - (360-RotationZ);
    if(e > 180) e -= 360;
    else if(e < -180) e += 360;
    tgt_ang_abs = tgt;
    tgt_ang_err = e;
    return e;
}

int turn_rel(int ang){
    int tgt = (360-RotationZ) + ang;
    tgt = (tgt+360)%360;
    return turn_abs(tgt);
}

int isColor(int r[6])
{
    return r[0]<=A_R && A_R<=r[1] && r[2]<=A_G && A_G<=r[3] && r[4]<=A_B && A_B<=r[5];
}

int check(float ranges[],float vals[],int n){
    for(int i = 0; i<n; i++) if(vals[i]<ranges[2*i] || vals[i]>ranges[2*i+1]) return false;
    return true;
}

int waitColor(int c[]){
    if(isColor(c))
    {
        Duration += WAIT_DURATION + RESUME_DURATION;
        printf("PAUSE: %dms\n",Duration);
        setSteering(0,0);
        LED_1=1;
        return true;
    };
    return false;
}

void init()
{
    CS_Y = (CS_R+CS_R+CS_B+CS_G+CS_G+CS_G)/6;
    onColor = CS_Y > 30 && CS_Y < 225;
    if(onColor){
        RBuf[BufLen] = CS_R;
        GBuf[BufLen] = CS_G;
        BBuf[BufLen] = CS_B;
        if(++BufLen==100) BufLen = 0;
    }else if(BufLen > 0){
        A_R = RBuf[(int)BufLen/2];
        A_G = GBuf[(int)BufLen/2];
        A_B = BBuf[(int)BufLen/2];
        BufLen = 0;
    }else{
        A_R = 0; A_G = 0; A_B = 0;
    }
    //printf("Darkness: %d, BufLen: %d\n",CS_Y,BufLen);
	if(is_init) return;
	is_init = true;
	line_pid = pid_init(0.,LINE_P,LINE_I,LINE_D);
    comp_pid = pid_init(0.,COMP_P,COMP_I,COMP_D);
}

void loop(unsigned short dt)
{
    init();
    //Pauses the code if duration > 0... doesnt necessarily stop robot from doing stuff
    if(Duration > 0)
    {
        Duration -= dt;
        if(Duration < resume_duration) setSteering(0,RESUME_SPEED); //linetrack?
        return;
    };
    if(tgt_ang_abs != -1){
        comp_pid = pid_update(comp_pid,-abs(tgt_ang_err),dt);
        if(tgt_ang_err<0) setSteering(-100,(int)comp_pid.v);
        else setSteering(100,(int)comp_pid.v);
        //updates error, also helpfully gives error free of the 359-0=359 problem
        if(abs(turn_abs(tgt_ang_abs))<5){
            tgt_ang_abs = -1; //sets back to sentinel value
            setSteering(0,FORWARD_EXTRA_SPEED);
            resume_duration = -1;
            Duration = FORWARD_EXTRA_DURATION; //not gonna spin on the same spot cause of same color
            comp_pid = pid_update(comp_pid,0,dt);
        }       
        //printf("tgt:%d,err:%d,val:%d\n",tgt_ang_abs,tgt_ang_err,(int)comp_pid.v);
        return;
    };

    resume_duration = RESUME_DURATION; //restores resume sequence
    //shouldnt ever run PID while paused due to accumulation
    float val = line_value();
    line_pid = pid_update(line_pid,val,dt);

    #ifdef CARNIVAL
    //printf("PID: OUT=%f, De=%f, I=%f\n",line_pid.v,line_pid.ep+val,line_pid.ip);
	setSteering(line_pid.v,LINE_SPEED_NORMAL-abs(K_SLOWDOWN)*line_pid.ep);
    #endif

    #ifdef SPEEDWAY
    //checkpoint wait
    if(waitColor(PURPLE)){
        checkpoint++;
        return;
    }

    //state tracking
    if(checkpoint==2) isPast = 0;
    if(abs(RotationZ-318)<5&&abs(RotationX-21)<3) isPast = 1;
    if(checkpoint==3&&abs(RotationZ-40)<5&&abs(RotationX-0)<3) isPast = 1;

    //conditional speed
    if(checkpoint==2&&!isPast) setSteering(line_pid.v,LINE_SPEED_SLOW-abs(K_SLOWDOWN)*line_pid.ep);
    else if(checkpoint==3&&!isPast) setSteering(line_pid.v,LINE_SPEED_SLOW-abs(K_SLOWDOWN)*line_pid.ep);
    else setSteering(line_pid.v,LINE_SPEED_NORMAL-abs(K_SLOWDOWN)*line_pid.ep);
    LED_1=0;
    #endif

    #ifdef SMARTCITY
    //before vs after yellow
    if(!isPast){
        //first pink turn
        if(checkpoint==0 && isColor(PINK)){
            setSteering(0,FORWARD_EXTRA_SPEED);
            Duration=FORWARD_EXTRA_DURATION;
            resume_duration = -1;
            checkpoint++;
            turn_rel(90);
            return;
        }
        //first wall turn
        if(checkpoint==4 && US_Front<US_TRIGGER){
            setSteering(0,FORWARD_EXTRA_SPEED);
            Duration=FORWARD_EXTRA_DURATION;
            resume_duration = -1;
            checkpoint++;
            turn_rel(90);
            return;
        }
        //check if in second half (after yellow)
        if(checkpoint==6 && isColor(YELLOW)){
            isPast = true;
            return;
        }
        //checkpoints
        if(waitColor(ORANGE)){
            //uturn for first checkpoint
            if(checkpoint++==1){
                resume_duration = -1;
                turn_rel(180);
            }
            return;
        }
    }else{
        //first green turn
        if((checkpoint==6 || checkpoint==11) && isColor(GREEN)){
            setSteering(0,FORWARD_EXTRA_SPEED);
            Duration=FORWARD_EXTRA_DURATION;
            resume_duration = -1;
            checkpoint++;
            turn_rel(90);
            return;
        }
        //cyan U turn
        if(checkpoint==7 && isColor(CYAN)){
            setSteering(0,0);
            Duration=FORWARD_EXTRA_DURATION;
            checkpoint++;
            resume_duration = -1;
            turn_rel(180);
            return;
        }
        //gray wall turn
        if(checkpoint==10 && US_Front<US_TRIGGER){
            setSteering(0,FORWARD_EXTRA_SPEED);
            Duration=FORWARD_EXTRA_DURATION;
            resume_duration = -1;
            checkpoint++;
            turn_rel(90);
            return;
        }
        //skip zigzag
        if(checkpoint==13 && isColor(PINK)){
            setSteering(0,FORWARD_EXTRA_SPEED);
            Duration=FORWARD_SKIP_ZIGZAG;
            checkpoint++;
            resume_duration = -1;
            return;
        }
        //checkpoints
        if(checkpoint>=8 && waitColor(ORANGE)){
            checkpoint++;
            return;
        }
    }
    //printf("isPast: %d, Checkpoint: %d\n",isPast,checkpoint);
    setSteering(line_pid.v,LINE_SPEED_NORMAL-abs(K_SLOWDOWN)*line_pid.ep);
    LED_1=0;
    #endif
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