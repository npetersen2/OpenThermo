#include "led7seg.h"
#include "platform.h"
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define ASSERT_LE   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define DEASSERT_LE HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)

static const uint8_t SEGa  = 0x01;
static const uint8_t SEGb  = 0x02;
static const uint8_t SEGc  = 0x04;
static const uint8_t SEGd  = 0x08;
static const uint8_t SEGe  = 0x10;
static const uint8_t SEGf  = 0x20;
static const uint8_t SEGg  = 0x40;
static const uint8_t SEGdp = 0x80;

// See: https://en.wikipedia.org/wiki/Seven-segment_display
static uint8_t __char_mapping(char c)
{
	uint8_t out = 0;

	switch (c) {

	// =======
	// Numbers
	// =======
	case '0':
		out = SEGa | SEGb | SEGc | SEGd | SEGe | SEGf;
		break;
	case '1':
		out = SEGb | SEGc;
		break;
	case '2':
		out = SEGa | SEGb | SEGd | SEGe | SEGg;
		break;
	case '3':
		out = SEGa | SEGb | SEGc | SEGd | SEGg;
		break;
	case '4':
		out = SEGb | SEGc | SEGf | SEGg;
		break;
	case '5':
		out = SEGa | SEGc | SEGd | SEGf | SEGg;
		break;
	case '6':
		out = SEGa | SEGc | SEGd | SEGe | SEGf | SEGg;
		break;
	case '7':
		out = SEGa | SEGb | SEGc;
		break;
	case '8':
		out = SEGa | SEGb | SEGc | SEGd | SEGe | SEGf | SEGg;
		break;
	case '9':
		out = SEGa | SEGb | SEGc | SEGd | SEGf | SEGg;
		break;

	// =======
	// Lower-case letters
	// =======
	case 'b':
		out = SEGc | SEGd | SEGe | SEGf | SEGg;
		break;
	case 'c':
		out = SEGd | SEGe | SEGg;
		break;
	case 'd':
		out = SEGb | SEGc | SEGd | SEGe | SEGg;
		break;
	case 'h':
		out = SEGc | SEGe | SEGf | SEGg;
		break;
	case 'i':
		out = SEGc;
		break;
	case 'n':
		out = SEGc | SEGe | SEGg;
		break;
	case 'o':
		out = SEGc | SEGd | SEGe | SEGg;
		break;
	case 'r':
		out = SEGe | SEGg;
		break;
	case 't':
		out = SEGd | SEGe | SEGf | SEGg;
		break;
	case 'u':
		out = SEGc | SEGd | SEGe;
		break;
	case 'y':
		out = SEGb | SEGc | SEGe | SEGf | SEGg;
		break;

	// =======
	// Upper-case letters
	// =======
	case 'A':
		out = SEGa | SEGb | SEGc | SEGe | SEGf | SEGg;
		break;
	case 'C':
		out = SEGa | SEGd | SEGe | SEGf;
		break;
	case 'E':
		out = SEGa | SEGd | SEGe | SEGf | SEGg;
		break;
	case 'F':
		out = SEGa | SEGe | SEGf | SEGg;
		break;
	case 'H':
		out = SEGb | SEGc | SEGe | SEGf | SEGg;
		break;
	case 'I':
		out = SEGe | SEGf;
		break;
	case 'J':
		out = SEGb | SEGc | SEGd | SEGe;
		break;
	case 'L':
		out = SEGd | SEGe | SEGf;
		break;
	case 'O':
		out = SEGa | SEGb | SEGc | SEGd | SEGe | SEGf;
		break;
	case 'P':
		out = SEGa | SEGb  | SEGe | SEGf | SEGg;
		break;
	case 'S':
		out = SEGa | SEGc | SEGd | SEGf | SEGg;
		break;
	case 'U':
		out = SEGb | SEGc | SEGd | SEGe | SEGf | SEGg;
		break;

	default:
		out = SEGg;
		break;
	}

	return out;
}


static void __send_byte(uint8_t word)
{
	  // Wait until TX buffer is empty
	  while (!(SPI1->SR & SPI_SR_TXE));

	  // Send bytes over the SPI
	  volatile uint8_t *out =  (volatile uint8_t*) &(SPI1->DR);
	  *out = word;

	  // Wait until the transmission is complete
	  while (SPI1->SR & SPI_SR_BSY);
}

void led7seg_set_brightness(int level)
{
	TIM3->CCR1 = level;

}

void led7seg_show_float(float out)
{
    // Round to 1 decimal point
	out = roundf(out * 100.0f) * 0.01;

    // Parse out each LED display output
    int tens = (int)(out * 0.1);
    int ones = (int)(out - (tens * 10));
    int tenths = (int)((out - (tens * 10) - ones) * 10);


    // Show on LEDs
	led7seg_show(
			'0' + tenths, 0,
			'0' + ones, 1,
			'0' + tens, 0
			);
}

void led7seg_show(char out1, bool dp1, char out2, bool dp2, char out3, bool dp3)
{
	led7seg_show_raw(
			__char_mapping(out1) | (dp1 ? SEGdp : 0),
			__char_mapping(out2) | (dp2 ? SEGdp : 0),
			__char_mapping(out3) | (dp3 ? SEGdp : 0)
			);
}

void led7seg_show_raw(uint8_t out1, uint8_t out2, uint8_t out3)
{
	__send_byte(out1);
	__send_byte(out2);
	__send_byte(out3);

	ASSERT_LE;
	DEASSERT_LE;
}
