#include "PumpFlowRate.h"

#ifdef PUNP_CALCULATION_CONST_FLOWRATE_CONST_PULSE

const double flow_rate_ml_per_ms = 0.00141177;
const long   n_pulse = 1000;

long cycle_period_ms(long brewing_volumn_ml, long brewing_time_mins) {
    long brewing_time_ms = brewing_time_mins * 60 * 1000;
    return brewing_time_ms / n_pulse;
}

long cycle_pump_on_duration_ms(long brewing_volumn_ml, long brewing_time_mins) {
    return (long) ((1.0 * brewing_volumn_ml) / n_pulse / flow_rate_ml_per_ms);
}

long get_pump_on_duration_ms(long brewing_volumn_ml, long brewing_time_mins) {
    return cycle_pump_on_duration_ms(brewing_volumn_ml, brewing_time_mins);
}

long get_pump_off_duration_ms(long brewing_volumn_ml, long brewing_time_mins) {
    long cycle_period = cycle_period_ms(brewing_volumn_ml, brewing_time_mins);
    long on_duration = cycle_period_ms(brewing_volumn_ml, brewing_time_mins);
    return (cycle_period - on_duration > 0)? cycle_period - on_duration : 0;
}

#elif defined(PUMP_CALCULATION_FIRST_ORDER_PULSE_ADJUSTMENT)

long get_pump_on_duration_ms(long brewing_volumn_ml, long brewing_time_mins) {
    return 0;
}

long get_pump_off_duration_ms(long brewing_volumn_ml, long brewing_time_mins) {
    return 0;
}

#endif