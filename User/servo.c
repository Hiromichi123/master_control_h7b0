/**
  ******************************************************************************
  * @file           : servo.c
  * @author         : XuJiang
  * @brief          : None
  * @attention      : None
  * @date           : 2023/10/20
  ******************************************************************************
  */
#include "main.h"
#include "user.h"
#include "stdlib.h"

uint8_t S=0;

bool servoTrun(uint8_t servoNum, uint16_t angle){
    angle = limit(angle, 270, false);

    int ccr = (angle / 18.0 + 2.5) * 200;
    if(servoNum == 1){
        TIM1->CCR1 = ccr;
    }
    else if(servoNum == 2){
        TIM1->CCR2 = ccr;
    }
    else if (servoNum == 4) {
        TIM1->CCR4 = ccr;
    }
    else{
        return ERROR;
    }
    return SUCCESS;
}

bool downDoorControl(uint8_t open){
    if (!open){
        // 两扇门那边不开
        servoTrun(1,147);
        servoTrun(2,41);
    }
    else{
        // 两扇门那边开
        servoTrun(1,77-5);
        servoTrun(2,111+5);
    }
    S = open;
    return SUCCESS;
}
