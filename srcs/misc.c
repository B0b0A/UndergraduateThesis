/*
 * misc.c
 *
 *  Created on: Mar 24, 2020
 *      Author: Bojan Aleksovski
 */

#include "misc.h"

bme280_config_t my_bme280 = {
        .sda_pin = GPIO_NUM_26,
        .scl_pin = GPIO_NUM_27,
        .i2c_interface = I2C_NUM_1
};

bme280_measurement_t my_measurement = {
		.temperature = 0,
		.humidity = 0,
		.pressure = 0
};

char *float_to_string(float temp){
    int length = snprintf( NULL, 0, "%.2f", temp ) + 1;
    char* mystr = malloc(length);
    snprintf( mystr, length + 1, "%.2f", temp );
    return mystr;
}
char * concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char *prepare_data(){
	char* mystr1 = float_to_string(my_measurement.temperature);
	char* mystr2 = float_to_string(my_measurement.humidity);
	char* mystr3 = float_to_string(my_measurement.pressure);
	char* an = "&";
	char* s1 = concat("temp=", mystr1);
	free(mystr1);
	char* s2 = concat("pres=", mystr2);
	free(mystr2);
	char* s3 = concat("hum=", mystr3);
	free(mystr3);
	char* s1_ = concat(s1, an);
	free(s1);
	char* s2_ = concat(s1_,s2);
	free(s1_);
	free(s2);
	char* s2__ = concat(s2_, an);
	free(s2_);
	char* s4 = concat(s2__,s3);
	free(s2__);
	free(s3);
	char* data = concat(WEBSITE,s4);
	return data;
}


void delay_sec(int time)
{
    vTaskDelay(time * 1000 / portTICK_PERIOD_MS);
}
