/*
 * tasks.c
 *
 *  Created on: Mar 24, 2020
 *      Author: Bojan Aleksovski
 */

#include "tasks.h"

void main_task(void *arg){
	while(1){
	my_measurement = bme280_make_measurement(&my_bme280);
    bme280_print_measurement(&my_measurement);
    delay_sec(500);
	}
}

void timer_turn_off_bt_task(void *arg){
	while(1){
	bool evt = 0;
	xQueueReceive(timer_queue, &evt, portMAX_DELAY);
	if(evt){
		deinit_blufi();
		deinitTimer(TIMER_GROUP_0, TIMER_0);
		bt_on = 0;
		}
	}
}

void bt_button_pressed_task(void* arg){
    while(1){
        xQueueReceive(gpio_evt_queue, &bt_on, portMAX_DELAY);
        if(bt_on){
        	init_blufi();
        	initTimerGroup0(TIMER_0, TEST_WITH_RELOAD, TIMER_INTERVAL0_SEC);
        }
    }
}

void http_send_data_task(void* arg){
    while(1){
    	init_http_client();
    	post_data(prepare_data());
    	delay_sec(300);
        }
    }
