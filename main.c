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

bme280_config_t my_bme280 = {
        .sda_pin = GPIO_NUM_19,
        .scl_pin = GPIO_NUM_18,
        .i2c_interface = I2C_NUM_1
};

void init_NVS(){
    esp_err_t ret;

    // Initialize NVS
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

static void main_task(void *arg){
	while(1){
	bme280_measurement_t my_measurement;
	my_measurement = bme280_make_measurement(&my_bme280);
    bme280_print_measurement(&my_measurement);
    delay_sec(5);
	}
}

static void timer_bt_task(void *arg){
	while(1)
	{
	bool evt = 0;
	xQueueReceive(timer_queue, &evt, portMAX_DELAY);
	if (evt)
		{
		deinit_blufi();
		deinitTimer(TIMER_GROUP_0, TIMER_0);
		}
	}
}

void app_main()
{
	timer_queue = xQueueCreate(1, sizeof(bool));

	init_NVS();
	init_WiFi();
	init_blufi();
	bme280_init(&my_bme280);
	initTimerGroup0(TIMER_0, TEST_WITH_RELOAD, TIMER_INTERVAL0_SEC);

	xTaskCreate(main_task, "main_task", 2048, NULL, 5, NULL);
	xTaskCreate(timer_bt_task, "timer_bt_task", 2048, NULL, 5, NULL);

}
