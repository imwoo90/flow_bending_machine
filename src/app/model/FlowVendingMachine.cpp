#include <FlowVendingMachine.h>
#include "state/Selling.h"
#include "state/StopSelling.h"

void FlowVendingMachine::begin(bool isNormal, std::function<void(std::unordered_map<std::string, std::string>)> onChangedCallback, std::function<void(const int)> timeoutCallback) {
    MachineState::_onChangedCallback = onChangedCallback;
    MachineState::_timeoutCallback = timeoutCallback;
    if (isNormal) {
        _state = Selling::getInstance();
    } else {
        _state = StopSelling::getInstance();
    }
    _database = MachineData::getInstance();
    _database->initialize();
    _state->_database = _database;
    MachineState::_onChangedCallback(_state->_data);
}

void FlowVendingMachine::pressKey(const char key) {
    _state = _state->pressKey(key);
    MachineState::_onChangedCallback(_state->_data);
}

void FlowVendingMachine::releaseKey(const char key) {
    _state = _state->releaseKey(key);
    MachineState::_onChangedCallback(_state->_data);
}

void FlowVendingMachine::recognizeBanknote(const int bankNote) {
    _state = _state->recognizeBanknote(bankNote);
    MachineState::_onChangedCallback(_state->_data);
}

void FlowVendingMachine::timeout(const int signal) {
    _state = _state->timeout(signal);
    MachineState::_onChangedCallback(_state->_data);
}