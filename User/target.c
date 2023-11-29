/**
  ******************************************************************************
  * @file           : target.c
  * @author         : XuJiang
  * @brief          : 用于处理目标信息
  * @attention      : None
  * @date           : 2023/8/3
  ******************************************************************************
  */
#include "stdlib.h"
#include "stdbool.h"
#include "main.h"
#include "target.h"

/**
  * @brief 目标初始化函数
  * @note 初始化Target，calloc会自动对空间设置值为0
  * @param None
  * @retval 指向一个Target结构体的指针
  */
Target_TypeDef *Target_new(void) {
    return (Target_TypeDef *) calloc(1, sizeof(Target_TypeDef));
}

staticTarget_TypeDef *staticTarget_new(void) {
    staticTarget_TypeDef *p = (staticTarget_TypeDef *) calloc(1, sizeof(Target_TypeDef));
    return p;
}

bool transformEncoder2Pulse(uint16_t aimEncoder, uint16_t curEncoder, bool *direction, uint32_t *pulse) {
    if (abs(aimEncoder - 65536 / 2) > (65536 / 2 - 50)) {
        if (aimEncoder < 50 && curEncoder > 65536 - 500) {
            *pulse = (aimEncoder + 65536 - curEncoder) * 25 / 512; // 将编码器值换算到脉冲值
            *direction = BACK;
            return SUCCESS;
        }
        else if (aimEncoder > 65536 - 50 && curEncoder < 50) {
            *pulse = (curEncoder + 65536 - aimEncoder) * 25 / 512; // 将编码器值换算到脉冲值
            *direction = FORWARD;
            return SUCCESS;
        }
    }
    if (curEncoder < aimEncoder) {
        *pulse = (uint32_t)(aimEncoder - curEncoder) * 25 / 512; // 将编码器值换算到脉冲值
        *direction = BACK;
    }
    else if (curEncoder > aimEncoder) {
        *pulse = (uint32_t)(curEncoder - aimEncoder) * 25 / 512; // 将编码器值换算到脉冲值
        *direction = FORWARD;
    }
    return SUCCESS;
}
