#include <stdio.h>
#include <unistd.h>         //Used for UART
#include <fcntl.h>          //Used for UART
#include <termios.h>        //Used for UART
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include "i2cbme.h"
#include "bme280_defs.h"
#include "socket.h"
#include "csv.h"

void handle_init();
void handle_close();
pthread_t thread;

int main(int argc, const char * argv[]) {
    handle_init();

    struct bme280_data temp_and_humidty;
    signal(SIGINT, handle_close);

    //pthread_create(&thread, NULL, &func, NULL);
    //pthread_detach(thread); 

    while(1) {
        temp_and_humidty = get_external_temperature_and_humidity();

        printf("Temp:: %f Humidity:: %f \n", temp_and_humidty.temperature, temp_and_humidty.humidity);
        write_csv_on_file(temp_and_humidty.temperature, temp_and_humidty.humidity);
        sleep(1);
    }

    return 0;
}

void handle_init() {
    //init_socket();
    init_bme();
    handle_file_creation();
}

void handle_close() {
    printf("\nEncerrando execução...\n");
    close_bme();
    //close_socket();
    exit(0);
}
