#include "controller.h"
#include "ui.h"
#include "platform.h"
#include "drv_tmp117.h"
#include <stdint.h>
#include <stdbool.h>

static void __turn_off_heat(void);
static void __turn_on_heat(void);
static void __track_time(void);

static float tempF_ref = 55.0f; // Initialize to cold set point so heat doesn't start right away
static float tempF_deadband = 0.5f;

#define TIME_ON_MINIMUM  (2*60) // [sec]
#define TIME_OFF_MINIMUM (4*60) // [sec]

static uint32_t time_on_sec = 0;
static uint32_t time_off_sec = TIME_OFF_MINIMUM; // Initialize to having been off "forever"

static bool is_heat_on = false;

void controller_step(void)
{
	__track_time();

	float tempF_now = ui_get_latest_tempF();

	if (is_heat_on && tempF_now > (tempF_ref - tempF_deadband/2.0f)) {
		__turn_off_heat();
	}

	if (!is_heat_on && tempF_now < (tempF_ref - tempF_deadband)) {
		__turn_on_heat();
	}
}

void controller_set_reference(float tempF)
{
	tempF_ref = tempF;
}

static void __turn_off_heat(void)
{
	if (!is_heat_on) {
		return;
	}

	// Check is safe to turn off
	if (time_on_sec >= TIME_ON_MINIMUM) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
		is_heat_on = false;
	}
}

static void __turn_on_heat(void)
{
	if (is_heat_on) {
		return;
	}

	// Check is safe to turn off
	if (time_off_sec >= TIME_OFF_MINIMUM) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
		is_heat_on = true;
	}
}

static void __track_time(void)
{
	if (is_heat_on) {
		time_on_sec++;
		time_off_sec = 0;
	} else {
		time_off_sec++;
		time_on_sec = 0;
	}
}
