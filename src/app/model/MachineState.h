#pragma once

#include <Arduino.h>
#include <MachineData.h>

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

    static std::unordered_map<std::string, std::string> _data;
    static MachineData* _database;

    virtual MachineState* pressKey(const char key) { return this;}
    virtual MachineState* releaseKey(const char key) {return this;}
    virtual MachineState* recognizeBanknote(const int banknote) {return this;}
    virtual MachineState* timeout(const int signal) {return this;}
};
