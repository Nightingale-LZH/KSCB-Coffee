#include "main.h"

#ifndef DEBUG

void setup() {

}

void loop() {

}


//  +---------------------------------------------------------------------------------------------+
//  |                                          Main Loop                                          |
//  +---------------------------------------------------------------------------------------------+


void main_loop_update() {
    SETUP_FSM_FUNCTION(main_loop);

    
}






#endif  //  not DEBUG
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG

//  DEBUG OPTIONS
#define DEBUG_LED
// #define DEBUG_MOTOR
// #define DEBUG_BUTTON
#define DEBUG_DISPLAY

void setup() {
    #ifdef DEBUG_MOTOR
    pinMode(PIN_L298N_ENA, OUTPUT);
    pinMode(PIN_L298N_IN1, OUTPUT);
    pinMode(PIN_L298N_IN2, OUTPUT);

    // Motor Forward Direcction
    digitalWrite(PIN_L298N_IN1, HIGH);
    digitalWrite(PIN_L298N_IN2, LOW);
    #endif  //  DEBUG_MOTOR


    #ifdef DEBUG_LED
    pinMode(PIN_LED_BUILTIN, OUTPUT);
    pinMode(PIN_LED_BREW  , OUTPUT);
    pinMode(PIN_LED_FIN   , OUTPUT);
    pinMode(PIN_LED_MOTOR , OUTPUT);
    pinMode(PIN_LED_ML    , OUTPUT);
    pinMode(PIN_LED_HR_MIN, OUTPUT);
    #endif  //  DEBUG_LED


    #ifdef DEBUG_BUTTON
    Serial.begin(9600);
    pinMode(PIN_BTN_CHANGE, INPUT_PULLUP);
    pinMode(PIN_BTN_START , INPUT_PULLUP);
    pinMode(PIN_BTN_PLUS  , INPUT_PULLUP);
    pinMode(PIN_BTN_MINUS , INPUT_PULLUP);
    #endif  //  DEBUG_BUTTON


    #ifdef DEBUG_DISPLAY
    
    #endif  //  DEBUG_DISPLAY
}

void loop() {
    #ifdef DEBUG_MOTOR
    digitalWrite(PIN_L298N_ENA, LOW);
    delay(2500);

    digitalWrite(PIN_L298N_ENA, HIGH);
    delay(500);
    #endif  //  DEBUG_MOTOR


    #ifdef DEBUG_LED
    digitalWrite(PIN_LED_BUILTIN, HIGH);
    digitalWrite(PIN_LED_BREW  , HIGH);
    delay(1000);
    digitalWrite(PIN_LED_FIN   , HIGH);
    delay(1000);
    digitalWrite(PIN_LED_MOTOR , HIGH);
    delay(1000);
    digitalWrite(PIN_LED_HR_MIN, HIGH);
    delay(1000);
    digitalWrite(PIN_LED_ML    , HIGH);
    delay(1000);

    digitalWrite(PIN_LED_BUILTIN, LOW);
    digitalWrite(PIN_LED_BREW  , LOW);
    digitalWrite(PIN_LED_FIN   , LOW);
    digitalWrite(PIN_LED_MOTOR , LOW);
    digitalWrite(PIN_LED_ML    , LOW);
    digitalWrite(PIN_LED_HR_MIN, LOW);
    delay(1000);
    #endif  //  DEBUG_LED


    #ifdef DEBUG_BUTTON
    if (!digitalRead(PIN_BTN_CHANGE)) { Serial.println("CHANGE"); }
    if (!digitalRead(PIN_BTN_START )) { Serial.println("START"); }
    if (!digitalRead(PIN_BTN_PLUS  )) { Serial.println("PLUS"); }
    if (!digitalRead(PIN_BTN_MINUS )) { Serial.println("MINUS"); }
    Serial.println("---");
    delay(1000);
    #endif  //  DEBUG_BUTTON


    #ifdef DEBUG_DISPLAY
    display.setBrightness(0x0f);

    display.showNumberDecEx(0000, TM1637_DOT_BYTECODE);
    delay(500);
    display.showNumberDecEx(1111);
    delay(500);
    display.showNumberDecEx(2222, TM1637_DOT_BYTECODE);
    delay(500);
    display.showNumberDecEx(3333);
    delay(500);
    display.showNumberDecEx(4444, TM1637_DOT_BYTECODE);
    delay(500);
    display.showNumberDecEx(5555);
    delay(500);
    display.showNumberDecEx(6666, TM1637_DOT_BYTECODE);
    delay(500);
    display.showNumberDecEx(7777);
    delay(500);
    display.showNumberDecEx(8888, TM1637_DOT_BYTECODE);
    delay(500);
    display.showNumberDecEx(9999);
    delay(500);
    #endif  //  DEBUG_DISPLAY
}
#endif // DEBUG_IO

