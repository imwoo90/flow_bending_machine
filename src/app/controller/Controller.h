#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <FreeRTOS.h>
#include <queue.h>

#include <Arduino.h>
#include <FlowVendingMachine.h>
#include <device/Keypad_4x3/Keypad_4x3.h>
#include <device/BanknoteReader/OBH_K03S.h>
#include <device/Relay/Relay.h>

class Controller {
private:
    QueueHandle_t _q;
    FlowVendingMachine* _machine;
    Keypad_4x3* _keypad;
    BanknoteReader* _bankNoteReader;
    std::vector<Relay> _relays;

    int setupKeypad();
    int setupKBankNoteReader();
    int setupRelays();

    int setupModel();
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