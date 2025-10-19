#ifndef __PUMP_MANAGER_H__
#define __PUMP_MANAGER_H__

#include "Scheduler.h"
#include "L298NSingle.h"
#include "LEDSimple.h"
#include "FSMMacroV2.h"
#include "PumpScheduler.h"

class PumpSchduler;     //  forward declaration, we don't know which header file will be initialized first

class PumpManager: public SchedulerFSM {
public:
    PumpManager(L298NSingle *register_pump, LED *led_pump_indicator, PumpSchduler *pump_scheduler);
    ~PumpManager();

    L298NSingle *get_registered_pump() const;
    LED *get_pump_led() const;
    PumpSchduler *get_pump_scheduler() const;

    void off();
    void on();
    void forward_on();
    void backward_on();

    void override_off();
    void override_on();
    void override_forward_on();
    void override_backward_on();
    void end_override();

    void start_schedule(long brewing_time_ms, long brewing_volumn_ml);
    void end_schedule();

protected:
    void event();

private:
    L298NSingle *registered_pump;
    LED *pump_led;
    PumpSchduler *pump_scheduler;

    //  temporary disable the scheduler, but schduler is still ticking
    bool is_pump_override_on;

    CREATE_FSM(pump_manager, 0);
};




#endif  //  __PUMP_MANAGER_H__