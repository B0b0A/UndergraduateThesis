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

#include "main.h"
#include "esp_wifi.h"
#include "esp_http_client.h"
#include "esp_tls.h"

#define WIFI_LIST_NUM   10
#define WEBSITE "http://website12341234.000webhostapp.com/index.php?"

extern wifi_config_t sta_config;
extern wifi_config_t ap_config;
extern const int CONNECTED_BIT;
extern bool gl_sta_connected;
extern uint8_t gl_sta_bssid[6];
extern uint8_t gl_sta_ssid[32];
extern int gl_sta_ssid_len;

void init_WiFi();
void init_http_client();
void post_data(char *data);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_WIFI_H_ */
