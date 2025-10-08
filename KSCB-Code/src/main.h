#ifndef __MAIN_H__
#define __MAIN_H__

#include <Arduino.h>
#include "TM1637Display.h"

#define DEBUG_IO

#define PIN_LED_BUILTIN 13

#define PIN_LED_BREW    6
#define PIN_LED_FIN     5
#define PIN_LED_MOTOR   4
#define PIN_LED_HR_MIN  3
#define PIN_LED_ML      2

#define PIN_BTN_CHANGE  14
#define PIN_BTN_START   17
#define PIN_BTN_PLUS    15
#define PIN_BTN_MINUS   16

#define PIN_L298N_ENA   11
#define PIN_L298N_IN1   10
#define PIN_L298N_IN2   9

#define PIN_TM1637_DIO  12
#define PIN_TM1637_CLK  7



#define TM1637_DOT_BYTECODE 0b01000000

TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);


#endif  // __MAIN_H__