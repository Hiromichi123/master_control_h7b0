/**
  ******************************************************************************
  * @file           : oled.c
  * @author         : XuJiang
  * @brief          : None
  * @attention      : None
  * @date           : 2023/10/6
  ******************************************************************************
  */

#include "user.h"

void OledInit() {
    OLED_Init();
    OLED_Clear();

    OLED_ShowChar(0,0,'@', 16);
    OLED_ShowChar(16+32,2,'A',16);
    OLED_ShowChar(16+48+48,2,'B',16);
    OLED_ShowChar(16+32,4,'C',16);
    OLED_ShowChar(16+48+48,4,'D',16);

    OLED_ShowChar(16,6,'S',16);
    OLED_ShowChar(48,6,'P',16);
    OLED_ShowChar(128-16,6,'I',16);

    UART_printf("OLED init OK!\r\n");
}

void OledUpdate() {
    char temp[50];

    static int lastA=999, lastB=999, lastC=999, lastD=999;
    static uint8_t lastRubbish;
    static int lastS=999, lastP=999;
    static int lastPCurrent=999;

    if (lastRubbish != lastRubbishChar){
        OLED_ShowChar(16, 0, lastRubbishChar, 16);
        lastRubbish = lastRubbishChar;
    }


    if (lastA != A){
        sprintf(temp, "%3d", A);
        OLED_ShowString(0, 2, (uint8_t *) temp, 16);
        lastA = A;
    }

    if (lastB != B){
        sprintf(temp, "%3d", B);
        OLED_ShowString(48+16, 2, (uint8_t *) temp, 16);
        lastB = B;
    }

    if (lastC != C){
        sprintf(temp, "%3d", C);
        OLED_ShowString(0, 4, (uint8_t *) temp, 16);
        lastC = C;
    }

    if (lastD != D){
        sprintf(temp, "%3d", D);
        OLED_ShowString(48+16, 4, (uint8_t *) temp, 16);
        lastD = D;
    }

    if(lastS != S){
        OLED_ShowChar(0,6,S+'0',16);
        lastS = S;
    }

    if(lastP != P_g030){
        OLED_ShowChar(32, 6, P_g030 + '0', 16);
    }

    OLED_ShowNum(64, 6, I, 4, 16);
}
