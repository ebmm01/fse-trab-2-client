#ifndef I2CBME_H_
#define I2CBME_H_
#include "bme280_defs.h"

int init_bme();
struct bme280_data get_external_temperature_and_humidity();
void close_bme();


#endif /* I2CBME_H_ */