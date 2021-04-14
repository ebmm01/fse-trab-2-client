
#ifndef GPIO_H_
#define GPIO_H_

#define LAMPADA_COZINHA 0 // GPIO 17
#define LAMPADA_SALA 1 // GPIO 18
#define LAMPADA_QUARTO_01 2 // GPIO 27
#define LAMPADA_QUARTO_02 3 // GPIO 22
#define AR_CONDICIONADO_QUARTO_01 23 // GPIO 13
#define AR_CONDICIONADO_QUARTO_02 24 // GPIO 19


void init_gpio_device(int device_type);
void handle_gpio_device(int device_type, int intensity);
int get_device_state(int device_type);


#endif /* GPIO_H_ */