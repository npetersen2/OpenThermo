#include "app.h"
#include "controller.h"
#include "ui.h"

static int counter_1Hz = 0;

// This callback is ran at 100Hz
void app_callback(void)
{
	counter_1Hz++;
	if (counter_1Hz >= 100) {
		counter_1Hz = 0;

		// Run controller at 1Hz
		controller_step();
	}

	// Update UI at 100Hz
	ui_step();
}
