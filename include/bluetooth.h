/*
 * bluetooth.h
 *
 *  Created on: Mar 19, 2020
 *      Author: Bojan Aleksovski
 */

#ifndef MAIN_BLUETOOTH_H_
#define MAIN_BLUETOOTH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "esp_bt.h"
#include "esp_blufi_api.h"
#include "esp_bt_defs.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"

#include "mbedtls/aes.h"
#include "mbedtls/dhm.h"
#include "mbedtls/md5.h"
#include "esp32/rom/crc.h"

#define BLUETOOTH_TAG "BLUETOOTH"
#define BLUFI_INFO(fmt, ...)   ESP_LOGI(BLUETOOTH_TAG, fmt, ##__VA_ARGS__)
#define BLUFI_ERROR(fmt, ...)  ESP_LOGE(BLUETOOTH_TAG, fmt, ##__VA_ARGS__)
#define BLUFI_DEVICE_NAME            "BLUFI_DEVICE"

void init_blufi();
void deinit_blufi();

void blufi_dh_negotiate_data_handler(uint8_t *data, int len, uint8_t **output_data, int *output_len, bool *need_free);
int blufi_aes_encrypt(uint8_t iv8, uint8_t *crypt_data, int crypt_len);
int blufi_aes_decrypt(uint8_t iv8, uint8_t *crypt_data, int crypt_len);
uint16_t blufi_crc_checksum(uint8_t iv8, uint8_t *data, int len);

int blufi_security_init(void);
void blufi_security_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_BLUETOOTH_H_ */
