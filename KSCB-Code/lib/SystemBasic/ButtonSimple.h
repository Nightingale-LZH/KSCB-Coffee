#ifndef __BUTTON_SIMPLE_H__
#define __BUTTON_SIMPLE_H__

#include "Arduino.h"

//  default as pull-up
class Button {
public:
    explicit Button(int pin_btn);
    ~Button();

    bool is_pressed();
    bool is_released();
private:
    int pin_btn;
};


#endif  //  __BUTTON_SIMPLE_H__