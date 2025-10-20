#include "main.h"

#ifdef MAIN

void setup() {}

void loop() {
    main_loop_update();
    built_in_led_update();
    display_manager.update();
    pump_manager.update();

    delay(FSM_DELAY_MS);
}


//  +---------------------------------------------------------------------------------------------+
//  |                                          Main Loop                                          |
//  +---------------------------------------------------------------------------------------------+


void main_loop_update() {
    SETUP_FSM_FUNCTION(main_loop);

    STATE(ML_init) {
        display_manager.show_segment(SEG_PCD);
        SLEEP_TO(1000, ML_init_2);
    }

    STATE(ML_init_2) {
        display_manager.show_segment(SEG_ZLIU);
        SLEEP_TO(1500, ML_idle_prep);
    }

    //  ------- Idle -------
    STATE(ML_idle_prep) {
        pump_manager.end_schedule();
        display_manager.show_segment(SEG_empty_str);
        SLEEP_TO(500, ML_idle_prep_2);
    }

    STATE(ML_idle_prep_2) {
        pump_manager.end_schedule();
        display_manager.show_segment(SEG_PREP);
        SLEEP_TO(1000, ML_idle_prep_3);
    }

    STATE(ML_idle_prep_3) {
        //  Setting up menu
        is_setting_volumn = false;
        is_setting_time = true;

        refresh_display_menu();
        TO(ML_idle);
    }

    STATE(ML_idle) {
        refresh_display_menu_scheduled();
        WHEN(btn_change.is_pressed()) TO(ML_C0);
        WHEN(btn_plus  .is_pressed()) TO(ML_P0);
        WHEN(btn_minus .is_pressed()) TO(ML_M0);
        WHEN(btn_start .is_pressed()) { 
            display_manager.show_segment(SEG_BREW);
            RESET_TIMER(main_loop); 

            TO(ML_S0); 
        }
    }

    STATE(ML_C0) {
        WHEN(btn_change.is_released()) {
            //  switching setting item
            if (is_setting_volumn) {
                is_setting_volumn = false;
                is_setting_time = true;

            } else if (is_setting_time) {
                is_setting_volumn = true;
                is_setting_time = false;
            }

            refresh_display_menu();
            TO(ML_idle)
        }
    }

    STATE(ML_P0) {
        WHEN(btn_plus.is_released()) {
            //  plus setting item
            if (is_setting_volumn) {
                if (brewing_volumn_ml + BREWING_VOLUMN_INCREMENT_ML < 9999) {
                    brewing_volumn_ml += BREWING_VOLUMN_INCREMENT_ML;
                }
            } else if (is_setting_time) {
                if (brewing_time_mins + BREWING_TIME_INCREMENT_MINS < 99 * 60) {
                    brewing_time_mins += BREWING_TIME_INCREMENT_MINS;
                }
            }

            refresh_display_menu();
            TO(ML_idle)
        }
    }

    STATE(ML_M0) {
        WHEN(btn_minus.is_released()) {
            //  minus setting item
            if (is_setting_volumn) {
                if (brewing_volumn_ml - BREWING_VOLUMN_INCREMENT_ML > 0) {
                    brewing_volumn_ml -= BREWING_VOLUMN_INCREMENT_ML;
                }
            } else if (is_setting_time) {
                if (brewing_time_mins - BREWING_TIME_INCREMENT_MINS > 0) {
                    brewing_time_mins -= BREWING_TIME_INCREMENT_MINS;
                }
            }

            refresh_display_menu();
            TO(ML_idle)
        }
    }

    STATE(ML_S0) {
        WHEN(btn_start.is_released()) TO(ML_brew_prep);
        WHEN(IS_TIME_ELAPSED(main_loop, 3000)) {
            display_manager.show_segment(SEG_CALI);
            TO(ML_S0A);
        }
    }

    STATE(ML_S0A) {
        WHEN(btn_start.is_released()) TO(ML_weight_prep);
    }

    //  ------- Brewing -------

    STATE(ML_brew_prep) {
        //  setup brewing schedule
        long brewing_total_time_ms = brewing_time_mins * 60 * 1000;

        is_showing_brewing_time_left = true;
        is_showing_brewing_volumn_left = false;

        pump_manager.start_schedule(brewing_total_time_ms, brewing_volumn_ml);
        //  manually trigger time once to activate flashing dots
        display_manager.show_time_min(pump_manager.get_brewing_time_remaining_mim(), true, true);
        refresh_display_brewing();

        TO(ML_brew)
    }

    STATE(ML_brew) {
        refresh_display_brewing_scheduled();

        WHEN(btn_change.is_pressed()) TO(ML_C1);
        WHEN(btn_plus  .is_pressed()) { pump_manager.override_forward_on();  TO(ML_P1); }
        WHEN(btn_minus .is_pressed()) { pump_manager.override_backward_on(); TO(ML_M1); }
        WHEN(btn_start .is_pressed()) { 
            display_manager.show_segment(SEG_HOLD);
            RESET_TIMER(main_loop);     

            TO(ML_S1); 
        }
        WHEN(pump_manager.is_brewing_finished()) { 
            display_manager.show_segment(SEG_FIN);
            pump_manager.end_schedule(); 
            pump_manager.backward_on();

            SLEEP_TO(ML_finish, 15000); 
        }
    }

    STATE(ML_C1) { 
        refresh_display_brewing_scheduled();

        WHEN(btn_change.is_released()) {
            //  switching setting item
            if (is_showing_brewing_volumn_left) {
                is_showing_brewing_volumn_left = false;
                is_showing_brewing_time_left = true;

            } else if (is_showing_brewing_time_left) {
                is_showing_brewing_volumn_left = true;
                is_showing_brewing_time_left = false;
            }

            refresh_display_brewing();
            TO(ML_brew)
        }
    }

    STATE(ML_P1) {
        refresh_display_brewing_scheduled();

        WHEN(btn_plus.is_released()) {
            pump_manager.override_off();
            pump_manager.end_override();

            refresh_display_brewing();
            TO(ML_brew)
        }
    }

    STATE(ML_M1) {
        refresh_display_brewing_scheduled();

        WHEN(btn_minus.is_released()) {
            pump_manager.override_off();
            pump_manager.end_override();

            refresh_display_brewing();
            TO(ML_brew)
        }
    }

    STATE(ML_S1) {
        WHEN(btn_start.is_released()) {
            refresh_display_brewing();

            TO(ML_brew);
        }
        WHEN(IS_TIME_ELAPSED(main_loop, 3000)) { 
            display_manager.show_segment(SEG_STOP);
            pump_manager.end_schedule(); 

            TO(ML_S1A); 
        }
    }

    STATE(ML_S1A) {
        WHEN(btn_start.is_released()) TO(ML_idle_prep);
    }

    STATE(ML_finish) {
        pump_manager.off();
        led_brew.off();
        led_fin.on();

        TO(ML_finish_2)
    }

    STATE(ML_finish_2) {
        WHEN(btn_start.is_pressed()) TO(ML_finish_A);
    }

    STATE(ML_finish_A) {
        WHEN(btn_start.is_released()) TO(ML_idle_prep);
    }

    //  ------- Weighting -------

    STATE(ML_weight_prep) {
        curr_volumn_ul = 0;
        refresh_display_meansuring(curr_volumn_ul);

        TO(ML_weight)
    }

    STATE(ML_weight) {
        refresh_display_meansuring_scheduled(curr_volumn_ul);

        WHEN(btn_change.is_pressed()) TO(ML_C2);

        WHEN(btn_plus  .is_pressed()) { 
            pump_manager.override_forward_on();  
            RESET_TIMER(main_loop_measuring); 
            
            TO(ML_P2); 
        }

        WHEN(btn_minus .is_pressed()) { 
            pump_manager.override_backward_on(); 
            RESET_TIMER(main_loop_measuring); 

            TO(ML_M2); 
        }

        WHEN(btn_start .is_pressed()) TO(ML_S2);
    }

    STATE(ML_C2) { 
        WHEN(btn_change.is_released()) {
            curr_volumn_ul = 0;
            refresh_display_meansuring(curr_volumn_ul);

            TO(ML_weight)
        }
    }

    STATE(ML_P2) {
        refresh_display_meansuring_scheduled(
            curr_volumn_ul + PumpStats::ms2ul(TIME_ELAPSED(main_loop_measuring))
        );

        WHEN(btn_plus.is_released()) {
            pump_manager.override_off();
            curr_volumn_ul += PumpStats::ms2ul(TIME_ELAPSED(main_loop_measuring));
            pump_manager.end_override();
            refresh_display_meansuring(curr_volumn_ul);

            TO(ML_weight)
        }
    }

    STATE(ML_M2) {
        refresh_display_meansuring_scheduled(
            curr_volumn_ul - PumpStats::ms2ul(TIME_ELAPSED(main_loop_measuring))
        );

        WHEN(btn_minus.is_released()) {
            pump_manager.override_off();
            curr_volumn_ul -= PumpStats::ms2ul(TIME_ELAPSED(main_loop_measuring));
            pump_manager.end_override();
            refresh_display_meansuring(curr_volumn_ul);

            TO(ML_weight)
        }
    }

    STATE(ML_S2) {
        WHEN(btn_start.is_released()) TO(ML_idle_prep);
    }
}

