#include "head.h"
static const char *TAG = "main";


void app_main(void) {
	ESP_LOGD(TAG, "ESP MAIN START");

//Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);

	ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
	wifi_init_sta();
	ESP_LOGD(TAG, "STOP MAIN");

	xTaskCreate(&tcp_client_task, "TCP_Client", 4096, NULL, 5, NULL);
}
