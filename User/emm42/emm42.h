#ifndef EMM42_H
#define EMM42_H

#include "stdbool.h"
#include "stdint.h"
#include "usart.h"

typedef unsigned char UChar;
#define FORWARD 1
#define BACK 0

// 和emm42通信的结构体
typedef struct {
    UART_HandleTypeDef *uart;
    UChar address;
    UChar checkByte;
} emm42_motor;

// 和emm42通信的命令结构体
typedef struct {
    UChar commandByte;
    UChar params[10];
    unsigned int paramsLength;
} command;

// 初始化EMM电机的UART
// 参数:
// - motor: 指向emm42_motor结构体的指针
// - uart: 指向UART_HandleTypeDef结构体的指针
// - address: 电机的地址
// - checkByte: 通信的校验字节
// 返回值:
// - 如果成功返回0，如果发生错误返回-1
int emmMotorUartInit(emm42_motor *motor, UART_HandleTypeDef *uart, UChar address, UChar checkByte);

// 向EMM电机发送命令
// 参数:
// - motor: emm42_motor结构体
// - sendCommand: 要发送的命令
// 返回值:
// - 如果成功返回0，如果发生错误返回-1
int emmMotorSend(emm42_motor motor, command sendCommand);

// 从EMM电机读取编码器数据
// 参数:
// - motor: emm42_motor结构体
// - dataOfEncoder: 存储编码器数据的指针
// 返回值:
// - 如果成功返回0，如果发生错误返回-1
int emm42ReadEncoder(emm42_motor motor, uint16_t* dataOfEncoder);

// 从EMM电机读取位置数据
// 参数:
// - motor: emm42_motor结构体
// - dataOfPosition: 存储位置数据的指针
// 返回值:
// - 如果成功返回0，如果发生错误返回-1
int emm42ReadPosition(emm42_motor motor, int32_t* dataOfPosition);

// 启用或禁用EMM电机
// 参数:
// - motor: emm42_motor结构体
// - enable: true启用，false禁用
// 返回值:
// - 如果成功返回0，如果发生错误返回-1
int emm42Enable(emm42_motor motor, bool enable);

// 设置EMM电机的速度
// 参数:
// - motor: emm42_motor结构体
// - direction: 旋转方向（FORWARD或BACK）
// - speed: 速度值
// - acceleration: 加速度值
// 返回值:
// - 如果成功返回0，如果发生错误返回-1
int emm42SetSpeed(emm42_motor motor, bool direction, uint16_t speed, uint8_t acceleration);

// 设置EMM电机的位置
// 参数:
// - motor: emm42_motor结构体
// - direction: 运动方向（FORWARD或BACK）
// - speed: 速度值
// - acceleration: 加速度值
// - pulse: 脉冲值
// - wait: true等待完成，false立即继续
// 返回值:
// - 如果成功返回0，如果发生错误返回-1
int emm42SetPosition(emm42_motor motor, bool direction, uint16_t speed, uint8_t acceleration, uint32_t pulse, bool wait);

#endif //EMM42_H