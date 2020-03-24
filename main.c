/*
 * main.c
 *
 *  Created on: Mar 13, 2020
 *      Author: Bojan Aleksovski
 */

#include "main.h"

void app_main()
{
	timer_queue = xQueueCreate(1, sizeof(bool));
	gpio_evt_queue = xQueueCreate(1, sizeof(bool));

	init_NVS();
	init_WiFi();
	bme280_init(&my_bme280);
	init_bt_button();

	xTaskCreate(main_task, "main_task", 2048, NULL, 5, NULL);
	xTaskCreate(timer_turn_off_bt_task, "timer_bt_task", 2048, NULL, 5, NULL);
	xTaskCreate(bt_button_pressed_task, "bt_button_pressed_task", 2048, NULL, 5, NULL);
	xTaskCreate(http_send_data_task, "http_send_data_task", 2048, NULL, 5, NULL);
}
