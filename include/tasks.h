/*
 * tasks.h
 *
 *  Created on: Mar 24, 2020
 *      Author: Bojan Aleksovski
 */

#ifndef MAIN_INCLUDE_TASKS_H_
#define MAIN_INCLUDE_TASKS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "main.h"

void main_task(void *arg);
void timer_turn_off_bt_task(void *arg);
void bt_button_pressed_task(void* arg);
void http_send_data_task(void* arg);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_INCLUDE_TASKS_H_ */
