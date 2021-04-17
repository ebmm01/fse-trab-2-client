#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>    
#include <string.h>    
#include <softPwm.h>  /* include header file for software PWM */
#include <stdlib.h>
#include <unistd.h>
#include "gpio.h"
#include "socket.h"
#include "events.h"

static int sensor_reference[6] = {
	LAMPADA_COZINHA, // GPIO 17
	LAMPADA_SALA, // GPIO 18
	LAMPADA_QUARTO_01, // GPIO 27
	LAMPADA_QUARTO_02, // GPIO 22
	AR_CONDICIONADO_QUARTO_01, // GPIO 13
	AR_CONDICIONADO_QUARTO_02, // GPIO 19
};

void init_gpio_device(int device_type) {
	pinMode(device_type, OUTPUT);	/* set GPIO as output */
	softPwmCreate(device_type, 1, 100);	/* set PWM channel along with range*/
}

void handle_gpio_device(int device_type, int intensity) {
    softPwmWrite (device_type, intensity);
	usleep(15000);
}

int get_device_state(int device_type) {
	return digitalRead(device_type);
}

void gpio_handle_message(char* payload) {
	char *temp_str;
	int sensor_reference_index, sensor_value;

    temp_str = strtok(payload, ";");
	sensor_reference_index = atoi(temp_str);
    
	temp_str = strtok(NULL, ";");
	sensor_value = atoi(temp_str);

	handle_gpio_device(sensor_reference[sensor_reference_index], sensor_value * 100);

	char message[100];
	int sensor_state;

	if (sensor_reference_index > 3) // Por algum motivo os ar-condicionados sempre retornam 0 :/
		sensor_state = sensor_value;
	else
		sensor_state = get_device_state(sensor_reference_index);
	
	sprintf(message, "%s;%d;%d", GPIO_DEVICE_STATE, sensor_reference_index, sensor_state);

	send_message_to_server(message, strlen(message));
}