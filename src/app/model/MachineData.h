#ifndef MACHINE_DATA_H
#define MACHINE_DATA_H

#include <Arduino.h>

class MachineData {
private:

    MachineData() {}
public:
    static MachineData* getInstance() {
        static MachineData singleton_instance;
        return &singleton_instance;
    }

};

#endif