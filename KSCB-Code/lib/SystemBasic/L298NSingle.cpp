#include "L298NSingle.h"

L298NSingle::L298NSingle(int pin_ena, int pin_in1, int pin_in2)
    : pin_ena(pin_ena), pin_in1(pin_in1), pin_in2(pin_in2) {

    pinMode(this->pin_ena, OUTPUT);
    pinMode(this->pin_in1, OUTPUT);
    pinMode(this->pin_in2, OUTPUT);

    //  default as forward direction
    digitalWrite(this->pin_in1, HIGH);
    digitalWrite(this->pin_in2, LOW);
}

L298NSingle::~L298NSingle() {
}

void L298NSingle::forward_on() {
    digitalWrite(this->pin_in1, HIGH);
    digitalWrite(this->pin_in2, LOW);

    digitalWrite(this->pin_ena, HIGH);
}

void L298NSingle::backward_on() {
    digitalWrite(this->pin_in1, HIGH);
    digitalWrite(this->pin_in2, LOW);

    digitalWrite(this->pin_ena, HIGH);
}

void L298NSingle::on() {
    digitalWrite(this->pin_ena, HIGH);
}

void L298NSingle::off() {
    digitalWrite(this->pin_ena, LOW);
}
