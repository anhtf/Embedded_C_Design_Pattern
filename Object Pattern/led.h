#ifndef LED_H
#define LED_H

#include <stdint.h>

struct led_device {
	uint8_t pin;
	uint8_t state;
};

void led_init(struct led_device *dev, uint8_t pin_num);
void led_turn_on(struct led_device *dev);
void led_turn_off(struct led_device *dev);
uint8_t led_get_state(struct led_device *dev);

#endif