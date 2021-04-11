#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>    
#include <softPwm.h>  /* include header file for software PWM */
#include <unistd.h>

#define LAMPADA_COZINHA 0
#define DEVICE_FAN 5

void init_gpio_device(int device_type) {
	wiringPiSetup();		/* initialize wiringPi setup */
	pinMode(device_type, OUTPUT);	/* set GPIO as output */
	softPwmCreate(device_type, 1, 100);	/* set PWM channel along with range*/
}

void handle_gpio_device(int device_type, int intensity) {
    softPwmWrite (device_type, intensity);
	usleep(8000);
}
