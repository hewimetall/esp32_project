#include "head.h"
static const char *TAG = "main";

void print_date(void){
	char buff[LEN_QUEUE_BUFF];
	EventBits_t bits;
	for (;;) {
		bits = xEventGroupWaitBits(status_group,
		IP_RECV_OK,
		pdFALSE,
		pdFALSE,
		portMAX_DELAY);
		if (socket_r_date != NULL) {
			ESP_LOGI(TAG,"ok");
			if( xQueueReceive( socket_r_date,&( buff ),100/portTICK_PERIOD_MS))
					{
		    	  						ESP_LOGI(TAG, "RESV:%s", buff);
					}
		}
	}
	vTaskDelete(NULL);
}
void app_main(void) {
	char buff[LEN_QUEUE_BUFF];
	ESP_LOGD(TAG, "START");

//Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK(ret);
//Initialize static date
	/*Create  event group */
	s_wifi_event_group = xEventGroupCreate();
	status_group = xEventGroupCreate();
	/*Create  Queue for socket transfer*/
	socket_r_date=xQueueCreate(LEN_QUEUE_BUFF,sizeof buff);

	ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
	ret=wifi_init_sta();
	if(ret == ESP_OK){
		xTaskCreate(&tcp_client_task, "TCP_Client", 2048, NULL, 5, NULL);
	}else{
		ESP_LOGW(TAG,"FAIL wifi_init_sta");
	}
	xTaskCreate(&print_date, "TCP_print", 2048, NULL, 4, NULL);

	ESP_LOGD(TAG, "STOP");
}
