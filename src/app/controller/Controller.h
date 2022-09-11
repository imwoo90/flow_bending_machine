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
    Controller() {}
public:
    static Controller* getInstance() {
        static Controller singleton_instance;
        return &singleton_instance;
    }
    void setup();
    void loop();
};

#endif