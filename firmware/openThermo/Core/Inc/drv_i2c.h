#ifndef DRV_I2C_H
#define DRV_I2C_H

#include <stdint.h>

void drv_i2c_init(void);

void drv_i2c_tx(uint16_t DevAddress, uint8_t *data, uint16_t size);
void drv_i2c_rx(uint16_t DevAddress, uint8_t *data, uint16_t size);

#endif // DRV_I2C_H
