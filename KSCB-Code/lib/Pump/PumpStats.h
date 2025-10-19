#ifndef __PUMP_STATES_H__
#define __PUMP_STATES_H__

namespace PumpStats {
    //  minisecond to microliter
    long ms2ul(long ms);

    //  minisecond to mililiter
    long ms2ml(long ms);

    //  given pulse on-duration (ms) for given volumn in microliter
    long pulse_ul2ms(long ul);

    //  given pulse volumn (microliter) for given volumn on duration(ms)
    long pulse_ms2ul(long ms);
}

#endif