//  +---------------------------------------------------------------------------------------------+
//  |                                       Helper Function                                       |
//  +---------------------------------------------------------------------------------------------+

void refresh_display_menu() {
    if (is_setting_volumn) {
        display_manager.show_ml(brewing_volumn_ml);
    } else if (is_setting_time) {
        display_manager.show_time_min(brewing_time_mins, true, false);
    }

    led_brew.off();
    led_fin.off();
}

void refresh_display_menu_scheduled() {
    if (!IS_TIME_ELAPSED(refresh_display_menu_scheduled_timer, 500)) {
        return;
    } 
    RESET_TIMER(refresh_display_menu_scheduled_timer); 

    refresh_display_menu();
}

void refresh_display_brewing() {
    if (is_showing_brewing_time_left) {
        long time_remaining_min = pump_manager.get_brewing_time_remaining_mim();
        if (time_remaining_min < 0) { 
            time_remaining_min = 0;
            display_manager.show_time_min(0, true, false);
        }

        if (display_manager.is_showing_hr_min()) {
            display_manager.update_time_min(time_remaining_min);
        } else {
            //  now showing the time, then call the function to reset
            display_manager.show_time_min(time_remaining_min, true, true);
        }

    } else if (is_showing_brewing_volumn_left) {
        display_manager.show_ml(pump_manager.get_brewing_volumn_remaining_ml());
    }

    led_brew.on();
    led_fin.off();
}

