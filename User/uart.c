#include "user.h"

int UART_printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int length;
    char buffer[128];

    length = vsnprintf(buffer, 128, fmt, ap);

    HAL_UART_Transmit(&uartDebug, (uint8_t *) buffer, length, HAL_MAX_DELAY);

    va_end(ap);
    return length;
}

uint8_t uartDebugRxBuff[10], uartPushRxBuff[10], uartFullRxBuff[10];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    static uint32_t uartDebugTempData;
    static int uartDebugTempDataPositive = 1;
    static int pushUartRxTempData;
    static int fullUartRxTempData;
    if (huart == &uartPushControl) {
        HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
        uint8_t re_buf = uartPushRxBuff[0];
        if (re_buf >= '0' && re_buf <= '9') {
            pushUartRxTempData = pushUartRxTempData * 10 + re_buf - '0';
        }
        else{
            switch (re_buf) {
                case 'P':
                    if (pushUartRxTempData == 0 || pushUartRxTempData == 1 || pushUartRxTempData == 2)
                        P_g030 = pushUartRxTempData;
                    break;
                case 'I':
                    I = pushUartRxTempData;
                    break;
                case 'Q':
                    if (pushUartRxTempData == 0 || pushUartRxTempData == 1 || pushUartRxTempData == 2)
                        Q_g030 = pushUartRxTempData;
                    break;
                case 'J':
                    J = pushUartRxTempData;
                    break;
                default:
                    break;
            }
            pushUartRxTempData = 0;
        }
        HAL_UART_Receive_IT(&uartPushControl, uartPushRxBuff, 1);
    }
    else if (huart == &uartFull) {
        HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
        uint8_t re_buf = uartFullRxBuff[0];
        if (re_buf >= '0' && re_buf <= '9') {
            fullUartRxTempData = fullUartRxTempData * 10 + re_buf - '0';
        }
        else{
            switch (re_buf) {
                case 'R':
                    R = fullUartRxTempData;
                    break;
                case 'H':
                    H = fullUartRxTempData;
                    break;
                case 'K':
                    K = fullUartRxTempData;
                    break;
                case 'O':
                    O = fullUartRxTempData;
                    break;
                default:
                    break;
            }
            fullUartRxTempData = 0;
        }
        HAL_UART_Receive_IT(&uartFull, uartFullRxBuff, 1);
    }
    else if (huart == &uartDebug) {
        HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
        uint8_t re_buf = uartDebugRxBuff[0];
        if (re_buf >= '0' && re_buf <= '9') {
            uartDebugTempData = uartDebugTempData * 10 + re_buf - '0';
        } else {
            switch (re_buf) {
                case 'S':
                    downDoorControl(uartDebugTempData);
                    break;
                case 'R':
                    turnToBin(RECYCLABLE);
                    break;
                case 'H':
                    turnToBin(HAZARDOUS);
                    break;
                case 'K':
                    turnToBin(KITCHEN);
                    break;
                case 'O':
                    turnToBin(OTHER);
                    break;
                case 'A':
                    A = uartDebugTempData * uartDebugTempDataPositive;
                    break;
                case 'B':
                    B = uartDebugTempData * uartDebugTempDataPositive;
                    break;
                case 'C':
                    C = uartDebugTempData * uartDebugTempDataPositive;
                    break;
                case 'D':
                    D = uartDebugTempData * uartDebugTempDataPositive;
                    break;
                case 'P':
                    P_AGX = uartDebugTempData;
                    break;
                case 'Q':
                    Q_AGX = uartDebugTempData;
                    break;
                case 'F':
                    F = uartDebugTempData > 0 ? 1 : 0;
                    HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, F);
                    break;
                case '-':
                    uartDebugTempData = 0;
                    uartDebugTempDataPositive = -1;

                    // ���յ�����ʱ�������u1tempPositive��Ҫֱ�ӷ���
                    HAL_UART_Receive_IT(&uartDebug, uartDebugRxBuff, 1);
                    return;

                default:
                    break;
            }
            uartDebugTempData = 0;
            uartDebugTempDataPositive = 1;
        }

        HAL_UART_Receive_IT(&uartDebug, uartDebugRxBuff, 1);
    }
}
