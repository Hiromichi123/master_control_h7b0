//
// Created by XuJiang on 2023/6/3.
//
#include "main.h"
#include "key.h"
#include "usart.h"
#include "user.h"
#include "tim.h"

#define GPIO_EXTI_FOR_KEY

int keyValue = KEY_NONE_PRESS;

uint32_t keyPressTime = 0;

#ifdef GPIO_EXTI_FOR_KEY

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
//    if (GPIO_Pin == KEY1_Pin || GPIO_Pin == KEY2_Pin || GPIO_Pin == KEY3_Pin)  //判断中断源
//    {
//        keyPressTime = HAL_GetTick(); // 记录当前时间
//        HAL_TIM_Base_Start_IT(&htim17);
//    }
    if (GPIO_Pin == LASER1_Pin) {
        T1 = HAL_GetTick();
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
    }
    else if (GPIO_Pin == LASER2_Pin) {
        T2 = HAL_GetTick();
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == htim17.Instance) {
        processKey();
    }
//    if (htim->Instance == htim16.Instance) {
//        if (HAL_GPIO_ReadPin(LASER_GPIO_Port, LASER_Pin) == 0) {
//            T = HAL_GetTick();
//            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET);
////            HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
//        }
//        if (HAL_GetTick() - T > 200) {
//            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
////            HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
//        }
//    }
}


/**
  * @brief 获取按键信息的函数
  * @note 不会阻塞程序
  * @param None
  * @retval None
  */
int getKey() {
    // 检测案件按下状态，阻塞程序
    // K1和K4标反了，CubeMX以原理图为准，但程序将K1返回K4
    if (!HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)) {
        return KEY1_PRESS;
    } else if (!HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)) {
        return KEY2_PRESS;
    } else if (!HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)) {
        while (!HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)) {}
        return KEY3_PRESS;
    }
    //None pressed
    return KEY_NONE_PRESS;
}

#endif

#ifndef GPIO_EXTI_FOR_KEY
/**
  * @brief 获取按键信息的函数
  * @note 会阻塞程序，不能在中断里用，否则请自行改为中断控制
  * @param None
  * @retval None
  */
int getKey() {
    // 检测案件按下状态，阻塞程序
    // K1和K4标反了，CubeMX以原理图为准，但程序将K1返回K4
    if (!HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)) {
        while (!HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)) {}
        return KEY4_PRESS;
    }
    else if (!HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)) {
        while (!HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)) {}
        return KEY2_PRESS;
    }
    else if (!HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)) {
        while (!HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)) {}
        return KEY3_PRESS;
    }
    else if (!HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin)) {
        while (!HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin)) {}
        return KEY1_PRESS;
    }
    else if (!HAL_GPIO_ReadPin(KEY_USER_GPIO_Port, KEY_USER_Pin)) {
        while (!HAL_GPIO_ReadPin(KEY_USER_GPIO_Port, KEY_USER_Pin)) {}
        return KEY_USER_PRESS;
    }
    //None pressed
    return KEY_NONE_PRESS;
}
#endif

/**
  * @brief 处理按键的功能函数
  * @note 会阻塞程序
  * @param None
  * @retval None
  */
int processKey() {
#ifndef GPIO_EXTI_FOR_KEY
    keyValue = getKey();
#endif
    if (HAL_GetTick() - keyPressTime < 50) {
        return KEY_NONE_PRESS;
    }
    HAL_TIM_Base_Stop_IT(&htim17);
    keyValue = getKey();
    keyPressTime = 0;
    int lastKeyValue = keyValue;
    switch (keyValue) {
        case KEY1_PRESS:
            A = B = 10;
            break;
        case KEY2_PRESS:
            A = 10;
            B = -10;
            break;
        case KEY3_PRESS:
            C = -20;
            D = 20;
            break;
        default:
            break;
    }
    keyValue = KEY_NONE_PRESS;
    return lastKeyValue;
}
