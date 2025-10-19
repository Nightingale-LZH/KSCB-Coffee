//  +-------------------------------------------------------------------------+
//  | This is the library that help record time in Arduino Envrionment.       |
//  | Additionally, users can register time-sensitive event and automatically |
//  | invoke them when checking status.                                       |
//  |                                                                         |
//  | Users can set up many interesting behavior like periodic timer using    |
//  | this macro library.                                                     |
//  |                                                                         |
//  | Author:       Zihui(Andy) Liu                                           |
//  | Email:        liuzihui@uw.edu, salzhx@gmail.com                         |
//  | Last Update:  Oct 10, 2025                                              |
//  |                                                                         |
//  |                                    Check README.md for more information |
//  +-------------------------------------------------------------------------+

#ifndef __TIMER_MACRO_V2
#define __TIMER_MACRO_V2

#include "Arduino.h"

//  Interfaces
//  
//  CREATE_TIMER(tsName)
//
//  RESET_TIMER(tsName)
//  TIME_ELAPSED(tsName)
//  IS_TIME_ELAPSED(tsName, time_elapsed)



//  ---------- Helper Macros ---------- //
#define TIMER_VARIABLE_TYPE unsigned long

//  ---------- TS Initialization ---------- //
#define CREATE_TIMER(tsName) \
TIMER_VARIABLE_TYPE TIMER_ ## tsName ## _timeStamp = millis(); 

//  ---------- Actions ---------- //
#define RESET_TIMER(tsName) \
TIMER_ ## tsName ## _timeStamp = millis();

#define TIME_ELAPSED(tsName) (millis() - TIMER_ ## tsName ## _timeStamp)

//  return true if time has elapsed for given amount, prevent overflow
#define IS_TIME_ELAPSED(tsName, time_elapsed_ms) (TIMER_ ## tsName ## _timeStamp + time_elapsed_ms <= millis())


#endif  //  __TIMER_MACRO_V2