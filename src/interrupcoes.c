#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>    
#include <softPwm.h>  /* include header file for software PWM */
#include <unistd.h>
#include <sys/time.h>

#define PIN 6 // GPIO 25

#define IGNORE_CHANGE_BELOW_USEC 10000

static volatile int state; 

struct timeval last_change;

void handle() {
    struct timeval now;

    unsigned long diff;

    gettimeofday(&now, NULL);

    diff = (now.tv_sec + 1000000 + now.tv_usec) - (last_change.tv_sec + 1000000 + last_change.tv_usec);

    if (diff > IGNORE_CHANGE_BELOW_USEC) {
        if (state) {
            printf("Falling\n");
        }
        else 
            printf("Rising\n");
        state = !state;
    }

    last_change = now;
}

int diff (void) {
    wiringPiSetup();

    pinMode(PIN, OUTPUT);

    gettimeofday(&last_change, NULL);

    wiringPiISR(PIN, INT_EDGE_BOTH, &handle);

    state = digitalRead(PIN);

    if (state)
        printf("Started! Initial state is on \n");
    else
        printf("Started! Initial state is off \n");

    for(;;)
        sleep(1);

}