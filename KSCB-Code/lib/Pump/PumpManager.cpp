#include "PumpManager.h"

PumpManager::PumpManager(L298NSingle *register_pump, LED *led_pump_indicator, PumpSchduler *pump_scheduler)
    : SchedulerFSM(1)
    , registered_pump(register_pump), pump_led(led_pump_indicator), pump_scheduler(pump_scheduler)
    , is_pump_override_on(false) {

    this->enable_accurate_mode();
    this->off();
}

PumpManager::~PumpManager() {
}

L298NSingle *PumpManager::get_registered_pump() const {
    return this->registered_pump;
}

LED *PumpManager::get_pump_led() const {
    return this->pump_led;
}

PumpSchduler *PumpManager::get_pump_scheduler() const {
    return this->pump_scheduler;
}

void PumpManager::off() {
    this->registered_pump->off();
    this->pump_led->off();
}

void PumpManager::on() {
    this->registered_pump->on();
    this->pump_led->on();
}

void PumpManager::forward_on() {
    this->registered_pump->forward_on();
    this->pump_led->on();
}

void PumpManager::backward_on() {
    this->registered_pump->backward_on();
    this->pump_led->on();
}

void PumpManager::override_off() {
    this->is_pump_override_on = true;
    this->off();
}

void PumpManager::override_on() {
    this->is_pump_override_on = true;
    this->on();
}

void PumpManager::override_forward_on() {
    this->is_pump_override_on = true;
    this->forward_on();
}

void PumpManager::override_backward_on() {
    this->is_pump_override_on = true;
    this->backward_on();
}

void PumpManager::end_override() {
    this->is_pump_override_on = false;
}

void PumpManager::start_schedule(long brewing_time_ms, long brewing_volumn_ml) {
    this->pump_scheduler->start_schedule(brewing_time_ms, brewing_volumn_ml);
    this->fsm_start();
    RESET_FSM(pump_manager)
}

void PumpManager::end_schedule() {
    this->fsm_finished();
    this->off();
}

bool PumpManager::is_brewing_finished() const {
    return this->pump_scheduler->is_finished();
}

long PumpManager::get_brewing_volumn_injected_ml() const {
    return this->pump_scheduler->get_brewing_volumn_injected_ul() / 1000;
}

long PumpManager::get_brewing_volumn_remaining_ml() const {
    return this->pump_scheduler->get_brewing_volumn_remaining_ul() / 1000;
}

long PumpManager::get_brewing_time_elapsed_ms() const {
    return this->pump_scheduler->get_brewing_time_elapsed_ms();
}

long PumpManager::get_brewing_time_remaining_ms() const {
    return this->pump_scheduler->get_brewing_time_remaining_ms();
}

long PumpManager::get_brewing_time_elapsed_mim() const {
    return this->get_brewing_time_elapsed_ms() / 1000 / 60;
}

long PumpManager::get_brewing_time_remaining_mim() const {
    return this->get_brewing_time_remaining_ms() / 1000 / 60;
}

void PumpManager::event() {
    SETUP_FSM_FUNCTION(pump_manager);

    //  pump off state
    STATE(0) {
        if (!this->is_pump_override_on) {
            this->off();
        }

        if (this->pump_scheduler->is_finished()) {
            this->fsm_finished();
        } else {
            SLEEP_TO_NEXT(this->pump_scheduler->get_off_duration_ms());
        }
    }

    //  pump on state
    STATE(1) {
        if (!this->is_pump_override_on) {
            this->forward_on();
        }

        long on_duration_ms = this->pump_scheduler->get_on_duration_ms();
        this->pump_scheduler->next_cycle();
        SLEEP_TO(on_duration_ms, 0);
    }
}

