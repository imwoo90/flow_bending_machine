#ifndef MACHINE_STATE_H
#define MACHINE_STATE_H

#include <Arduino.h>
#include <functional>

#include <unordered_map>
#include <string>
#include <vector>

class MachineState {
private:
    virtual void initialize() = 0;
public:
    static std::function<void(const int)> _timeoutCallback;
    static std::function<void(std::unordered_map<std::string, std::string>)> _onChangedCallback;

    std::unordered_map<std::string, std::string> data;

    virtual MachineState* pressKey(const char key) { return this;}
    virtual MachineState* releaseKey(const char key) {return this;}
    virtual MachineState* recognizeBanknote(const int Banknote) {return this;}
    virtual MachineState* timeout(const int signal) {return this;}
};

#endif