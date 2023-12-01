#include "main.h"
#include "usart.h"
#include "tim.h"

#include "user.h"

int R = 99, H = 99, K = 99, O = 99;
bool F = 0;
uint32_t T1 = 0, T2 = 0;

void setup() {
    UART_printf("Program begin!\r\n");

    // 风扇初始化
    HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, GPIO_PIN_SET);

    // 舵机促初始化
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

    // 张大头智控初始化
    emm42_motor_init();
    // ABCD舵机初始化
    ABCDMotorStop();
    // 舵盘电机回零
    Emm_V5_Origin_Trigger_Return(&uartTurnMotor, 1, 0, false);
    HAL_Delay(10);
    // 抽拉板电机回零
    Emm_V5_Origin_Trigger_Return(&uartPullingPlateMotor, 2, 2, false);

    HAL_Delay(3000);

    // 清空串口缓冲器，并开启接收中断
    while (HAL_UART_Receive(&uartAGX, uart_AGX_rxbuf, 1, 1) == HAL_OK) {}
    HAL_UART_Receive_IT(&uartAGX, uart_AGX_rxbuf, 1);
    while (HAL_UART_Receive(&uartFull, uart_full_rxbuf, 1, 1) == HAL_OK) {}
    HAL_UART_Receive_IT(&uartFull, uart_full_rxbuf, 1);
    while (HAL_UART_Receive(&uartPushControlP, uart_pushP_rxbuf, 1, 1) == HAL_OK) {}
    HAL_UART_Receive_IT(&uartPushControlP, uart_pushP_rxbuf, 1);
    while (HAL_UART_Receive(&uartPushControlQ, uart_pushQ_rxbuf, 1, 1) == HAL_OK) {}
    HAL_UART_Receive_IT(&uartPushControlQ, uart_pushQ_rxbuf, 1);


    UART_printf("Init OK!\r\n");

    // 蜂鸣器哔~哔~
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
}

bool isBaseStatusNew() {
    static enum RUBBISH_TYPE last_type;
    static int last_A, last_B, last_C, last_D, last_S, last_F, last_P, last_Q;
    static uint32_t last_T1, last_T2;

    bool rst = false;

    if (rubbishType != last_type || A != last_A || B != last_B || C != last_C || D != last_D || S != last_S ||
        F != last_F || P != last_P || Q != last_Q) {
        rst = true;
    }

    if (T1 != last_T1 || T2 != last_T2) {
        rst = true;
    }

    last_type = rubbishType;
    last_A = A;
    last_B = B;
    last_C = C;
    last_D = D;
    last_S = S;
    last_F = F;
    last_P = P;
    last_Q = Q;
    last_T1 = T1;
    last_T2 = T2;

    return rst;
}

void loop() {
    upMotorsSpeedControl();
    downMotorsSpeedControl();
    turnMotorControl();
    updatePStatus();
    updateQStatus();

    if (HAL_GetTick() - T1 > 200 && HAL_GetTick() - T2 > 200) {
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
    }

    // 定时串口打印
    static uint32_t last_print_t;
    if (isBaseStatusNew() || HAL_GetTick() - last_print_t > 100) {
        int t1 = (int) (HAL_GetTick() - T1);
        if (t1 > 10000) {
            t1 = 10000;
        }
        int t2 = (int) (HAL_GetTick() - T2);
        if (t2 > 10000) {
            t2 = 10000;
        }
        UART_printf("@%s %dA %dB %dC %dD %dS %dP %dQ %dF %dT %dt %dI %dJ %dR %dH %dK %dO\r\n",
                    rubbishType2Str[rubbishType], A, B, C, D, S, P, Q, F, t1, t2, I, J, R, H, K, O);
        last_print_t = HAL_GetTick();
    }
}
