#ifndef OBH_K03S_H
#define OBH_K03S_H

#include <Arduino.h>
#include "BanknoteReader.h"

typedef enum {
    RESET_WAIT,
    WAIT,
    STARTWAIT,
    RECOGNITION_RETURN,
    RECOGNITION_WAIT,
    RECOGNITION_END,
    RETURN_START,
    RETURN_WAIT,
    RETURN_END,
    STACK_START,
    STACK_WAIT,
    STACK_END,
    ERROR_WAIT,
    INSERT_ENABLE,
    INSERT_DISABLE,
    STACK_OPENED = 0X10,
    FORCESTACK_WAIT,
    FORCESTACK_END,
} ActiveStatus;

class OBH_K03S : public BanknoteReader {
private:
    Stream* _serial = NULL;

    int process(char _buf[5]);
    int activeStatusProcess(ActiveStatus reg);
    bool receiveCommnad(const char* _buf, const char* cmd);
    void sendCommand(const char*);
protected:
    virtual int getBilldata();
public:
    OBH_K03S(Stream &serial);
    virtual int initialized(const char* taskName);
    virtual void enable();
    virtual void disable();
};

#endif