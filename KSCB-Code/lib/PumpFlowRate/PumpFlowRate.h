#ifndef __PUMP_FLOW_RATE_H__
#define __PUMP_FLOW_RATE_H__

//  policy
#define PUNP_CALCULATION_CONST_FLOWRATE_CONST_PULSE
// #define PUMP_CALCULATION_FIRST_ORDER_PULSE_ADJUSTMENT

long get_pump_on_duration_ms(long brewing_volumn_ml, long brewing_time_mins);
long get_pump_off_duration_ms(long brewing_volumn_ml, long brewing_time_mins);

#endif