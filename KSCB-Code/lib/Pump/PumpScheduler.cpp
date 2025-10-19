#include "PumpScheduler.h"


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
    return 95000;
}

void PumpSchduler::next_cycle() {
}

void PumpSchduler::start_schedule(long brewing_time_ms, long brewing_volumn_ml) {
    this->reset_timer();
    this->is_finished_ = false;
    this->total_brewing_time_ms = brewing_time_ms;
    this->total_brewing_volumn_ul = brewing_volumn_ml * 1000;
    this->brewing_volumn_remaining_ul = 0;
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
