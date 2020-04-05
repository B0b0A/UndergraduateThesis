/*
 * gpio_intr.h
 *
 *  Created on: Mar 22, 2020
 *      Author: Bojan Aleksovski
 */

#ifndef MAIN_INCLUDE_GPIO_INTR_H_
#define MAIN_INCLUDE_GPIO_INTR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "driver/gpio.h"

#define GPIO_BT_BTN     35
#define GPIO_BT_BTN_MASK (1ULL<<GPIO_BT_BTN)
#define ESP_INTR_FLAG_DEFAULT 0

extern xQueueHandle gpio_evt_queue;
extern bool bt_on;

void init_bt_button();

#ifdef __cplusplus
}
#endif

#endif /* MAIN_INCLUDE_GPIO_INTR_H_ */
