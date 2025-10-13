#include "DisplayManager.h"

DisplayManager::DisplayManager(TM1637Display *register_display, LED *unit_ml, LED *unit_hr_min, uint8_t dot_bytecode)
    : Scheduler(1000000UL)   //  1s
    , registered_display(register_display), unit_ml(unit_ml), unit_hr_min(unit_hr_min)
    , dot_bytecode(dot_bytecode), is_dot_flashing(false), hours(0), mins(0), is_dot_on(0)
    , showing_volumn(false), showing_time(false), showing_text_segment(false) {

    registered_display->setBrightness(7);
}

DisplayManager::~DisplayManager() {
}

void DisplayManager::show_time_min(unsigned int mins, bool show_dots, bool flashing_dot) {
    this->show_time_hour_min(mins / 60, mins % 60, show_dots, flashing_dot);
}

void DisplayManager::show_time_hour_min(unsigned int hours, unsigned int mins, bool show_dots, bool flashing_dot) {
    if (show_dots) {
        this->registered_display->showNumberDecEx(hours * 100 + mins, this->dot_bytecode, true);
    } else {
        this->registered_display->showNumberDecEx(hours * 100 + mins, 0, true);
    }

    this->unit_hr_min->on();
    this->unit_ml->off();
    this->hours = hours;
    this->mins = mins;
    this->is_dot_on = show_dots;
    this->is_dot_flashing = flashing_dot;
    
    this->showing_volumn = false;
    this->showing_time = true;
    this->showing_text_segment = false;
}

void DisplayManager::update_time_min(unsigned int mins) {
    this->update_time_hour_min(mins / 60, mins % 60);
}

void DisplayManager::update_time_hour_min(unsigned int hours, unsigned int mins) {
    this->hours = hours;
    this->mins = mins;
}

void DisplayManager::show_segment(uint8_t *segment_encoding) {
    this->registered_display->setSegments(segment_encoding);

    this->unit_hr_min->off();
    this->unit_ml->off();
    this->is_dot_flashing = false;

    this->showing_volumn = false;
    this->showing_time = false;
    this->showing_text_segment = true;
}

void DisplayManager::show_ml(int num) {
    if (num > 9999) {
        num = 9999;
    } 
    if (num < -999) {
        num = -999;
    }
    this->registered_display->showNumberDec(num);

    this->unit_hr_min->off();
    this->unit_ml->on();
    this->is_dot_flashing = false;

    this->showing_volumn = true;
    this->showing_time = false;
    this->showing_text_segment = false;
}

TM1637Display *DisplayManager::get_display() const {
    return this->registered_display;
}

bool DisplayManager::is_showing_ml() const {
    return showing_volumn;
}

bool DisplayManager::is_showing_hr_min() const {
    return showing_time;
}

bool DisplayManager::is_showing_text_segment() const {
    return showing_text_segment;
}

void DisplayManager::event() {
    if (this->is_dot_flashing) {
        this->show_time_hour_min(this->hours, this->mins, !this->is_dot_on, this->is_dot_flashing);
    }
}
