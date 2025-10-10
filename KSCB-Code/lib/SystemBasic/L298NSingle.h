#ifndef __L298NSINGLE_H__
#define __L298NSINGLE_H__

#include "Arduino.h"

class L298NSingle {
public:
    L298NSingle(int pin_ena, int pin_in1, int pin_in2);
    ~L298NSingle();

    void forward_on();
    void backward_on();
    void on();
    void off();
private:
    int pin_ena;
    int pin_in1;
    int pin_in2;
};

#endif  //  __L298NSINGLE_H__