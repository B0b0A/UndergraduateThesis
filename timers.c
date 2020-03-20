/*
 * timers.c
 *
 *  Created on: Mar 20, 2020
 *      Author: Bojan Aleksovski
 */

#include <stdio.h>
#include "esp_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/periph_ctrl.h"
#include "driver/timer.h"
#include "main.h"
#include "timers.h"

xQueueHandle timer_queue;

static void IRAM_ATTR timer_group0_isr(void *para)
{
    timer_spinlock_take(TIMER_GROUP_0);
    timer_idx_t timer_idx = (int) para;

	bme280_measurement_t my_measurement;

    if(bme280_init(&my_bme280) == 0)
    	printf("Error, no connection to sensor");
    else
    {
        my_measurement = bme280_make_measurement(&my_bme280);
	xQueueSendFromISR(timer_queue, &my_measurement, NULL);
    }

    // Clear the interrupt
    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, timer_idx);
    // After the alarm has been triggered, we enable it again so it gets triggered again
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, timer_idx);

    // Send event data back to the main program task
    timer_spinlock_give(TIMER_GROUP_0);
}

void tg0_timer_init(timer_idx_t timer_idx, bool auto_reload, double timer_interval_sec)
{
	timer_config_t config;
	config.divider = TIMER_DIVIDER;
	config.counter_dir = TIMER_COUNT_UP;
	config.counter_en = TIMER_PAUSE;
	config.alarm_en = TIMER_ALARM_EN;
	config.intr_type = TIMER_INTR_LEVEL;
	config.auto_reload = auto_reload;
	timer_init(TIMER_GROUP_0, timer_idx, &config);
	timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0);
	timer_set_alarm_value(TIMER_GROUP_0, timer_idx, timer_interval_sec * TIMER_SCALE);
	timer_enable_intr(TIMER_GROUP_0, timer_idx);
	timer_isr_register(TIMER_GROUP_0, timer_idx, timer_group0_isr, (void *) timer_idx, ESP_INTR_FLAG_IRAM, NULL);
	timer_start(TIMER_GROUP_0, timer_idx);
}


