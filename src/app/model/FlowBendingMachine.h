#ifndef FLOW_BENDING_MACHINE_H
#define FLOW_BENDING_MACHINE_H

#include <MachineState.h>
#include <MachineData.h>

#include <functional>

class FlowBendingMachine {
private:
    MachineState* _state;

    FlowBendingMachine() {}
public:
    void begin(bool isNormal, std::function<void(std::unordered_map<std::string, std::string>)> onChangedCallback, std::function<void(const int)> timeoutCallback);
    static FlowBendingMachine* getInstance() {
        static FlowBendingMachine singleton_instance;
        return &singleton_instance;
    }

    void pressKey(const char key);
    void releaseKey(const char key);
    void recognizeBanknote(const int bankNote);
    void timeout(const int signal);
};

#endif

