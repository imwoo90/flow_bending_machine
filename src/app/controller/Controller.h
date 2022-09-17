#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <FreeRTOS.h>
#include <queue.h>

#include <Arduino.h>
#include <FlowBendingMachine.h>
#include <device/Keypad_4x3/Keypad_4x3.h>
#include <device/BanknoteReader/OBH_K03S.h>

class Controller {
private:
    QueueHandle_t _q;
    FlowBendingMachine* _machine;
    Keypad_4x3* _keypad;
    BanknoteReader* _bankNoteReader;

    int setupKeypad();
    int setupKBankNoteReader();
    int setupMotor();

    int setupModel();
public:
    void setup();
    void loop();
};


extern Controller flowBendingMachineController;

#endif