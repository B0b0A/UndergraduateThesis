/*
 * timers.c
 *
 *  Created on: Mar 20, 2020
 *      Author: Bojan Aleksovski
 */

#include "timers.h"

xQueueHandle timer_queue;

void IRAM_ATTR timer_group0_isr(void *para)
{
    int timer_idx = (int) para;
    bool evt;

    /* Prepare basic event data
       that will be then sent back to the main program task */
    evt = 1;
    /* Clear the interrupt
       and update the alarm time for the timer with without reload */

    TIMERG0.int_clr_timers.t0 = 1;

    /* After the alarm has been triggered
      we need enable it again, so it is triggered the next time */
    TIMERG0.hw_timer[timer_idx].config.alarm_en = TIMER_ALARM_EN;

    /* Now just send the event data back to the main program task */
    xQueueSendFromISR(timer_queue, &evt, NULL);
}

void initTimerGroup0(timer_idx_t timer_idx, bool auto_reload, double timer_interval_sec){
    /* Select and initialize basic parameters of the timer */
    timer_config_t config;
    config.divider = TIMER_DIVIDER;
    config.counter_dir = TIMER_COUNT_UP;
    config.counter_en = TIMER_PAUSE;
    config.alarm_en = TIMER_ALARM_EN;
    config.intr_type = TIMER_INTR_LEVEL;
    config.auto_reload = auto_reload;
    timer_init(TIMER_GROUP_0, timer_idx, &config);

    /* Timer's counter will initially start from value below.
       Also, if auto_reload is set, this value will be automatically reload on alarm */
    timer_set_counter_value(TIMER_GROUP_0, timer_idx, 0x00000000ULL);

    /* Configure the alarm value and the interrupt on alarm. */
    timer_set_alarm_value(TIMER_GROUP_0, timer_idx, timer_interval_sec * TIMER_SCALE);
    timer_enable_intr(TIMER_GROUP_0, timer_idx);
    timer_isr_register(TIMER_GROUP_0, timer_idx, timer_group0_isr, NULL, ESP_INTR_FLAG_IRAM, NULL);

    timer_start(TIMER_GROUP_0, timer_idx);
}

void deinitTimer(timer_group_t timer_group, timer_idx_t timer_idx){
	timer_pause(timer_group, timer_idx);
	timer_disable_intr(timer_group, timer_idx);
}
