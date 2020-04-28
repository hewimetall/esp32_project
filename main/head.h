#ifndef MAIN_HEADER
#define MAIN_HEADER
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

/* General config */
#define LEN_QUEUE_BUFF 4
QueueHandle_t socket_r_date;
/* FreeRTOS event group to signal event*/
EventGroupHandle_t status_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we get when wifi init start successful
 * - we set bit when socket connect successful
 * - we receiving data via socket */
#define WIFI_STATUS_OK		BIT0
#define IP_SOCKET_OK        BIT1
#define IP_RECV_OK			BIT2


/* wifi conf */
/* FreeRTOS event group to signal when we are connected*/
EventGroupHandle_t s_wifi_event_group;
/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries*/
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

#define ESP_WIFI_SSID      "test_wifi"
#define ESP_WIFI_PASS      "test1234"
#define ESP_WIFI_CHANNEL	0  /**< channel of target AP. Set to 1~13 to scan starting from the specified channel before connecting to AP. If the channel of AP is unknown, set it to 0.*/
#define ESP_MAXIMUM_RETRY  5
esp_err_t wifi_init_sta(void);


/*ip config */
#define HOST_IP_ADDR "10.42.0.1"
#define PORT 80
#define COMM_START "start"
#define MAX_DATE_LENS 60
void tcp_client_task(void *pvParameters);

#endif
