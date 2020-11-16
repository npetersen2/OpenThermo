#ifndef LED7SEG_H
#define LED7SEG_H

#include <stdint.h>
#include <stdbool.h>

// 0 = dim, 10000 = full brightness
void led7seg_set_brightness(int level);

void led7seg_show_float(float out);
void led7seg_show_int(int out);

void led7seg_show(char out1, bool dp1, char out2, bool dp2, char out3, bool dp3);
void led7seg_show_raw(uint8_t out1, uint8_t out2, uint8_t out3);

#endif // LED7SEG_H
