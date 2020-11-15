#ifndef DRV_ADC_H
#define DRV_ADC_H

#include <stdint.h>

void drv_adc_init(void);

uint32_t drv_adc_sample(void);

#endif // DRV_ADC_H
