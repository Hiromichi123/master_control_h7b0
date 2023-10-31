//
// Created by XuJiang on 2023/5/17.
//
#include "main.h"
#include "emm42.h"

/**
  * @brief ��ȡ����������
  * @note ��ȡλ�ô���������ֵ����������Ϊuint16_t����ΧΪ0-65535
  * @param �����Ϣ�ṹ�壬��Ҫ����ı��������ݵ�ַ
  * @retval �ɹ���־
  */
int emm42ReadEncoder(emm42_motor motor, uint16_t* dataOfEncoder){
    command readCommand;
    readCommand.commandByte = 0x30;
    readCommand.paramsLength = 0;
    emmMotorSend(motor, readCommand);
    UChar dataBack[4];
    int dataBackLength=4;
#ifdef TM4
    UChar *p=dataBack;
    for (int i=0; i<dataBackLength; i++){
        *p = UARTCharGet(*motor.uart);
        p++;
    }
#endif // TM4
#ifdef STM32
    HAL_UART_Receive(motor.uart, (uint8_t *)dataBack, dataBackLength, HAL_MAX_DELAY);
#endif // STM32
    if(dataBack[0] == motor.address && dataBack[3] == motor.checkByte){
        *dataOfEncoder = (dataBack[1] << 8) | dataBack[2];
        return SUCCESS;
    }
    return ERROR;
}

/**
  * @brief ��ȡλ������
  * @note ��ȡ�����ʵʱλ�ã�����ӦС��Ļ�ϵĵ�1�е����ݣ���������Ϊint32_t����ΧΪ-2147483647-2147483647
  * @param �����Ϣ�ṹ�壬��Ҫ�����λ�����ݵ�ַ
  * @retval �ɹ���־
  */
int emm42ReadPosition(emm42_motor motor, int32_t* dataOfPosition){
    command readCommand;
    readCommand.commandByte = 0x36;
    readCommand.paramsLength = 0;
    emmMotorSend(motor, readCommand);
    UChar dataBack[6];
    int dataBackLength=6;
#ifdef TM4
    UChar *p=dataBack;
    for (int i=0; i<dataBackLength; i++){
        *p = UARTCharGet(*motor.uart);
        p++;
    }
#endif // TM4
#ifdef STM32
    HAL_UART_Receive(motor.uart, (uint8_t *)dataBack, dataBackLength, HAL_MAX_DELAY);
#endif // STM32
    if(dataBack[0] == motor.address && dataBack[5] == motor.checkByte){
        *dataOfPosition = (dataBack[1] << 24) | (dataBack[2] << 16) | (dataBack[3] << 8) | dataBack[4];
        return SUCCESS;
    }
    return ERROR;
}
