#include "led.h"

void led_init(struct led_device *dev, uint8_t pin_num)
{
	if (!dev)
		return;

	dev->pin = pin_num;
	dev->state = 0;

	hal_gpio_configure_output(dev->pin);
	hal_gpio_write(dev->pin, 0);
}

void led_turn_on(struct led_device *dev)
{
	if (!dev)
		return;

	dev->state = 1;
	hal_gpio_write(dev->pin, 1);
}

void led_turn_off(struct led_device *dev)
{
	if (!dev)
		return;

	dev->state = 0;
	hal_gpio_write(dev->pin, 0);
}

uint8_t led_get_state(struct led_device *dev)
{
	if (!dev)
		return 0;

	return dev->state;
}