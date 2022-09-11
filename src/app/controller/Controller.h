#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <FreeRTOS.h>
#include <queue.h>

#include <Arduino.h>
#include <FlowBendingMachine.h>
#include <device/Keypad_4x3/Keypad_4x3.h>

class Controller {
private:
    QueueHandle_t _q;
    FlowBendingMachine* _machine;
    Keypad_4x3* _keypad;
public:
    void setup();
    void loop();
};


extern Controller flowBendingMachineController;

#endif