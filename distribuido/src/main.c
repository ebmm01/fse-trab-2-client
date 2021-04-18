#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <wiringPi.h> /* include wiringPi library */
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include "bme280.h"
#include "socket.h"
#include "gpio.h"
#include "csv.h"
#include "interrupcoes.h"
#include "update_temp.h"

void handle_init();
void handle_close();
pthread_t thread_interruptions, thread_sockets;

int main(int argc, const char * argv[]) {
    
    handle_init();

    signal(SIGINT, handle_close);
    signal(SIGPIPE, SIG_IGN);

    pthread_create(&thread_interruptions, NULL, &init_interruptions_handler, NULL);
    pthread_detach(thread_interruptions);
    
    pthread_create(&thread_sockets, NULL, &handle_socket_received_messages, NULL);
    pthread_detach(thread_sockets);
    
    while(1) {
        sleep(1);
    }

    return 0;
}

void handle_init() {
    printf("\nIniciando execução, aguarde...\n");

    init_socket();

    /* initialize wiringPi setup */
    wiringPiSetup();		

    /* initialize bme280 setup */
    if (bme280_init(0x76))
	{
        printf("\nErro ao iniciar BME. Encerrando...\n");
		exit(1); // problem - quit
	}
    else 
        printf("BME280 aberto com sucesso.\n");

    /* initialize output devices */
    init_gpio_device(LAMPADA_COZINHA);
    init_gpio_device(LAMPADA_SALA);
    init_gpio_device(LAMPADA_QUARTO_01);
    init_gpio_device(LAMPADA_QUARTO_02);
    init_gpio_device(AR_CONDICIONADO_QUARTO_01);
    init_gpio_device(AR_CONDICIONADO_QUARTO_02);

    /* handle csv files */
    handle_file_creation();

    sleep(1);
    
    /* send sensors data to sserver */
    send_sensor_data_to_server();
}

void handle_close() {
    printf("\nEncerrando execução...\n");
    close_bme();
    pthread_cancel(thread_interruptions);
    pthread_cancel(thread_sockets);
    close_socket();
    exit(0);
}
