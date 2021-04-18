#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sensors.h"
#include "menu.h"

volatile int sensors_values[] = { 2, 2, 2, 2, 2, 2, 2, 2 };
volatile float temperature = 0, humidity = 0;

char sensor_state[2][20] = { "desativado", "ativado" };

char output_sensor_names[6][35] = {
    "LAMPADA_COZINHA",
	"LAMPADA_SALA",
	"LAMPADA_QUARTO_01",
	"LAMPADA_QUARTO_02",
	"AR_CONDICIONADO_QUARTO_01", 
	"AR_CONDICIONADO_QUARTO_02", 
};

void receive_all_sensors_data(char * payload) {
    for (int i = 0; i < 8; i++) {
        sensors_values[i] =  (int) payload[i] - 48;
    }
}

void update_sensor_data(char * payload) {
    char *temp_str;
	int sensor_reference_index, sensor_value;

    temp_str = strtok(payload, ";");
	sensor_reference_index = atoi(temp_str);
    
	temp_str = strtok(NULL, ";");
	sensor_value = atoi(temp_str);

	sensors_values[sensor_reference_index] = sensor_value;
}

void update_temp_and_humidity(char * payload) {
    char *temp_str;

    temp_str = strtok(payload, ";");
	temperature = atof(temp_str);
    
	temp_str = strtok(NULL, ";");
	humidity = atof(temp_str);
}

void update_gpio_device_value(char * payload) {
    char *temp_str;
	int device_index, sensor_value;

    temp_str = strtok(payload, ";");
	device_index = atoi(temp_str);
    
	temp_str = strtok(NULL, ";");
	sensor_value = atoi(temp_str);

	sensors[device_index] = sensor_value;

	sprintf(terminal_messages, "Sensor %s %s.", output_sensor_names[device_index], sensor_state[sensor_value]);
}