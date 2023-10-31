//
// Created by XuJiang on 2023/6/3.
//

#ifndef STM32H7B0_CAR_KEY_H
#define STM32H7B0_CAR_KEY_H

enum KEY_PRESS_FLAG{
    KEY_NONE_PRESS,
    KEY1_PRESS,
    KEY2_PRESS,
    KEY3_PRESS,
    KEY_USER_PRESS,
};
int getKey();
int processKey();
#endif //STM32H7B0_CAR_KEY_H
