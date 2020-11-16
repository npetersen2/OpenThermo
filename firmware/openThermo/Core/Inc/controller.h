#ifndef CONTROLLER_H
#define CONTROLLER_H

void controller_step(void);

void controller_set_reference(float tempF);

float controller_get_percent_heat_on_last_hour(void);

#endif // CONTROLLER_H
