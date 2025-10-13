#ifndef __PUMP_MANAGER_H__
#define __PUMP_MANAGER_H__

#include "Scheduler.h"
#include "L298NSingle.h"
#include "LEDSimple.h"
#include "FSMMacroV2.h"

class PumpManager: public SchedulerFSM {
public:
    PumpManager(L298NSingle *register_pump, LED *led_pump_indicator);
    ~PumpManager();

    void off();

    void on();
    void forward_on();
    void backward_on();

    void on_pulse(int on_duration, int off_duration);
    void forward_on_pulse(int on_duration, int off_duration);
    void backward_on_pulse(int on_duration, int off_duration);

    void reset_pulse_count();
    unsigned long get_pulse_count() const;
    
protected:
    void event();

private:
    L298NSingle *registered_pump;
    LED *led_pump;
    int pump_on_duration;
    int pump_off_duration;
    bool is_forward_on; //  both off means regular on
    bool is_backward_on;
    unsigned long pulse_count;

    CREATE_FSM(pump_manager, 0);
};


#endif  //  __PUMP_MANAGER_H__