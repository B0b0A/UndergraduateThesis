#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "bme280.h"
#include "bluetooth.h"
#include "wifi.h"
#include "timers.h"
#include "main.h"

	// Config for BME280 sensor
    bme280_config_t my_bme280 = {
            .sda_pin = GPIO_NUM_19,
            .scl_pin = GPIO_NUM_18,
            .i2c_interface = I2C_NUM_1
    };
    // Initialize NVS (Non-violatile storage) for the Wi-Fi
void init_NVS(){
    esp_err_t ret;

    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
}

void delay_sec(int time)
{
    vTaskDelay(time * 1000 / portTICK_PERIOD_MS);
}

void app_main()
{
	bme280_measurement_t my_measurement;
	timer_idx_t timer_idx = TIMER_0;
	init_NVS();
	init_WiFi();
	init_blufi();
	tg0_timer_init(timer_idx, WITH_RELOAD, TIMER_INTERVAL1_SEC);
	bme280_init(&my_bme280);
    while(1)
    {
        xQueueReceive(timer_queue, &my_measurement, portMAX_DELAY);
    	bme280_print_measurement(&my_measurement);
}
