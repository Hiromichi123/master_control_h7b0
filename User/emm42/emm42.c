//
// Created by XuJiang on 2023/5/17.
//
#include "main.h"
#include "emm42.h"
#ifdef EMM42_DEBUG
#include "stdio.h"
#endif

/**
  * @brief ����������ڳ�ʼ��
  * @note ��ɴ��ںţ����ݵ�ַ��У���ֽ�����
  * @param �����Ϣ�ṹ��ָ�룬����ָ�룬���ݵ�ַ�ֽڣ�У���ֽ�
  * @retval �ɹ���־
  */
int emmMotorUartInit(emm42_motor *motor, UART_HandleTypeDef *uart, UChar address, UChar checkByte){
    motor->uart = uart;
    motor->address = address;
    motor->checkByte = checkByte;
    return SUCCESS;
}

/**
  * @brief ��������������ݷ���
  * @note None
  * @param �����Ϣ�ṹ�壬���͵�ָ��ṹ��
  * @retval �ɹ���־
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
    //��ջ�����
    while (UARTCharGetNonBlocking(*motor.uart) != -1);
    SysDelay_ms(10);

    for (int i = 0; i < commandLength; i++){
        UARTCharPut(*motor.uart, hexData[i]);
    }

#endif

    return SUCCESS;
}