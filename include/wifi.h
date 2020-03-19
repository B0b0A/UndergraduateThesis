/*
 * WiFi.h
 *
 *  Created on: Mar 19, 2020
 *      Author: Bojan Aleksovski
 */

#ifndef MAIN_WIFI_H_
#define MAIN_WIFI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_blufi_api.h"
#include "esp_bt_defs.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

extern wifi_config_t sta_config;
extern wifi_config_t ap_config;
extern const int CONNECTED_BIT;
extern bool gl_sta_connected;
extern uint8_t gl_sta_bssid[6];
extern uint8_t gl_sta_ssid[32];
extern int gl_sta_ssid_len;

void init_WiFi();

#ifdef __cplusplus
}
#endif

#endif /* MAIN_WIFI_H_ */
