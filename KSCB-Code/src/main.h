#ifndef __MAIN_H__
#define __MAIN_H__

#include "Arduino.h"
#include "TM1637Display.h"
#include "FSMMacroV2.h"
#include "TimerMacroV2.h"
#include "L298NSingle.h"
#include "LEDSimple.h"
#include "ButtonSimple.h"
#include "DisplayManager.h"
#include "PumpManager.h"

// #define DEBUG

//  +---------------------------------------------------------------------------------------------+
//  |                                              IO                                             |
//  +---------------------------------------------------------------------------------------------+

#define FSM_DELAY_MS 5

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

LED led_builtin (PIN_LED_BUILTIN );
LED led_brew    (PIN_LED_BREW    );
LED led_fin     (PIN_LED_FIN     );
LED led_motor   (PIN_LED_MOTOR   );
LED led_ml      (PIN_LED_ML      );
LED led_hr_min  (PIN_LED_HR_MIN  );

Button btn_change(PIN_BTN_CHANGE);
Button btn_start (PIN_BTN_START );
Button btn_plus  (PIN_BTN_PLUS  );
Button btn_minus (PIN_BTN_MINUS );

TM1637Display display(PIN_TM1637_CLK, PIN_TM1637_DIO);
DisplayManager display_manager(&display, &led_ml, &led_hr_min, TM1637_DOT_BYTECODE);

L298NSingle pump(PIN_L298N_ENA, PIN_L298N_IN1, PIN_L298N_IN2);
PumpManager pump_manager(&pump, &led_motor);


//  +---------------------------------------------------------------------------------------------+
//  |                                       Internal States                                       |
//  +---------------------------------------------------------------------------------------------+

//  --- menu ---

#define BREWING_VOLUMN_INCREMENT_ML 50
#define BREWING_TIME_INCREMENT_MINS 30

long brewing_volumn_ml = 500;
long brewing_time_mins = 300;

bool is_setting_volumn = true;
bool is_setting_time = false;

//  --- brewing sequence ---

long brewing_total_time_ms = 0;
long brewing_volumn_left_ml = 0;

int pump_on_duration_ms = 0;
int pump_off_duration_ms = 0;

bool is_showing_brewing_volumn_left = true;
bool is_showing_brewing_time_left = false;

//  --- measuring sequence ---

long curr_volumn_ml = 0;


//  +---------------------------------------------------------------------------------------------+
//  |                                          Main Loop                                          |
//  +---------------------------------------------------------------------------------------------+

enum FSM_Main_Loop {
    ML_init, 
    ML_idle_prep, ML_idle, 
    ML_S0, ML_S0A, ML_C0, ML_M0, ML_P0, 
    ML_brew_prep, ML_brew, 
    ML_S1, ML_S1A, ML_C1, ML_M1, ML_P1,
    ML_finish, ML_finish_A, 
    ML_weight_prep, ML_weight,
    ML_S2, ML_S2A, ML_C2, ML_M2, ML_P2
};
FSM_Main_Loop state_main_loop = ML_init;

CREATE_FSM(main_loop, ML_init);
CREATE_TIMER(main_loop)
CREATE_TIMER(main_loop_brewing)
void main_loop_update();

//  +---------------------------------------------------------------------------------------------+
//  |                                       Helper Function                                       |
//  +---------------------------------------------------------------------------------------------+

void refresh_display_menu();

void refresh_display_brewing();
void refresh_display_brewing_scheduled();   //  refresh every 500 ms. 
CREATE_TIMER(refresh_display_brewing_scheduled_timer)

//  +---------------------------------------------------------------------------------------------+
//  |                                        Autonomous                                           |
//  +---------------------------------------------------------------------------------------------+

// Built-in LED blinking
CREATE_FSM(builtin_led, 0);
void built_in_led_update();



#endif  // __MAIN_H__