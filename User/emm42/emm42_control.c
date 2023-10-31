//
// Created by XuJiang on 2023/5/17.
//
#include "main.h"
#include "emm42.h"

/**
  * @brief ���ص��
  * @note ���Ʊջ������ʹ��״̬
  * @param �����Ϣ�ṹ�壬���ر�־
  * @retval �ɹ���־
  */
int emm42Enable(emm42_motor motor, bool enable) {
    command sendCommand;
    sendCommand.commandByte = 0xF3;
    sendCommand.params[0] = enable ? 0x01 : 0x00;
    sendCommand.paramsLength = 1;
    emmMotorSend(motor, sendCommand);
    return SUCCESS;
}

/**
  * @brief �����ٶ�
  * @note ���Ʊջ���������趨�ķ����ٶȺͼ��ٶȽ���һֱ��ת�����ٶ���ָ����������ֹͣʱ���߼Ӽ��ٵ�λ�����Լ��ٵ��������ֹͣ����
  * @param �����Ϣ�ṹ�壬�����ٶȣ����ٶ�
  * @retval �ɹ���־
  */
int emm42SetSpeed(emm42_motor motor, bool direction, uint16_t speed, uint8_t acceleration) {
    command sendCommand;
    sendCommand.commandByte = 0xF6;

    if (speed > 0x4FF) {
        speed = 0x4FF;
    }

    sendCommand.params[0] = (speed & 0xff00) >> 8;
    sendCommand.params[1] = speed & 0x00ff;

    if (direction) {
        sendCommand.params[0] |= 0x10;
    }

    sendCommand.params[2] = acceleration;
    sendCommand.paramsLength = 3;
    emmMotorSend(motor, sendCommand);
    return SUCCESS;
}

/**
  * @brief λ��ģʽ����
  * @note ���Ʊջ���������趨�ķ����ٶȺͼ��ٶȽ������λ���˶�
  * @param �����Ϣ�ṹ�壬�����ٶȣ����ٶȣ�������
  * @retval �ɹ���־
  */
int emm42SetPosition(emm42_motor motor, bool direction, uint16_t speed, uint8_t acceleration, uint32_t pulse, bool wait) {
    command sendCommand;
    sendCommand.commandByte = 0xFD;

    if (speed > 0x4FF) {
        speed = 0x4FF;
    }

    sendCommand.params[0] = (speed & 0xff00) >> 8;
    sendCommand.params[1] = speed & 0x00ff;

    if (direction) {
        sendCommand.params[0] |= 0x10;
    }

    sendCommand.params[2] = acceleration;

    sendCommand.params[3] = (pulse & 0xff0000) >> 16;
    sendCommand.params[4] = (pulse & 0xff00) >> 8;
    sendCommand.params[5] = pulse & 0xff;
    sendCommand.paramsLength = 6;
    emmMotorSend(motor, sendCommand);

    UChar dataBack[3];
    int dataBackLength = 3;
#ifdef TM4
    UChar *p=dataBack;
    for (int i=0; i<dataBackLength; i++){
        *p = UARTCharGet(*motor.uart);
        p++;
    }
#endif // TM4
#ifdef STM32
    HAL_UART_Receive(motor.uart, (uint8_t *) dataBack, dataBackLength, 1000);
#endif // STM32
    if (dataBack[0] == motor.address && dataBack[1]==0x02 && dataBack[2] == motor.checkByte) {
        if(!wait){
            return SUCCESS;
        }
        HAL_UART_Receive(motor.uart, (uint8_t *) dataBack, dataBackLength, 1000);
        if (dataBack[0] == motor.address && dataBack[1]==0x9f && dataBack[2] == motor.checkByte){
            return SUCCESS;
        }
        else{
            return ERROR;
        }
    }
    return ERROR;
}

