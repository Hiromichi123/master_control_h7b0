/**
  ******************************************************************************
  * @file           : servo.c
  * @author         : XuJiang
  * @brief          : None
  * @attention      : None
  * @date           : 2023/8/29
  ******************************************************************************
  */

#include "main.h"
#include "user.h"
#include "stdlib.h"

int motorTrunClk = 0;
uint8_t lastRubbishChar = 'H';
enum RUBBISH_TYPE rubbishType = OTHER;
int binTransform[4][4] = {{0,     800,   1600, -800},
                          {-800,  0,     800,  1600},
                          {-1600, -800,  0,    800},
                          {800,   -1600, -800, 0}};
// 第i行第j列为垃圾桶i到j的clk变换
char rubbishType2Str[4][32] = {"H", "R", "K", "O"};


bool turnToBin(binTypeDef type) {
    motorTrunClk += binTransform[rubbishType][type];

    rubbishType = type;
    lastRubbishChar = rubbishType2Str[type][0];
    return SUCCESS;
}

bool turnMotorControl() {
    static int lastMotorTurnClk;
    if (motorTrunClk != lastMotorTurnClk) {
        lastMotorTurnClk = motorTrunClk;
        Emm_V5_Pos_Control(&uartTurnMotor, 1, motorTrunClk < 0, 100, 192, abs(motorTrunClk), true, false);
    }
    return SUCCESS;
}
