/*
 * main.h
 *
 *  Created on: Mar 20, 2020
 *      Author: Eli
 */

#ifndef MAIN_INCLUDE_MAIN_H_
#define MAIN_INCLUDE_MAIN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "bme280.h"
#include "bluetooth.h"
#include "wifi.h"
#include "timers.h"

extern  bme280_config_t my_bme280;

#ifdef __cplusplus
}
#endif

#endif /* MAIN_INCLUDE_MAIN_H_ */
