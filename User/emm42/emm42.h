//#define EMM42_DEBUG
//#define TM4
#define STM32


#ifndef EMM42_H
#define EMM42_H

#include "stdbool.h"
#include "stdint.h"

typedef unsigned char UChar;
#define FORWARD 1
#define BACK 0


#ifdef EMM42_DEBUG
#define UART_HandleTypeDef int
UChar HexToAscii(UChar c);
int printChar(UChar hexData[], int commandLength);
#endif

#ifdef TM4
#define SUCCESS 1
#define ERROR 0
typedef uint32_t UART_HandleTypeDef;
#include "../main.h"
#endif

#ifdef STM32
#include "usart.h"
#endif

typedef struct {
    UART_HandleTypeDef *uart;
    UChar address;
    UChar checkByte;
} emm42_motor;

typedef struct {
    UChar commandByte;
    UChar params[10];
    unsigned int paramsLength;
} command;

int emmMotorUartInit(emm42_motor *motor, UART_HandleTypeDef *uart, UChar address, UChar checkByte);
int emmMotorSend(emm42_motor motor, command sendCommand);

int emm42ReadEncoder(emm42_motor motor, uint16_t* dataOfEncoder);
int emm42ReadPosition(emm42_motor motor, int32_t* dataOfPosition);

int emm42Enable(emm42_motor motor, bool enable);
int emm42SetSpeed(emm42_motor motor, bool direction, uint16_t speed, uint8_t acceleration);
int emm42SetPosition(emm42_motor motor, bool direction, uint16_t speed, uint8_t acceleration, uint32_t pulse, bool wait);
#endif //EMM42_H