void refresh_display_brewing_scheduled() {
    if (!IS_TIME_ELAPSED(refresh_display_brewing_scheduled_timer, 500)) {
        return;
    } 
    RESET_TIMER(refresh_display_brewing_scheduled_timer); 

    refresh_display_brewing();
}

void refresh_display_meansuring(long volumn_ul) {
    display_manager.show_ml(volumn_ul / 1000);
    led_brew.on();
    led_fin.on();
}

void refresh_display_meansuring_scheduled(long volumn_ul) {
    if (!IS_TIME_ELAPSED(refresh_display_meansuring_scheduled_timer, 100)) {
        return;
    } 
    RESET_TIMER(refresh_display_meansuring_scheduled_timer); 

    refresh_display_meansuring(volumn_ul);
}

//  +---------------------------------------------------------------------------------------------+
//  |                                        Autonomous                                           |
//  +---------------------------------------------------------------------------------------------+

void built_in_led_update() {
    SETUP_FSM_FUNCTION(builtin_led);

    STATE(0) {
        led_builtin.on();
        SLEEP_TO_NEXT(1000);
    }

    STATE(1) {
        led_builtin.off();
        SLEEP_TO(1000, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(CALIBRATE)

#define ON_DURATION_MS 200
#define OFF_DURATION_MS 500
#define N_PULSE 0

void setup() {
    display.showNumberDec(0);
    
    for (int i = 0; i < N_PULSE; i++) {
        delay(OFF_DURATION_MS);
        pump.forward_on();
        led_motor.on();
        delay(ON_DURATION_MS);
        pump.off();
        led_motor.off();

        display.showNumberDec(i + 1);
    }

    led_fin.on();
}

void loop() {
    if (btn_plus.is_pressed()) {
        pump.forward_on();
        led_motor.on();
    } else if (btn_minus.is_pressed()) {
        pump.backward_on();
        led_motor.on();
    } else {
        pump.off();
        led_motor.off();
    }
    delay(100);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#elif defined(DEBUG)

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

