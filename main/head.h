#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "esp_netif_ip_addr.h"

/* wifi conf */
#define ESP_WIFI_SSID      "test_wifi"
#define ESP_WIFI_PASS      "test1234"
#define ESP_WIFI_CHANNEL	0  /**< channel of target AP. Set to 1~13 to scan starting from the specified channel before connecting to AP. If the channel of AP is unknown, set it to 0.*/
#define ESP_MAXIMUM_RETRY  5
void wifi_init_sta(void);

/*ip config */
#define HOST_IP_ADDR "10.42.0.1"
#define PORT 80
#define COMM_START "start"

void tcp_client_task(void *pvParameters);
