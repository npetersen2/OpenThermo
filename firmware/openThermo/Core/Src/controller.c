#include "controller.h"
#include "ui.h"
#include "platform.h"
#include "drv_tmp117.h"
#include <stdint.h>
#include <stdbool.h>

// Tuning for LPF based on Ts = 1 sec
//
// Ts = 1
// A = exp(-2 * pi * Fb * Ts);
#define FILTER_A_4hour_TC (0.99956376286f)
inline static float __filter(float filter_gain_A, float input, float *prev_input);

static float heat_on_last_hour_percent_z1 = 0.0f;
static float heat_on_last_hour_percent = 0.0f;

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

	// Update filter for time heat has been on
	float on_percent = 0.0f;
	if (is_heat_on) {
		on_percent = 100.0f;
	}
	heat_on_last_hour_percent = __filter(FILTER_A_4hour_TC, on_percent, &heat_on_last_hour_percent_z1);
}

void controller_set_reference(float tempF)
{
	tempF_ref = tempF;
}

float controller_get_percent_heat_on_last_hour(void)
{
	return heat_on_last_hour_percent;
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
		// Record time
		time_on_sec++;
		time_off_sec = 0;
	} else {
		// Record time
		time_off_sec++;
		time_on_sec = 0;
	}
}

inline static float __filter(float filter_gain_A, float input, float *z1)
{
    float output = (input * (1.0f - filter_gain_A)) + *z1;
    *z1 = output * filter_gain_A;
    return output;
}
