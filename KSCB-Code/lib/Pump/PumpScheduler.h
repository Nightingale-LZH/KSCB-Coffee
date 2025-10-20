#ifndef __PUMP_SCHEDULER_H__
#define __PUMP_SCHEDULER_H__

#include "Arduino.h"
#include "PumpManager.h"
#include "PumpStats.h"

class PumpManager;  //  forward declaration, we don't know which header file will be initialized first

//  +---------------------------------------------------------------------------------------------+
//  |                                         PumpSchduler                                        |
//  +---------------------------------------------------------------------------------------------+
//  base class, constant pulse with no adjustment
class PumpSchduler {
public:
    friend PumpManager;

    PumpSchduler();
    ~PumpSchduler();

protected:
    virtual long get_on_duration_ms() const;
    virtual long get_off_duration_ms() const;
    virtual void next_cycle();
    
    //  this should be called by pump manager
    virtual void start_schedule(long brewing_time_ms, long brewing_volumn_ml);

    long get_brewing_time_elapsed_ms() const;
    long get_brewing_time_remaining_ms() const;
    long get_brewing_volumn_injected_ul() const;
    long get_brewing_volumn_remaining_ul() const;

    bool is_finished() const;
    void set_brewing_finished();
    
    void reset_timer();

    long scheduler_starting_time_ms;
    bool finished;
    long total_brewing_volumn_ul;
    long brewing_volumn_remaining_ul;
    long total_brewing_time_ms;
};

//  +---------------------------------------------------------------------------------------------+
//  |                                PumpScheduler_ConstantNumPulse                               |
//  +---------------------------------------------------------------------------------------------+
//  uniform injection with constant number of pulses
class PumpScheduler_ConstantNumPulse: public PumpSchduler {
public:
    explicit PumpScheduler_ConstantNumPulse(int n_pulse);
    ~PumpScheduler_ConstantNumPulse();

protected:
    long get_on_duration_ms() const override;
    long get_off_duration_ms() const override;
    void next_cycle() override;
    void start_schedule(long brewing_time_ms, long brewing_volumn_ml) override;

private:
    int n_pulse;
    long pulse_injected_volumn_ul;
    long pulse_on_duration_ms;
    long pulse_off_duration_ms;
};

#endif