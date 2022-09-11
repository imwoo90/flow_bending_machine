#ifndef STOP_SELLING_H
#define STOP_SELLING_H

#include <MachineState.h>
#include <MachineData.h>

class StopSelling : public MachineState {
private:
    StopSelling() {}
    virtual void initialize();
public:
    static StopSelling* getInstance();

    virtual MachineState* pressKey(const char key);
    virtual MachineState* releaseKey(const char key);
};

#endif