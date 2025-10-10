#ifndef __MAIN_H__
#define __MAIN_H__

#include "Arduino.h"
#include "TM1637Display.h"
#include "FSMMacroV2.h"
#include "TimerMacroV2.h"
#include "L298NSingle.h"
#include "LEDSimple.h"
#include "ButtonSimple.h"

// #define DEBUG

//  +---------------------------------------------------------------------------------------------+
//  |                                              IO                                             |
//  +---------------------------------------------------------------------------------------------+

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

LED led_brew(PIN_LED_BREW);
LED led_fin(PIN_LED_FIN);
LED led_motor(PIN_LED_MOTOR);
LED led_ml(PIN_LED_ML);
LED led_hr_min(PIN_LED_HR_MIN);

Button btn_change(PIN_BTN_CHANGE);
Button btn_start(PIN_BTN_START);
Button btn_plus(PIN_BTN_PLUS);
Button btn_minus(PIN_BTN_MINUS);

TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);

L298NSingle pump(PIN_L298N_ENA, PIN_L298N_IN1, PIN_L298N_IN2);

//  +---------------------------------------------------------------------------------------------+
//  |                                       Internal States                                       |
//  +---------------------------------------------------------------------------------------------+



//  +---------------------------------------------------------------------------------------------+
//  |                                          Main Loop                                          |
//  +---------------------------------------------------------------------------------------------+

enum FSM_Main_Loop {
    ML_init, ML_idle_prep, ML_idle, 
    ML_S0, ML_S0A, ML_C0, ML_M0, ML_P0, 
    ML_brew_prep, ML_brew, 
    ML_S1, ML_S1A, ML_C1, ML_M1, ML_P1,
    ML_finish, ML_finish_A, 
    ML_weight,
    ML_S2, ML_S2A, ML_C2, ML_M2, ML_P2
};
FSM_Main_Loop state_main_loop = ML_init;

CREATE_FSM(main_loop, ML_init);
CREATE_TIMER(main_loop)
void main_loop_update();

#endif  // __MAIN_H__