#ifndef __BME280__
#define __BME280__

int bme280Init(int iAddr);
int bme280ReadValues(int *T, int *P, int *H);
void get_temperature_and_humidity(float *temperature, float *humidity);

#endif // __BME280__