/*
 * main.h
 *
 *  Created on: Mar 20, 2020
 *      Author: Bojan Aleksovski
 */

#ifndef MAIN_INCLUDE_MAIN_H_
#define MAIN_INCLUDE_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_types.h"
#include "esp_log.h"

#include "bme280.h"
#include "bluetooth.h"
#include "gpio_intr.h"
#include "wifi.h"
#include "timers.h"
#include "tasks.h"
#include "misc.h"
#include "tftspi.h"
#include "tft.h"
#include "memory.h"


#ifdef __cplusplus
}
#endif

#endif /* MAIN_INCLUDE_MAIN_H_ */
