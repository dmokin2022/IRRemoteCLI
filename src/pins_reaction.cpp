#include "pins_reaction.h"

void pin_toggle(uint8_t pin, uint16_t pwidth) {
    int current_state = digitalRead(pin);

    if (current_state) 
        analogWrite(pin, 0);
    else
        analogWrite(pin, pwidth);

    //digitalWrite(pin, digitalRead(pin) ? LOW : HIGH);
}

void pin_pwm(uint8_t pin, uint16_t pwidth) {
    analogWrite(pin, pwidth);
}

void pin_on(uint8_t pin) {
    analogWrite(pin, MAX_PWM_VALUE);
}

void pin_off(uint8_t pin) {
    analogWrite(pin, 0);
}
