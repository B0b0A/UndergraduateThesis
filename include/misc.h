/*
 * misc.h
 *
 *  Created on: Mar 24, 2020
 *      Author: Bojan Aleksovski
 */

#ifndef MAIN_INCLUDE_MISC_H_
#define MAIN_INCLUDE_MISC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "nvs_flash.h"

extern bme280_config_t my_bme280;
extern bme280_measurement_t my_measurement;

char *float_to_string(float temp);
char *concat(const char *s1, const char *s2);
char *prepare_data();
void delay_sec(int time);


#ifdef __cplusplus
}
#endif

#endif /* MAIN_INCLUDE_MISC_H_ */
