/*
 * bme280.h
 *
 *  Created on: Mar 13, 2020
 *      Author: Bojan Aleksovski
 */

#ifndef MAIN_BME280_H_
#define MAIN_BME280_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"

/*!< BME280 Registers  */
#define ID_REG                  0xD0               /*!< Contains Chip ID which is 0x60 */
#define RESET_REG               0xE0               /*!< Complete power-on-reset procedure if RESET_VALUE is written */
#define CTRL_HUM_REG            0xF2               /*!< Contains humidity data acquisition options */
#define STATUS_REG              0xF3               /*!< Contains 2 bits which indicate the status of the device */
#define CTRL_MEAS_REG           0xF4               /*!< Contains pressure and temperature data acquisition options */
#define CONFIG_REG              0xF5               /*!< Contains rate, fitler and interface options */
#define PRES_DATA1_REG          0xF7               /*!< Contains raw pressure measurement output data MSB [19:12] */
#define PRES_DATA2_REG          0xF8               /*!< LSB [11:4] */
#define PRES_DATA3_REG          0xF9               /*!< XLSB [3:0] at bit 7,6,5,4 */
#define TEMP_DATA1_REG          0xFA               /*!< Contains raw temperature measurement output data MSB [19:12] */
#define TEMP_DATA2_REG          0xFB               /*!< LSB [11:4] */
#define TEMP_DATA3_REG          0xFC               /*!< XLSB [3:0] at bit 7,6,5,4 */
#define HUM_DATA1_REG           0xFD               /*!< Contains raw temperature measurement output data MSB [15:8] */
#define HUM_DATA2_REG           0xFE               /*!< LSB [7:0]  */

/*!< BME280 given values  */
#define CHIP_ID                 0x60               /*!< LSB [7:0]  */
#define RESET_VALUE             0xB6               /*!< Put in RESET_REG in order to initiate power-on-reset procedure */
/* If pin CSB = Vdd then I2C interface is active, otherwise SPI interface */
/* If using I2C interface, then the device's 8bit address is 1110110&'SDO'&'R/W' */
#define I2CSlaveAddressW             0xEC               /*!< In order to write  */
#define I2CSlaveAddressR             0xED               /*!< In order to read  */

typedef struct {
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
    uint16_t dig_P1;
    int16_t dig_P2;
    int16_t dig_P3;
    int16_t dig_P4;
    int16_t dig_P5;
    int16_t dig_P6;
    int16_t dig_P7;
    int16_t dig_P8;
    int16_t dig_P9;
    uint8_t dig_H1;
    int16_t dig_H2;
    uint8_t dig_H3;
    int16_t dig_H4;
    int16_t dig_H5;
    int8_t dig_H6;

    int32_t adc_T;
    int32_t adc_P;
    int32_t adc_H;
    int32_t t_fine;

    gpio_num_t sda_pin;
    gpio_num_t scl_pin;
    i2c_port_t i2c_interface;
} bme280_config_t;

typedef struct {
	float temperature;
    float pressure;
    float humidity;
} bme280_measurement_t ;

uint8_t bme280_init(bme280_config_t *foo);
uint8_t bme280_read(bme280_config_t *foo, uint8_t reg);
void bme280_write(bme280_config_t *foo, uint8_t reg, uint8_t val);
void bme280_set_oversampling(bme280_config_t *foo);
void bme280_read_compdata(bme280_config_t *foo);
void bme280_read_measured(bme280_config_t *foo);
float bme280_calc_temp(bme280_config_t *foo);
float bme280_calc_pressure(bme280_config_t *foo);
float bme280_calc_humidity(bme280_config_t *foo);
bme280_measurement_t bme280_make_measurement(bme280_config_t *foo);
void bme280_print_measurement(bme280_measurement_t *foo);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_BME280_H_ */
