#include "PumpScheduler.h"

//  +---------------------------------------------------------------------------------------------+
//  |                                         PumpSchduler                                        |
//  +---------------------------------------------------------------------------------------------+

PumpSchduler::PumpSchduler()
    : scheduler_starting_time_ms(millis()), is_finished_(true)
    , total_brewing_volumn_ul(0), brewing_volumn_remaining_ul(0), total_brewing_time_ms(0) {
}

PumpSchduler::~PumpSchduler() {
}

long PumpSchduler::get_on_duration_ms() const {
    return 500;
}

long PumpSchduler::get_off_duration_ms() const {
    return 9500;
}

void PumpSchduler::next_cycle() {
    if (this->get_brewing_time_elapsed_ms() <= 0) {
        this->set_brewing_finished();
    }
}

void PumpSchduler::start_schedule(long brewing_time_ms, long brewing_volumn_ml) {
    this->reset_timer();
    this->is_finished_ = false;
    this->total_brewing_time_ms = brewing_time_ms;
    this->total_brewing_volumn_ul = brewing_volumn_ml * 1000;
    this->brewing_volumn_remaining_ul = brewing_volumn_ml * 1000;
}

long PumpSchduler::get_brewing_time_elapsed_ms() const {
    return millis() - this->scheduler_starting_time_ms;
}

long PumpSchduler::get_brewing_time_remaining_ms() const {
    return this->total_brewing_time_ms - this->get_brewing_time_elapsed_ms();
}

long PumpSchduler::get_brewing_volumn_injected_ul() const {
    return this->total_brewing_volumn_ul - this->brewing_volumn_remaining_ul;
}

long PumpSchduler::get_brewing_volumn_remaining_ul() const {
    return this->brewing_volumn_remaining_ul;
}

bool PumpSchduler::is_finished() const {
    return this->is_finished_;
}

void PumpSchduler::set_brewing_finished() {
    this->is_finished_ = true;
}

void PumpSchduler::reset_timer() {
    this->scheduler_starting_time_ms = millis();
}

//  +---------------------------------------------------------------------------------------------+
//  |                                PumpScheduler_ConstantNumPulse                               |
//  +---------------------------------------------------------------------------------------------+

PumpScheduler_ConstantNumPulse::PumpScheduler_ConstantNumPulse(int n_pulse)
    : PumpSchduler()
    , n_pulse(n_pulse), pulse_injected_volumn_ul(0), pulse_on_duration_ms(0), pulse_off_duration_ms(0) {
}

PumpScheduler_ConstantNumPulse::~PumpScheduler_ConstantNumPulse() {
}

long PumpScheduler_ConstantNumPulse::get_on_duration_ms() const {
    return this->pulse_on_duration_ms;
}

long PumpScheduler_ConstantNumPulse::get_off_duration_ms() const {
    return this->pulse_off_duration_ms;
}

void PumpScheduler_ConstantNumPulse::next_cycle() {
    PumpSchduler::next_cycle();
    this->brewing_volumn_remaining_ul -= this->pulse_injected_volumn_ul;
}

void PumpScheduler_ConstantNumPulse::start_schedule(long brewing_time_ms, long brewing_volumn_ml) {
    PumpSchduler::start_schedule(brewing_time_ms, brewing_volumn_ml);

    this->pulse_injected_volumn_ul = this->total_brewing_volumn_ul / this->n_pulse;
    
    long cycle_period_ms = this->total_brewing_time_ms / this->n_pulse;
    this->pulse_on_duration_ms = PumpStats::pulse_ul2ms(this->pulse_injected_volumn_ul);
    this->pulse_off_duration_ms = cycle_period_ms - this->pulse_on_duration_ms;
}
