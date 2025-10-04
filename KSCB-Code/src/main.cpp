#include "main.h"

void setup() {
    pinMode(PIN_L298N_ENA, OUTPUT);
    pinMode(PIN_L298N_IN1, OUTPUT);
    pinMode(PIN_L298N_IN2, OUTPUT);
    pinMode(PIN_BUILTIN_LED, OUTPUT);

    // Motor Forward Direcction
    digitalWrite(PIN_L298N_IN1, HIGH);
    digitalWrite(PIN_L298N_IN2, LOW);

}

void loop() {
    unsigned int delay_interval = map(
        analogRead(PIN_POT_SPEED),
        0, 1024, 
        0, 10000
    );

    digitalWrite(PIN_L298N_ENA, LOW);
    digitalWrite(PIN_BUILTIN_LED, LOW);
    delay(delay_interval);

    digitalWrite(PIN_L298N_ENA, HIGH);
    digitalWrite(PIN_BUILTIN_LED, HIGH);
    delay(200);
}
