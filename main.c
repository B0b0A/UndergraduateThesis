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

// does single measurement as of now

void app_main()
{
	init_NVS();
	init_WiFi();
	init_blufi();
    bme280_config_t my_bme280 = {
            .sda_pin = GPIO_NUM_19,
            .scl_pin = GPIO_NUM_18,
            .i2c_interface = I2C_NUM_1
    };
   if(bme280_init(&my_bme280) == 0)
	   printf("Error, no connection to sensor");
   else{

        bme280_measurement_t my_measurement;
        my_measurement = bme280_make_measurement(&my_bme280);
        bme280_print_measurement(&my_measurement);

   }
}
