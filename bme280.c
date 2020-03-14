/*
 * bme280.c
 *
 *  Created on: Mar 13, 2020
 *      Author: Bojan Aleksovski
 */

#include "bme280.h"


// Initialize and set-up the sensor
uint8_t bme280_init(bme280_config_t *foo)
{
    // Set-up the I2C interface
    i2c_config_t config;
    config.mode = I2C_MODE_MASTER;
    config.scl_io_num = foo->scl_pin;
    config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    config.sda_io_num = foo->sda_pin;
    config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    config.slave.addr_10bit_en = 0;
 //   config.slave.slave_addr = I2CSlaveAddressW;               /* Address of BME280 */
    config.master.clk_speed = 100000;
    i2c_param_config(foo->i2c_interface, &config);
    i2c_driver_install(foo->i2c_interface, config.mode, 0, 0, 0);

    // Start communication with BME280
    if (bme280_read(foo, ID_REG) != CHIP_ID)
        return 0;

    // Reset sensor
    bme280_write(foo, RESET_REG, RESET_VALUE);

    // Wait until BME280 is ready
    while (bme280_read(foo, STATUS_REG) & 0x01)
        vTaskDelay(100 / portTICK_PERIOD_MS);

    // Read comp data
    bme280_read_compdata(foo);
    	vTaskDelay(100 / portTICK_PERIOD_MS);

    return 1;
}


	// Read data from BME280 register
uint8_t bme280_read(bme280_config_t *foo, uint8_t reg)
{
    uint8_t val;

    i2c_cmd_handle_t cmd;
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, I2CSlaveAddressW, 1);
    i2c_master_write_byte(cmd, reg, 1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(foo->i2c_interface, cmd, 1000);
    i2c_cmd_link_delete(cmd);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, I2CSlaveAddressR, 1);
    i2c_master_read_byte(cmd, &val, 1);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(foo->i2c_interface, cmd, 1000);
    i2c_cmd_link_delete(cmd);

    return val;
}


// Write data to BME280 register
void bme280_write(bme280_config_t *foo, uint8_t reg, uint8_t val)
{
i2c_cmd_handle_t cmd = i2c_cmd_link_create();
i2c_master_start(cmd);
i2c_master_write_byte(cmd, I2CSlaveAddressW, 1);
i2c_master_write_byte(cmd, reg, 1);
i2c_master_write_byte(cmd, val, 1);
i2c_master_stop(cmd);
i2c_master_cmd_begin(foo->i2c_interface, cmd, 1000);
i2c_cmd_link_delete(cmd);
}


// Set (over)sampling - check datasheet for different settigs
void bme280_set_oversampling(bme280_config_t *foo)
{
bme280_write(foo, CTRL_HUM_REG, 0x01); // Humidity x1 oversampling
bme280_write(foo, CTRL_MEAS_REG, 0x25); // Temperature and Pressure x1 oversampling and Forced mode
bme280_write(foo, CONFIG_REG, 0x00); // filter is off
}



// Read compensation data
void bme280_read_compdata(bme280_config_t *foo)
{
foo->dig_T1 = (bme280_read(foo, 0x89) << 8) + bme280_read(foo, 0x88);
foo->dig_T2 = (bme280_read(foo, 0x8B) << 8) + bme280_read(foo, 0x8A);
foo->dig_T3 = (bme280_read(foo, 0x8D) << 8) + bme280_read(foo, 0x8C);
foo->dig_P1 = (bme280_read(foo, 0x8F) << 8) + bme280_read(foo, 0x8E);
foo->dig_P2 = (bme280_read(foo, 0x91) << 8) + bme280_read(foo, 0x90);
foo->dig_P3 = (bme280_read(foo, 0x93) << 8) + bme280_read(foo, 0x92);
foo->dig_P4 = (bme280_read(foo, 0x95) << 8) + bme280_read(foo, 0x94);
foo->dig_P5 = (bme280_read(foo, 0x97) << 8) + bme280_read(foo, 0x96);
foo->dig_P6 = (bme280_read(foo, 0x99) << 8) + bme280_read(foo, 0x98);
foo->dig_P7 = (bme280_read(foo, 0x9B) << 8) + bme280_read(foo, 0x9A);
foo->dig_P8 = (bme280_read(foo, 0x9D) << 8) + bme280_read(foo, 0x9C);
foo->dig_P9 = (bme280_read(foo, 0x9F) << 8) + bme280_read(foo, 0x9E);
foo->dig_H1 = bme280_read(foo, 0xA1);
foo->dig_H2 = (bme280_read(foo, 0xE2) << 8) + bme280_read(foo, 0xE1);
foo->dig_H3 = bme280_read(foo, 0xE3);
foo->dig_H4 = (bme280_read(foo, 0xE4) << 4) | (bme280_read(foo, 0xE5) & 0x0F);
foo->dig_H5 = (bme280_read(foo, 0xE6) << 4) | (bme280_read(foo, 0xE5) >> 4);
foo->dig_H6 = bme280_read(foo, 0xE7);
}


	// Read the measured and uncompensated data in burst as recommended
