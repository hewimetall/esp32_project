/*
 * tcp_drive.c
 *
 *  Created on: Apr 26, 2020
 *      Author: j
 */
#include "head.h"

static const char *TAG = "Tcp Drive";

esp_err_t tcp_socket_init(int *sock){
	/* Get head socket*/
	*sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (*sock < 0) {
		ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
		return ESP_FAIL;
	}
	return ESP_OK;
}

esp_err_t tcp_socket_conn(int *sock){
	char host_ip[] = HOST_IP_ADDR;
	struct sockaddr_in dest_addr;
	dest_addr.sin_addr.s_addr = inet_addr(host_ip);
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(PORT);
	int err = connect(*sock, (struct sockaddr*) &dest_addr,
					sizeof(struct sockaddr_in6));
	if (err != 0) {
		ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
		return ESP_FAIL;
	}
	ESP_LOGI(TAG, "Successfully connected");

	return ESP_OK;
}

esp_err_t tcp_socket_send(int *sock,void *mem,size_t len){
	int err = send(*sock, mem, len, 0);
	if (err < 0) {
		ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
		return ESP_FAIL;
	}
return ESP_OK;
}

esp_err_t tcp_socket_send_start(int *sock){
	char command[] = COMM_START;
	int err=tcp_socket_send(sock,(void *) &command,sizeof(command)-1);
	if (err < 0) {
			ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
			return ESP_FAIL;
	}
	return ESP_OK;
}

int tcp_socket_recv(int *sock,void *mem,size_t len){
	int my_len=0;
	my_len =recv(*sock,mem,len-1,0);
	if (my_len <= 0){
			ESP_LOGI(TAG, "date not resive");
			return -1;
		}
	return my_len;
}

void tcp_client_task(void *pvParameters) {
	char rx_buffer[128];
	size_t rx_len;
	int my_socket,err=0;
	int len;
	rx_len =  sizeof rx_buffer;
	/* tcp get socket */
	tcp_socket_init(&my_socket);
	/*	Connect via socket */
	tcp_socket_conn(&my_socket);
	/* Command start */
	err=tcp_socket_send_start(&my_socket);
	while (1) {
			// Data received
			ESP_LOGI(TAG, "Received byte");
			len=tcp_socket_recv(&my_socket,rx_buffer,rx_len);
			rx_buffer[len]='\0';
			ESP_LOGI(TAG,"Recieved date: %s",rx_buffer);
			if(err){
				ESP_LOGE(TAG,"ERROS loops ");
				break;
			}
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}

		if (my_socket!= -1) {
			ESP_LOGE(TAG, "Shutting down socket and restarting...");
			shutdown(my_socket, 0);
			close(my_socket);
		}

	vTaskDelete(NULL);
}
