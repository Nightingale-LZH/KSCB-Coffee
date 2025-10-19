#ifndef __DISPLAY_MANAGER_H__
#define __DISPLAY_MANAGER_H__

#include "TM1637Display.h"
#include "LEDSimple.h"
#include "Scheduler.h"

class DisplayManager: public Scheduler {
public:
    DisplayManager(TM1637Display *register_display, LED *unit_ml, LED *unit_hr_min, uint8_t dot_bytecode);
    ~DisplayManager();

    //  e.g. 90 mins will auto convert to 1:30
    void show_time_min(unsigned int mins, bool show_dots, bool flashing_dot);

    void show_time_hour_min(unsigned int hours, unsigned int mins, bool show_dots, bool flashing_dot);

    //  change the internal variable
    void update_time_min(unsigned int mins);
    void update_time_hour_min(unsigned int hours, unsigned int mins);

    //  segment_encoding is array with 4 elements
    void show_segment(uint8_t *segment_encoding);

    //  support both positive and negative number
    void show_ml(int num);

    TM1637Display *get_display() const;

    bool is_showing_ml() const;
    bool is_showing_hr_min() const;
    bool is_showing_text_segment() const;

protected:
    void event() override;

private:
    TM1637Display *registered_display;
    LED *unit_ml;
    LED *unit_hr_min;
    uint8_t dot_bytecode;

    bool is_dot_flashing; 
    unsigned int hours;
    unsigned int mins;
    bool is_dot_on; 

    bool showing_volumn;
    bool showing_time;
    bool showing_text_segment;
};

#endif  //  __DISPLAY_MANAGER_H__