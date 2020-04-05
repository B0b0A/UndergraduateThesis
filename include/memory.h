/*
 * memory.h
 *
 *  Created on: Mar 29, 2020
 *      Author: Bojan Aleksovski
 */

#ifndef MAIN_INCLUDE_MEMORY_H_
#define MAIN_INCLUDE_MEMORY_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "esp_spiffs.h"

void init_NVS();
void init_SPIFFS();


#ifdef __cplusplus
}
#endif

#endif /* MAIN_INCLUDE_MEMORY_H_ */
