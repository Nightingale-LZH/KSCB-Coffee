#ifndef __PUMP_SCHEDULER_H__
#define __PUMP_SCHEDULER_H__

#include "Arduino.h"
#include "PumpManager.h"

class PumpManager;  //  forward declaration, we don't know which header file will be initialized first

//  base class, constant pulse with no adjustment
class PumpSchduler {
public:
    PumpSchduler();
    ~PumpSchduler();

    friend PumpManager;
    
protected:

    long get_on_duration_ms() const;
    long get_off_duration_ms() const;
    void next_cycle();
    
    //  this should be called by pump manager
    void start_schedule(long brewing_time_ms, long brewing_volumn_ml);
    long get_time_elapsed_ms() const;
    long get_time_elapsed_min() const;
    
    bool is_finished() const;
    void set_brewing_finished();
    
    void reset_timer();

    long scheduler_starting_time_ms;
    bool is_finished_;
    long remaining_brewing_volumn_ul;
    long total_brewing_time_ms;
};

#endif