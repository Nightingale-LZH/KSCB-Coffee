#ifndef __LED_SIMPLE_H__
#define __LED_SIMPLE_H__

#include "Arduino.h"

class LED {
public:
    explicit LED(int pin_led);
    ~LED();

    void on();
    void off();
private:
    int pin_led;
};


#endif  //  __LED_SIMPLE_H__