#include "ButtonSimple.h"

Button::Button(int pin_btn): pin_btn(pin_btn) {
    pinMode(this->pin_btn, INPUT_PULLUP);
}

Button::~Button() {
}

bool Button::is_pressed() {
    return !digitalRead(this->pin_btn);
}

bool Button::is_released() {
    return digitalRead(this->pin_btn);
}
