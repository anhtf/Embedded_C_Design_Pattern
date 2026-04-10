#include "led.h"

static struct led_device led_power;
static struct led_device led_error;

int main(void)
{
	led_init(&led_power, 13);
	led_init(&led_error, 14);

	led_turn_on(&led_power);
	
	if (system_has_fault()) {
		led_turn_on(&led_error);
	}

	while (1) {
		system_loop();
	}

	return 0;
}