# 张大头智控步进电机库函数使用说明
**2023年5月17日  蒋旭**

## 前言
本库适用于张大头智控闭环42步进电机，默认支持STM32与TM4，其他单片机需要自己完成串口发送及接受功能的适配

## 移植方法
1. 在emm42.h文件开头，定义需要的单片机类型
2. 在emm42.h文件23-32行，include单片机需要的串口库文件，例如stm32单片机的usart.h文件
3. 在main.h引入emm42.h头文件，例如    #include "../emm42/emm42.h"
4. 串口参数说明：stm32单片机为HAL库串口指针，例如&huart1; TM4单片机为UARTx_BASE;
当使用TM4单片机时，需要自行定义   UART_HandleTypeDef emm42MotorUart = UARTx_BASE;
之后，使用&UARTx_BASE

## 使用其他单片机
1. 修改emmMotorSend()函数中的串口发送方法
信息16进制存储在hexData[]数组,信息的长度存储在commandLength
需要完成：清空串口中断缓冲区，赋予一定延时，发送16进制数据到串口
2. 修改emm42_read.c文件中的两个串口数据读取方法
数据采用查询方式读取，需要读取dataBackLength个字节到dataBack数组
