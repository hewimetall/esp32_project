/* BSD Socket API Example
 This example code is in the Public Domain (or CC0 licensed, at your option.)
 Unless required by applicable law or agreed to in writing, this
 software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 CONDITIONS OF ANY KIND, either express or implied.
 */
#include "head.h"

static const char *TAG = "Udp Driver";

static struct sockaddr_in dest_addr;

esp_err_t udp_socket_init(int *sock) {
	dest_addr.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	char addr_str[16] = "";
	inet_ntoa_r(dest_addr.sin_addr, addr_str, sizeof(addr_str) - 1);
	*sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (*sock < 0) {
		ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "Socket created, %s:%d", HOST_IP_ADDR, PORT);
	return ESP_OK;
}

static esp_err_t udp_client_start(int *sock) {
	char messeng[] = COMM_START;
	ESP_LOGD(TAG, "START RX");
	int err = sendto(*sock, messeng, strlen(messeng), 0,
			(struct sockaddr*) &dest_addr, sizeof(dest_addr));

	if (err < 0) {
		ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "Message sent err %i",err);
	return ESP_OK;
}

static esp_err_t udp_client_recv(int *sock) {
	char rx_buffer[128];
	struct sockaddr_in source_addr; // Large enough for both IPv4 or IPv6
	socklen_t socklen = sizeof(source_addr);

	int len = recvfrom(*sock, rx_buffer, sizeof(rx_buffer) - 1, 0,
			(struct sockaddr*) &source_addr, &socklen);

	// Error occurred during receiving
	if (len < 0) {
		ESP_LOGE(TAG, "recvfrom failed: errno %d", errno);
		return ESP_FAIL;
	}
	// Data received
	else {
		rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
		ESP_LOGI(TAG, "%s", rx_buffer);
		solve_json(&rx_buffer);
	}
	bzero(rx_buffer, len - 1);

	return ESP_OK;
}

void udp_client_task(void *pvParameters) {
	int sock = -1;
	esp_err_t err = 0;

	while (!err) {
		err =0;
		/* Init socket udp and recive int sock*/
		err |= udp_socket_init(&sock);
		/* this Start rx comm for server */
		err |= udp_client_start(&sock);

		/* Set bit event group active rx */
		xEventGroupSetBits(s_wifi_event_group,
		UDP_SEND_ACTIVE);

		while (!err) {
			// SEND DATE
		err |= udp_client_recv(&sock);
			vTaskDelay(2000 / portTICK_PERIOD_MS);
		}

		err =0;
		/* Clear bit event group active rx */
		xEventGroupClearBits(s_wifi_event_group,
		UDP_SEND_ACTIVE);
		if (sock != -1) {
			ESP_LOGE(TAG, "Shutting down socket and restarting...");
			shutdown(sock, 0);
			close(sock);
		}
	}
	vTaskDelete(NULL);
}