void bme280_read_measured(bme280_config_t *foo)
{
    foo->adc_P = (bme280_read(foo, PRES_DATA1_REG) << 12);
	foo->adc_P += (bme280_read(foo, PRES_DATA2_REG) << 4);
	foo->adc_P += (bme280_read(foo, PRES_DATA3_REG) >> 4) & 0x0F;
    foo->adc_T = (bme280_read(foo, TEMP_DATA1_REG) << 12);
    foo->adc_T += (bme280_read(foo, TEMP_DATA2_REG) << 4);
    foo->adc_T += (bme280_read(foo, TEMP_DATA3_REG) >> 4) & 0x0F;
    foo->adc_H = (bme280_read(foo, HUM_DATA1_REG) << 8);
    foo->adc_H += bme280_read(foo, HUM_DATA2_REG);
}


	// Compensate and calculate according to vendor and return in DegC
float bme280_calc_temp(bme280_config_t *foo)
{
    int32_t var1, var2, T;

    var1 = ((((foo->adc_T >> 3) - ((int32_t) foo->dig_T1 << 1))) * ((int32_t) foo->dig_T2)) >> 11 ;
    var2 = (((((foo->adc_T >> 4) -((int32_t) foo->dig_T1)) * ((foo->adc_T >> 4) - ((int32_t) foo->dig_T1))) >> 12) * ((int32_t) foo->dig_T3)) >> 14;
    foo->t_fine = var1 + var2;
    T = (foo->t_fine * 5 + 128) >> 8;

    return (float) T / 100.0;
}


	// Compensate and calculate according to vendor and return in hPa
float bme280_calc_pressure(bme280_config_t *foo)
{
    int64_t var1, var2, p;

    var1 = ((int64_t) foo->t_fine) - 128000;
    var2 = var1 * var1 * (int64_t) foo->dig_P6;
    var2 = var2 + ((var1 * (int64_t) foo->dig_P5) << 17);
    var2 = var2 + (((int64_t) foo->dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t) foo->dig_P3) >> 8) +  ((var1 * (int64_t) foo->dig_P2) << 12) ;
    var1 = (((((int64_t) 1) << 47) + var1)) * ((int64_t) foo->dig_P1) >> 33;
    if (var1 == 0)       // avoid division by zero
        return 0;
    p = 1048576 - foo->adc_P;
    p = (((p << 31) - var2)*3125) / var1;
    var1 = (((int64_t) foo->dig_P9) * (p >> 13) *(p >> 13)) >> 25;
    var2 = (((int64_t) foo->dig_P8) * p) >> 19;
    p = (uint32_t) (((p + var1 + var2) >> 8) + (((int64_t) foo->dig_P7) << 4));

    return ((float) p / 256.0) / 100.0;
}


	// Compensate and calculate according to vendor and return in %RH
float bme280_calc_humidity(bme280_config_t *foo)
{
    int32_t v_x1_u32r;

    v_x1_u32r = (foo->t_fine - ((int32_t) 76800));
    v_x1_u32r = (((((foo->adc_H << 14) - (((int32_t) foo->dig_H4) << 20)-(((int32_t) foo->dig_H5) * v_x1_u32r)) + ((int32_t) 16384)) >> 15)
            * (((((((v_x1_u32r * ((int32_t) foo->dig_H6)) >> 10) * (((v_x1_u32r * ((int32_t) foo->dig_H3)) >> 11)
            + ((int32_t) 32768))) >> 10) + ((int32_t) 2097152)) * ((int32_t) foo->dig_H2) + 8192) >> 14));
    v_x1_u32r = (v_x1_u32r -(((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *((int32_t) foo->dig_H1))>> 4));
    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
    uint32_t h = (v_x1_u32r >> 12);

    return (float) h / 1024.0;
}


	// Measure and return measurement as struct
bme280_measurement_t bme280_make_measurement(bme280_config_t *foo)
{
	bme280_measurement_t my_measurement;
    bme280_set_oversampling(foo); // Set oversampling and more important - forced mode here
    //  in order to be able to make more measurements by calling the bme280_make_measurement function
    while ((bme280_read(foo, STATUS_REG) >> 3) & 0x1);
    bme280_read_measured(foo);
    my_measurement.temperature = bme280_calc_temp(foo);
    my_measurement.pressure = bme280_calc_pressure(foo);
    my_measurement.humidity = bme280_calc_humidity(foo);

    return my_measurement;
}


	// Print a measurement
void bme280_print_measurement(bme280_measurement_t *foo)
{
        printf("Temperature: %f *C\r\n",  foo->temperature);
        printf("Pressure:    %f hPa\r\n", foo->pressure);
        printf("Humidity:    %f %%\r\n",  foo->humidity);
        printf("\r\n");
}
