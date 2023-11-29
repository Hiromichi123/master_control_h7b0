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

int P, P_AGX=-1, P_g030;
int I,J;
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
    static int P_last, P_g030_last, Q_g030_last, Q_last;

    if (P_AGX != -1) {
        P = P_AGX;
        P_AGX = -1;
    }

    if (Q_AGX != -1) {
        Q = Q_AGX;
        Q_AGX = -1;
    }

    if (P_g030 != P_g030_last) {
        if (P == 1 && P_g030 == 0) {
            P = 2;
        } else {
            P = P_g030;
        }
        P_g030_last = P_g030;
    }

    if (Q_g030 != Q_g030_last) {
        if (Q == 1 && Q_g030 == 0) {
            Q = 2;
        } else {
            Q = Q_g030;
        }
        Q_g030_last = Q_g030;
    }

    if (P != P_last) {
        if (P == 0) {
            HAL_UART_Transmit(&uartPushControl, (uint8_t *) "@0P", 3, 5);
            pullingPlateMotorControl(false);
        } else if (P == 1) {
            HAL_UART_Transmit(&uartPushControl, (uint8_t *) "@1P", 3,5);
        } else if (P == 2) {
            pullingPlateMotorControl(true);
            HAL_UART_Transmit(&uartPushControl, (uint8_t *) "@2P", 3,5);
        }
        P_last = P;
    }

    if (Q != Q_last) {
        if (Q == 0) {
            HAL_UART_Transmit_IT(&uartPushControl, (uint8_t *) "@0Q", 3);
        } else if (Q == 1) {
            HAL_UART_Transmit_IT(&uartPushControl, (uint8_t *) "@1Q", 3);
        } else if (Q == 2) {
            HAL_UART_Transmit_IT(&uartPushControl, (uint8_t *) "@2Q", 3);
        }
        Q_last = Q;
    }
}
