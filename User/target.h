/**
  ******************************************************************************
  * @file           : target.h
  * @author         : XuJiang
  * @brief          : 用于处理目标信息
  * @attention      : None
  * @date           : 2023/8/3
  ******************************************************************************
  */

#ifndef STM32H7B0_CAR_TARGET_H
#define STM32H7B0_CAR_TARGET_H

#include "stm32h7xx_hal.h"
#include "stdint.h"
#include "user.h"

typedef struct {
    uint16_t target1,target2,targetX1, targetX2,targetX3, targetX4,
            targetY1, targetY2,targetY3, targetY4,targetGx,targetGy,targetRx,targetRy;
    volatile uint16_t targetX, targetY;
} Target_TypeDef;

typedef struct {
    uint16_t p0x,p0y;
} staticTarget_TypeDef;

Target_TypeDef *Target_new(void);
staticTarget_TypeDef *staticTarget_new(void);
bool transformEncoder2Pulse(uint16_t aimEncoder, uint16_t curEncoder, bool *direction, uint32_t* pulse);

#endif //STM32H7B0_CAR_TARGET_H
