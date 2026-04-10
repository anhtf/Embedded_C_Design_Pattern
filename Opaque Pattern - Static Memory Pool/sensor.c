#include "sensor.h"
#include <stdbool.h>
#include <stddef.h>

#define MAX_SENSORS 4

struct sensor {
	int pin;
	int raw_value;
};

static struct sensor sensor_pool[MAX_SENSORS];
static bool sensor_allocated[MAX_SENSORS];

struct sensor *sensor_create(int pin)
{
	int i;

	for (i = 0; i < MAX_SENSORS; i++) {
		if (!sensor_allocated[i]) {
			sensor_allocated[i] = true;
			sensor_pool[i].pin = pin;
			sensor_pool[i].raw_value = 0;
			return &sensor_pool[i];
		}
	}

	return NULL;
}

void sensor_destroy(struct sensor *inst)
{
	int i;

	if (!inst)
		return;

	for (i = 0; i < MAX_SENSORS; i++) {
		if (&sensor_pool[i] == inst) {
			sensor_allocated[i] = false;
			return;
		}
	}
}

int sensor_read(struct sensor *inst)
{
	if (!inst)
		return -1;

	inst->raw_value = hal_gpio_read(inst->pin);

	return inst->raw_value;
}