#pragma once

#include <MachineState.h>
#include <MachineData.h>

#include <functional>

class FlowVendingMachine {
private:
    MachineState* _state;
    MachineData* _database;
    FlowVendingMachine() {}
public:
    void begin(bool isNormal, std::function<void(std::unordered_map<std::string, std::string>)> onChangedCallback, std::function<void(const int)> timeoutCallback);
    static FlowVendingMachine* getInstance() {
        static FlowVendingMachine singleton_instance;
        return &singleton_instance;
    }

    void pressKey(const char key);
    void releaseKey(const char key);
    void recognizeBanknote(const int bankNote);
    void timeout(const int signal);
};
