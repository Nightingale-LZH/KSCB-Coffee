#include "LEDSimple.h"

LED::LED(int pin_led): pin_led(pin_led) {
    pinMode(this->pin_led, OUTPUT);

    //  Default as off
    digitalWrite(this->pin_led, LOW);
}

LED::~LED() {
}

void LED::on() {
    digitalWrite(this->pin_led, HIGH);
}

void LED::off() {
    digitalWrite(this->pin_led, LOW);
}
