#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include "events.h"



void handle_socket_message(char* message) {
    char *event;

    event = strtok(message, ";");

    if (strcmp(event, SERVER_CLOSED) == 0) {
        kill(getpid(), SIGINT);
    }
}
