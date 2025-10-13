#include "PumpManager.h"

PumpManager::PumpManager(L298NSingle *register_pump, LED *led_pump_indicator)
    : SchedulerFSM(1)
    , registered_pump(register_pump), led_pump(led_pump_indicator)
    , pump_on_duration(0), pump_off_duration(0)
    , is_forward_on(true), is_backward_on(false), pulse_count(0) {

    this->off();
}

PumpManager::~PumpManager() {
}

void PumpManager::off() {
    this->registered_pump->off();
    this->led_pump->off();
    this->fsm_finished();
}

void PumpManager::on() {
    this->registered_pump->on();
    this->led_pump->on();
    this->fsm_finished();
}

void PumpManager::forward_on() {
    this->registered_pump->forward_on();
    this->led_pump->on();
    this->fsm_finished();
}

void PumpManager::backward_on() {
    this->registered_pump->backward_on();
    this->led_pump->on();
    this->fsm_finished();
}

void PumpManager::on_pulse(int on_duration, int off_duration) {
    this->pump_on_duration = on_duration;
    this->pump_off_duration = off_duration;
    this->fsm_start();
}

void PumpManager::forward_on_pulse(int on_duration, int off_duration) {
    this->is_forward_on = true;
    this->is_backward_on = false;
    this->on_pulse(on_duration, off_duration);
}

void PumpManager::backward_on_pulse(int on_duration, int off_duration) {
    this->is_forward_on = false;
    this->is_backward_on = true;
    this->on_pulse(on_duration, off_duration);
}

void PumpManager::reset_pulse_count() {
    this->pulse_count = 0;
}

unsigned long PumpManager::get_pulse_count() const {
    return this->pulse_count;
}

void PumpManager::event() {
    SETUP_FSM_FUNCTION(pump_manager);

    STATE(0) {
        this->registered_pump->off();
        this->led_pump->off();

        SLEEP_TO_NEXT(this->pump_off_duration);
    }

    STATE(1) {
        if (this->is_forward_on) {
            this->registered_pump->forward_on();
        } else if (this->is_backward_on) {
            this->registered_pump->backward_on();
        } else {
            this->registered_pump->on();
        }
        this->pulse_count++;

        this->led_pump->on();

        SLEEP_TO(this->pump_on_duration, 0);
    }
}
