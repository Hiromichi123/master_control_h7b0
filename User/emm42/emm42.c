//
// Created by XuJiang on 2023/5/17.
//
#include "main.h"
#include "emm42.h"
#ifdef EMM42_DEBUG
#include "stdio.h"
#endif

/**
  * @brief 步进电机串口初始化
  * @note 完成串口号，数据地址，校验字节配置
  * @param 电机信息结构体指针，串口指针，数据地址字节，校验字节
  * @retval 成功标志
  */
int emmMotorUartInit(emm42_motor *motor, UART_HandleTypeDef *uart, UChar address, UChar checkByte){
    motor->uart = uart;
    motor->address = address;
    motor->checkByte = checkByte;
    return SUCCESS;
}

/**
  * @brief 步进电机串口数据发送
  * @note None
  * @param 电机信息结构体，发送的指令结构体
  * @retval 成功标志
  */
int emmMotorSend(emm42_motor motor, command sendCommand){
    UChar hexData[20] = {0}, *p = hexData;

    *p = motor.address;
    p++;

    *p = sendCommand.commandByte;
    p++;

    for(int i=0;i<sendCommand.paramsLength; i++){
        *p = sendCommand.params[i];
        p++;
    }

    *p = motor.checkByte;
    p++;

    int commandLength = p - hexData;
#ifdef EMM42_DEBUG
    printf("uart=%d\t", *motor.uart);
    printChar(hexData, commandLength);
#endif

#ifdef STM32
    uint8_t uselessData;
    while (HAL_UART_Receive(motor.uart, &uselessData, 1, 1) != HAL_TIMEOUT);
    HAL_Delay(20);
    HAL_UART_Transmit(motor.uart, (uint8_t *) hexData, commandLength, HAL_MAX_DELAY);
#endif

#ifdef TM4
    //清空缓冲区
    while (UARTCharGetNonBlocking(*motor.uart) != -1);
    SysDelay_ms(10);

    for (int i = 0; i < commandLength; i++){
        UARTCharPut(*motor.uart, hexData[i]);
    }

#endif

    return SUCCESS;
}