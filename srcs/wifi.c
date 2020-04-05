/*
 * wifi.c
 *
 *  Created on: Mar 19, 2020
 *      Author: Bojan Aleksovski
 */

#include "wifi.h"

wifi_config_t sta_config;
wifi_config_t ap_config;

// Http client
esp_http_client_handle_t client;
static const char *TAG = "HTTP_CLIENT";

/* FreeRTOS event group to signal when we are connected & ready to make a request */
EventGroupHandle_t wifi_event_group;

/* The event group allows multiple bits for each event,
   but we only care about one event - are we connected
   to the AP with an IP? */
const int CONNECTED_BIT = BIT0;

/* store the station info for send back to phone */
bool gl_sta_connected = false;
uint8_t gl_sta_bssid[6];
uint8_t gl_sta_ssid[32];
int gl_sta_ssid_len;

static esp_err_t net_event_handler(void *ctx, system_event_t *event)
{
    wifi_mode_t mode;

    switch (event->event_id) {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;
    case SYSTEM_EVENT_STA_GOT_IP: {
        esp_blufi_extra_info_t info;

        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        esp_wifi_get_mode(&mode);

        memset(&info, 0, sizeof(esp_blufi_extra_info_t));
        memcpy(info.sta_bssid, gl_sta_bssid, 6);
        info.sta_bssid_set = true;
        info.sta_ssid = gl_sta_ssid;
        info.sta_ssid_len = gl_sta_ssid_len;
        esp_blufi_send_wifi_conn_report(mode, ESP_BLUFI_STA_CONN_SUCCESS, 0, &info);
        break;
    }
    case SYSTEM_EVENT_STA_CONNECTED:
        gl_sta_connected = true;
        memcpy(gl_sta_bssid, event->event_info.connected.bssid, 6);
        memcpy(gl_sta_ssid, event->event_info.connected.ssid, event->event_info.connected.ssid_len);
        gl_sta_ssid_len = event->event_info.connected.ssid_len;
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* This is a workaround as ESP32 WiFi libs don't currently
           auto-reassociate. */
        gl_sta_connected = false;
        memset(gl_sta_ssid, 0, 32);
        memset(gl_sta_bssid, 0, 6);
        gl_sta_ssid_len = 0;
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
        break;
    case SYSTEM_EVENT_SCAN_DONE: {
        uint16_t apCount = 0;
        esp_wifi_scan_get_ap_num(&apCount);
        if (apCount == 0) {
            BLUFI_INFO("No Access Points found");
            break;
        }
        wifi_ap_record_t *ap_list = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * apCount);
        if (!ap_list) {
            BLUFI_ERROR("malloc error, ap_list is NULL");
            break;
        }
        ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&apCount, ap_list));
        esp_blufi_ap_record_t * blufi_ap_list = (esp_blufi_ap_record_t *)malloc(apCount * sizeof(esp_blufi_ap_record_t));
        if (!blufi_ap_list) {
            if (ap_list) {
                free(ap_list);
            }
            BLUFI_ERROR("malloc error, blufi_ap_list is NULL");
            break;
        }
        for (int i = 0; i < apCount; ++i)
        {
            blufi_ap_list[i].rssi = ap_list[i].rssi;
            memcpy(blufi_ap_list[i].ssid, ap_list[i].ssid, sizeof(ap_list[i].ssid));
        }
        esp_blufi_send_wifi_list(apCount, blufi_ap_list);
        esp_wifi_scan_stop();
        free(ap_list);
        free(blufi_ap_list);
        break;
    }
    default:
        break;
    }
    return ESP_OK;
}

void init_WiFi()
{
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK( esp_event_loop_init(net_event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_FLASH) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK( esp_wifi_start() );
}

esp_err_t _http_event_handler(esp_http_client_event_t *evt){
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // Write out data
                 printf("%.*s", evt->data_len, (char*)evt->data);
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error(evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            break;
    }
    return ESP_OK;
}

void init_http_client(){
	    esp_http_client_config_t config = {
	        .url = "http://website12341234.000webhostapp.com/index.php",
	        .event_handler = _http_event_handler,
	    };
	    client = esp_http_client_init(&config);
}

void post_data(char *data){
		esp_err_t err;
	    // POST
	    esp_http_client_set_url(client,data);
	    esp_http_client_set_method(client, HTTP_METHOD_GET);
	    free(data);
	    err = esp_http_client_perform(client);
	    if (err == ESP_OK) {
	        ESP_LOGI(TAG, "HTTP POST Status = %d, content_length = %d",
	                esp_http_client_get_status_code(client),
	                esp_http_client_get_content_length(client));
	        		esp_http_client_cleanup(client);
	    } else {
	        ESP_LOGE(TAG, "HTTP POST request failed: %s", esp_err_to_name(err));
	    }
}
