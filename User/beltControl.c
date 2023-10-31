/**
  ******************************************************************************
  * @file           : control.c
  * @author         : XuJiang
  * @brief          : None
  * @attention      : None
  * @date           : 2023/8/3
  ******************************************************************************
  */

#include "main.h"
#include "target.h"
#include "user.h"
#include "usart.h"
#include "emm42/emm42.h"

int A = 0, B = 0, C = 0, D = 0;
emm42_motor motorA, motorB, motorC, motorD;
uint16_t m1_encoder, m2_encoder;

int emm42_motor_init(){
    motorA.uart = &uartMotorA;
    motorA.address = 0x01;
    motorA.checkByte = 0x6B;

    motorB.uart = &uartMotorB;
    motorB.address = 0x01;
    motorB.checkByte = 0x6B;

    motorC.uart = &uartMotorC;
    motorC.address = 0x01;
    motorC.checkByte = 0x6B;

    motorD.uart = &uartMotorD;
    motorD.address = 0x01;
    motorD.checkByte = 0x6B;

    return SUCCESS;
}

bool ABCDMotorStop(){
    emm42SetSpeed(motorA, FORWARD, 0, 100);
    emm42SetSpeed(motorB, FORWARD, 0, 100);
    emm42SetSpeed(motorC, FORWARD, 0, 100);
    emm42SetSpeed(motorD, FORWARD, 0, 100);
    return SUCCESS;
}


/**
  * @brief emm42电机位置控制
  * @note 弃用
  * @param motor： emm42电机信息结构体 aimEncoder： 目标编码器的值
  * @retval 成功标志
  */
bool motorTurnToTarget(emm42_motor motor, uint16_t aimEncoder) {
    bool direction;
    uint32_t pulse;
    if (motor.uart == motorA.uart) {
        transformEncoder2Pulse(aimEncoder, m1_encoder, &direction, &pulse);
//        UART_printf("motorA: aim:%d, di:%d, pulse:%d", aimEncoder, direction, pulse);
    }
    else if (motor.uart == motorB.uart) {
        transformEncoder2Pulse(aimEncoder, m2_encoder, &direction, &pulse);
//        UART_printf("motorB: aim:%d, di:%d, pulse:%d", aimEncoder, direction, pulse);
    }
    else {
        UART_printf("motorTurnToTarget: motor not found!\r\n");
        return ERROR;
    }
    emm42SetPosition(motor, direction, 10, 10, pulse, false);

    return SUCCESS;
}

int limit(int data, int rangeMax, bool allowNeg) {
    data = data > rangeMax ? rangeMax : data;
    if (allowNeg) {
        data = data < -rangeMax ? -rangeMax : data;
    }
    else {
        data = data < 0 ? 0 : data;
    }
    return data;
}

bool upMotorsSpeedControl() {
    static int lastA, lastB;
//    A = limit(A, 40, true);
//    B = limit(B, 40, true);

    if (A != lastA) {
        lastA = A;
        if (A >= 0) {
            emm42SetSpeed(motorA, FORWARD, A, 1);
        }
        else {
            emm42SetSpeed(motorA, BACK, -A, 1);
        }
    }

    if (B != lastB) {
        lastB = B;
        if (B >= 0) {
            emm42SetSpeed(motorB, FORWARD, B, 1);
        }
        else {
            emm42SetSpeed(motorB, BACK, -B, 1);
        }
    }

    return SUCCESS;
}

bool downMotorsSpeedControl() {
    static int lastC, lastD;
//    C = limit(C, 40, true);
//    D = limit(D, 40, true);

    if (C != lastC) {
        lastC = C;
        if (C >= 0) {
            emm42SetSpeed(motorC, FORWARD, C, 1);
        }
        else {
            emm42SetSpeed(motorC, BACK, -C, 1);
        }
    }

    if (D != lastD) {
        lastD = D;
        if (D >= 0) {
            emm42SetSpeed(motorD, FORWARD, D, 1);
        }
        else {
            emm42SetSpeed(motorD, BACK, -D, 1);
        }
    }

    return SUCCESS;
}
