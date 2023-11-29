#include "main.h"
#include "usart.h"
#include "tim.h"

#include "user.h"

int mode = 0;

int R = 99, H = 99, K = 99, O = 99;

bool F = 0;
int T1 = 0, T2=0;

void runDebug(){
//    HAL_GPIO_WritePin(FAN_GPIO_Port,FAN_Pin,GPIO_PIN_RESET);
//    HAL_Delay(3000);
//    HAL_GPIO_WritePin(FAN_GPIO_Port,FAN_Pin,GPIO_PIN_SET);
//    while (1);
}

void setup() {
    UART_printf("Program begin!\r\n");

    emm42_motor_init();
    HAL_GPIO_WritePin(FAN_GPIO_Port,FAN_Pin,GPIO_PIN_SET);

    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);

    HAL_Delay(1000);

    ABCDMotorStop();
    HAL_Delay(100);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

    Emm_V5_Origin_Trigger_Return(&uartTurnMotor, 1, 0, false); // ���̵������
    HAL_Delay(100);
    Emm_V5_Origin_Trigger_Return(&uartPullingPlateMotor, 2, 2, false); // ������������
//    P = 2;
    updatePStatus();

    // OledInit();

    HAL_Delay(5000);

    HAL_TIM_Base_Start_IT(&htim16);

    while (HAL_UART_Receive(&uartDebug, uartDebugRxBuff, 10, 1) == HAL_OK) {}
    HAL_UART_Receive_IT(&uartDebug, uartDebugRxBuff, 1);
    while (HAL_UART_Receive(&uartPushControl, uartPushRxBuff, 10, 1) == HAL_OK) {}
    HAL_UART_Receive_IT(&uartPushControl, uartPushRxBuff, 1);
    while (HAL_UART_Receive(&uartFull, uartFullRxBuff, 10, 1) == HAL_OK) {}
    HAL_UART_Receive_IT(&uartFull, uartFullRxBuff, 1);

    UART_printf("Init OK!\r\n");

    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
    HAL_Delay(500);
    HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);

    runDebug();
}

bool isBaseStatusNew() {
    static enum RUBBISH_TYPE last_type;
    static int last_A, last_B, last_C, last_D, last_S, last_P, last_F;
    bool rst;

    if (rubbishType != last_type || A != last_A || B != last_B || C != last_C || D != last_D || S != last_S ||
        P != last_P || F != last_F) {
        rst = true;
    }
    else {
        rst = false;
    }

    last_type = rubbishType;
    last_A = A;
    last_B = B;
    last_C = C;
    last_D = D;
    last_S = S;
    last_P = P;
    last_F = F;

    return rst;
}

void loop() {
//     processKey();
    upMotorsSpeedControl();
    downMotorsSpeedControl();
    turnMotorControl();
    updatePStatus();

    // OledUpdate(); // ����OLED��ʾ

    if (HAL_GetTick() - T1 > 200 && HAL_GetTick() - T2 > 200) {
//            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
    }

    // ��ʱ���ڴ�ӡ
    static uint32_t last_print_t = 0;
    if (isBaseStatusNew() || HAL_GetTick() - last_print_t > 100) {
        char msg[100] = {0};
        int t1 = HAL_GetTick() - T1;
        if (t1 > 10000) {
            t1 = 10000;
        }
        int t2 = HAL_GetTick() - T2;
        if (t2 > 10000) {
            t2 = 10000;
        }
        sprintf(msg, "@%s %dA %dB %dC %dD %dS %dP %dQ %dF %dT %dt %dI %dJ %dR %dH %dK %dO\r\n",
                rubbishType2Str[rubbishType], A, B, C, D, S, P, Q, F, t1, t2, I, J, R, H, K, O);
        UART_printf("%s", msg);
        last_print_t = HAL_GetTick();
    }
}
