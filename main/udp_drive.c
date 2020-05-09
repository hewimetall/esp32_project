/* BSD Socket API Example
   This example code is in the Public Domain (or CC0 licensed, at your option.)
   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "head.h"

static const char *TAG = "Udp Driver";
static const char *payload = "hellow";

static struct sockaddr_in dest_addr;


esp_err_t udp_socket_init(int *sock){
    dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    char addr_str[16]="";
    inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);
    *sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (*sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Socket created, %s:%d", HOST_IP_ADDR, PORT);
    return ESP_OK
}

static esp_err_t udp_client_start(int *sock){
	char messeng[] = COMM_START;
	ESP_LOGD("START RX");
	int err = sendto(*sock, messeng, strlen(messeng), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

	if (err < 0) {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Message sent");
    return ESP_OK;
}

void udp_client_task(void *pvParameters)
{
    char rx_buffer[128];
    char host_ip[] = HOST_IP_ADDR;
    int addr_family = 0;
    int ip_protocol = 0;
    int sock=-1;
    esp_err_t err=0;

    while (!err) {
//		return socket
//    	if error returnt ESP_FAIL
    	err |=udp_socket_init(&sock);
        while (!err) {
// Start trans for server
        	udp_client_start(&sock);
            struct sockaddr_in source_addr; // Large enough for both IPv4 or IPv6
            socklen_t socklen = sizeof(source_addr);
            int len = recvfrom(sock, rx_buffer, sizeof(rx_buffer) - 1, 0, (struct sockaddr *)&source_addr, &socklen);

            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
                break;
            }
            // Data received
            else {
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, source_addr.sin_addr);
                ESP_LOGI(TAG, "%s", rx_buffer);
                if (strncmp(rx_buffer, "OK: ", 4) == 0) {
                    ESP_LOGI(TAG, "Received expected message, reconnecting");
                    break;
                }

            }
            bzero(rx_buffer,len-1);
            rx_buffer[len]='\0';
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    vTaskDelete(NULL);
}
