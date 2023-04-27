#include <Arduino.h>

#define MAX_PWM_VALUE 255

void pin_toggle(uint8_t pin, uint16_t pwidth);
void pin_pwm(uint8_t pin, uint16_t pwidth);
void pin_on(uint8_t pin);
void pin_off(uint8_t pin);

