#include "PumpStats.h"

const double CONTINUOUS_PUMP_FLOW_RATE_ml_per_ms = 0.001405257098765432;

const double PULSE_PUMP_FLOW_slope_ml_per_ms = 0.0013847182802797895 ;
const double PULSE_PUMP_FLOW_intercept_ml    = 0.01216288737244074   ;

long PumpStats::ms2ul(long ms) {
    return (long)(ms * CONTINUOUS_PUMP_FLOW_RATE_ml_per_ms * 1000.);
}

long PumpStats::ms2ml(long ms) {
    return (long)(ms * CONTINUOUS_PUMP_FLOW_RATE_ml_per_ms);
}

long PumpStats::pulse_ul2ms(long ul) {
    return (long) ((ul / 1000. - PULSE_PUMP_FLOW_intercept_ml) / PULSE_PUMP_FLOW_slope_ml_per_ms);
}

long PumpStats::pulse_ms2ul(long ms) {
    return (long) ((PULSE_PUMP_FLOW_slope_ml_per_ms * ms + PULSE_PUMP_FLOW_intercept_ml) * 1000.);
}
