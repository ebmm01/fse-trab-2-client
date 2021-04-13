#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>    
#include <stdlib.h>    
#include <softPwm.h>  /* include header file for software PWM */
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include "interrupcoes.h"

static int sensorPins[] = {
    SENSOR_PRESENCA_SALA, // GPIO 25
    SENSOR_PRESENCA_COZINHA,  // GPIO 26
    SENSOR_ABERTURA_PORTA_COZINHA,  // GPIO 5
    SENSOR_ABERTURA_JANELA_COZINHA,  // GPIO 6
    SENSOR_ABERTURA_PORTA_SALA,  // GPIO 12
    SENSOR_ABERTURA_JANELA_SALA,  // GPIO 16
    SENSOR_ABERTURA_JANELA_QUARTO_01,  // GPIO 20
    SENSOR_ABERTURA_JANELA_QUARTO_02,  // GPIO 21
};

volatile int sensorData[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

static struct timeval last_changes[8];

struct timeval get_now() {
    struct timeval now;

    gettimeofday(&now, NULL);

    return now;
}

void handle_0() {
    unsigned long diff;

    struct timeval now = get_now();

    diff = (now.tv_sec + 1000000 + now.tv_usec) - (last_changes[0].tv_sec + 1000000 + last_changes[0].tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        if (sensorData[0]) {
            printf("SENSOR_PRESENCA_SALA Falling\n");
        }
        else 
            printf("SENSOR_PRESENCA_SALA Rising\n");
        sensorData[0] = !sensorData[0];
    }

    last_changes[0] = now;
}

void handle_1() {
    unsigned long diff;

    struct timeval now = get_now();

    diff = (now.tv_sec + 1000000 + now.tv_usec) - (last_changes[1].tv_sec + 1000000 + last_changes[1].tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        if (sensorData[1]) {
            printf("SENSOR_PRESENCA_COZINHA Falling\n");
        }
        else 
            printf("SENSOR_PRESENCA_COZINHA Rising\n");
        sensorData[1] = !sensorData[1];
    }

    last_changes[1] = now;
}

void handle_2() {
    unsigned long diff;

    struct timeval now = get_now();

    diff = (now.tv_sec + 1000000 + now.tv_usec) - (last_changes[2].tv_sec + 1000000 + last_changes[2].tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        if (sensorData[2]) {
            printf("SENSOR_ABERTURA_PORTA_COZINHA Falling\n");
        }
        else 
            printf("SENSOR_ABERTURA_PORTA_COZINHA Rising\n");
        sensorData[2] = !sensorData[2];
    }

    last_changes[2] = now;
}

void handle_3() {
    unsigned long diff;

    struct timeval now = get_now();

    diff = (now.tv_sec + 1000000 + now.tv_usec) - (last_changes[3].tv_sec + 1000000 + last_changes[3].tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        if (sensorData[3]) {
            printf("SENSOR_ABERTURA_JANELA_COZINHA Falling\n");
        }
        else 
            printf("SENSOR_ABERTURA_JANELA_COZINHA Rising\n");
        sensorData[3] = !sensorData[3];
    }

    last_changes[3] = now;
}

void handle_4() {
    unsigned long diff;

    struct timeval now = get_now();

    diff = (now.tv_sec + 1000000 + now.tv_usec) - (last_changes[4].tv_sec + 1000000 + last_changes[4].tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        if (sensorData[4]) {
            printf("SENSOR_ABERTURA_PORTA_SALA Falling\n");
        }
        else 
            printf("SENSOR_ABERTURA_PORTA_SALA Rising\n");
        sensorData[4] = !sensorData[4];
    }

    last_changes[4] = now;
}

void handle_5() {
    unsigned long diff;

    struct timeval now = get_now();

    diff = (now.tv_sec + 1000000 + now.tv_usec) - (last_changes[5].tv_sec + 1000000 + last_changes[5].tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        if (sensorData[5]) {
            printf("SENSOR_ABERTURA_JANELA_SALA Falling\n");
        }
        else 
            printf("SENSOR_ABERTURA_JANELA_SALA Rising\n");
        sensorData[5] = !sensorData[5];
    }

    last_changes[5] = now;
}

void handle_6() {
    unsigned long diff;

    struct timeval now = get_now();

    diff = (now.tv_sec + 1000000 + now.tv_usec) - (last_changes[6].tv_sec + 1000000 + last_changes[6].tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        if (sensorData[6]) {
            printf("SENSOR_ABERTURA_JANELA_QUARTO_01 Falling\n");
        }
        else 
            printf("SENSOR_ABERTURA_JANELA_QUARTO_01 Rising\n");
        sensorData[6] = !sensorData[6];
    }

    last_changes[6] = now;
}

void handle_7() {
    unsigned long diff;

    struct timeval now = get_now();

    diff = (now.tv_sec + 1000000 + now.tv_usec) - (last_changes[7].tv_sec + 1000000 + last_changes[7].tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        if (sensorData[7]) {
            printf("SENSOR_ABERTURA_JANELA_QUARTO_02 Falling\n");
        }
        else 
            printf("SENSOR_ABERTURA_JANELA_QUARTO_02 Rising\n");
        sensorData[7] = !sensorData[7];
    }

    last_changes[7] = now;
}

void (*func_ptr[8])() = { handle_0, handle_1, handle_2, handle_3, handle_4, handle_5, handle_6, handle_7 };

void* init_interruptions_handler() {

    wiringPiSetup();
    

    printf("\nPINMODE:: \n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", sensorPins[i]);
        pinMode(sensorPins[i], OUTPUT);
    }

    printf("\ngettimeofday:: \n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", sensorPins[i]);
        gettimeofday(&last_changes[i], NULL);
    }

    printf("\nwiringPiISR:: \n");
    for (int i = 0; i < 8; i++) {
        printf("%d ", sensorPins[i]);
        wiringPiISR(sensorPins[i], INT_EDGE_BOTH, func_ptr[i]);
    }

    updateSensorData();

    while(1)
        sleep(1);
}

void updateSensorData() {

    for (int i = 0; i < 8; i++) {
        sensorData[i] = digitalRead(sensorPins[i]);
    }
}