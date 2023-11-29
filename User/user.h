/**
  ******************************************************************************
  * @file           : user.h
  * @author         : XuJiang
  * @brief          : None
  * @attention      : None
  * @date           : 2023/8/28
  ******************************************************************************
  */

#ifndef H7B0_RUBBISH_USER_H
#define H7B0_RUBBISH_USER_H

#include "stdbool.h"
#include "emm42/emm42.h"
#include "key.h"
#include "OLED/oled.h"
#include "OLED/bmp.h"

#include "Emm_V5.h"

#define uartDebug huart1
#define uartFull huart2
#define uartUseless huart3
#define uartPushControl huart4
#define uartMotorC huart5
#define uartMotorD huart6
#define uartPullingPlateMotor huart7
#define uartMotorB huart8
#define uartTurnMotor huart9
#define uartMotorA huart10


extern int mode;
extern int R, H, K, O;
extern bool F;
extern int T;

void setup();
void loop();

void OledInit();
void OledUpdate();

enum RUBBISH_TYPE{
    HAZARDOUS,
    RECYCLABLE,
    KITCHEN,
    OTHER,
};
typedef uint8_t binTypeDef;
extern char rubbishType2Str[4][32];

extern emm42_motor motorA, motorB, motorC, motorD;
extern uint16_t m1_encoder, m2_encoder;

extern int A, B, C, D;
extern uint8_t S;

int emm42_motor_init();
bool ABCDMotorStop();

int limit(int data, int rangeMax, bool allowNeg);
bool upMotorsSpeedControl();
bool downMotorsSpeedControl();

bool servoTrun(uint8_t servoNum, uint16_t angle);
bool turnToBin(binTypeDef type);
bool turnMotorControl();
bool downDoorControl(uint8_t open);
bool pullingPlateMotorControl(bool open);

extern enum RUBBISH_TYPE rubbishType;
extern uint8_t lastRubbishChar;
extern int motorTrunClk;

extern int P, P_g030, P_AGX;
extern int I,J;
extern int Q, Q_g030, Q_AGX;

void updatePStatus();

#endif //H7B0_RUBBISH_USER_H
