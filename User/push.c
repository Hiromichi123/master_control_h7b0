/**
  ******************************************************************************
  * @file           : push.c
  * @author         : XuJiang
  * @brief          : None
  * @attention      : None
  * @date           : 2023/9/29
  ******************************************************************************
  */

#include "user.h"

int P, P_g030, P_AGX=-1;
int I, J;
int Q, Q_g030, Q_AGX=-1;

bool pullingPlateMotorControl(bool open) {
    if (!open) {
        Emm_V5_Pos_Control(&uartPullingPlateMotor, 2, 0, 200, 200, 100, true, false);
    } else {
        Emm_V5_Pos_Control(&uartPullingPlateMotor, 2, 0, 200, 200, 8800, true, false);
    }

    return SUCCESS;
}

void updatePStatus() {
    static int P_g030_last;

    bool tell_g030 = false;

    // 若是AGX收到了新数据
    if (P_AGX != -1) {
        P = P_AGX;
        tell_g030 = true; // 通知g030
        P_AGX = -1;
    }

    // 若是g030使得状态改变
    if (P_g030 != P_g030_last) {
        if (P_g030_last == 1 && P_g030 == 0) { // 自动回抽逻辑
            P = 2;
            tell_g030 = true; // 通知g030
            pullingPlateMotorControl(true);
        } else if (P_g030_last == 2 && P_g030 == 0) { // 自动关闭抽拉门
            pullingPlateMotorControl(false);
            P = P_g030;
        } else {
            P = P_g030;
        }
        P_g030_last = P_g030;
    }

    // 通知g030
    if (tell_g030) {
        if (P == 0) {
            HAL_UART_Transmit_IT(&uartPushControlP, (uint8_t *) "@0P", 3);
        } else if (P == 1) {
            HAL_UART_Transmit_IT(&uartPushControlP, (uint8_t *) "@1P", 3);
        } else if (P == 2) {
            HAL_UART_Transmit_IT(&uartPushControlP, (uint8_t *) "@2P", 3);
        }
    }
}


void updateQStatus() {
    static int Q_g030_last;

    bool tell_g030 = false;

    // 若是AGX收到了新数据
    if (Q_AGX != -1) {
        Q = Q_AGX;
        tell_g030 = true; // 通知g030
        Q_AGX = -1;
    }

    // 若是g030使得状态改变
    if (Q_g030 != Q_g030_last) {
        if (Q == 1 && Q_g030 == 0) { // 自动回抽逻辑
            Q = 2;
            tell_g030 = true; // 通知g030
        } else {
            Q = Q_g030;
        }
        Q_g030_last = Q_g030;
    }

    // 通知g030
    if (tell_g030) {
        if (Q == 0) {
            HAL_UART_Transmit_IT(&uartPushControlQ, (uint8_t *) "@0P", 3);
        } else if (Q == 1) {
            HAL_UART_Transmit_IT(&uartPushControlQ, (uint8_t *) "@1P", 3);
        } else if (Q == 2) {
            HAL_UART_Transmit_IT(&uartPushControlQ, (uint8_t *) "@2P", 3);
        }
    }
}
