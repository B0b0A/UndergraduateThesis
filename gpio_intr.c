/*
 * gpio_inter.c
 *
 *  Created on: Mar 22, 2020
 *      Author: Bojan Aleksovski
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "gpio_intr.h"

xQueueHandle gpio_evt_queue;
bool bt_on = 0;

void IRAM_ATTR bt_btn_isr_handler(void* arg){
	if(!bt_on){
    bt_on = 1;
    xQueueSendFromISR(gpio_evt_queue, &bt_on, NULL);
	}
}


void init_bt_button(){
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = GPIO_BT_BTN_MASK;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    gpio_isr_handler_add(GPIO_BT_BTN, bt_btn_isr_handler, NULL);

}


