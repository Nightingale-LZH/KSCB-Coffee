#include <Arduino.h>

#define PIN_L298N_ENA 3
#define PIN_L298N_IN1 2
#define PIN_L298N_IN2 4

#define PIN_POT_SPEED A0

void setup() {
    pinMode(PIN_L298N_ENA, OUTPUT);
    pinMode(PIN_L298N_IN1, OUTPUT);
    pinMode(PIN_L298N_IN2, OUTPUT);

    // Motor Forward Direcction
    digitalWrite(PIN_L298N_IN1, HIGH);
    digitalWrite(PIN_L298N_IN2, LOW);
}

void loop() {
    analogWrite(
        PIN_L298N_ENA, 
        map(
            analogRead(PIN_POT_SPEED),
            0, 1024, 
            0, 255
        )
    );

    delay(100);
}
