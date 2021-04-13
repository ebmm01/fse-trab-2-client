#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include "bme280.h"
#include "socket.h"
#include "csv.h"
#include "interrupcoes.h"

void handle_init();
void handle_close();
pthread_t thread_interruptions;

int main(int argc, const char * argv[]) {
    handle_init();

    int T, P, H;

    signal(SIGINT, handle_close);

    pthread_create(&thread_interruptions, NULL, &init_interruptions_handler, NULL);
    pthread_detach(thread_interruptions);

    
    while(1) {
        bme280ReadValues(&T, &P, &H);
        updateSensorData();
        
        // printf("Calibrated temp. = %.2f C, hum. = %.2f\n", (float)T/100.0, (float)H/836.0);
        // // printf("Printando dados sensores\n");
        // for (int i = 0; i < 8; i++) {
        //     printf("%d ", sensorData[i]);
        // }
        // printf("\n");
        
        write_csv_on_file((float)T/100.0, (float)H/1024.0);
        sleep(1);
    }

    return 0;
}

void handle_init() {
    printf("\nIniciando execução, aguarde...\n");

    //init_socket();

    if (bme280Init(0x76))
	{
        printf("\nErro ao iniciar BME. Encerrando...\n");
		exit(1); // problem - quit
	}
    else 
        printf("BME280 aberto com sucesso.\n");

    handle_file_creation();
    sleep(1);
}

void handle_close() {
    printf("\nEncerrando execução...\n");

    pthread_cancel(thread_interruptions);
    //close_socket();
    exit(0);
}
