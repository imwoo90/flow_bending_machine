#include <FlowBendingMachine.h>
#include "state/Selling.h"
#include "state/StopSelling.h"

void FlowBendingMachine::begin(bool isNormal, std::function<void(std::unordered_map<std::string, std::string>)> onChangedCallback, std::function<void(const int)> timeoutCallback) {
    MachineState::_onChangedCallback = onChangedCallback;
    MachineState::_timeoutCallback = timeoutCallback;
    if (isNormal) {
        _state = Selling::getInstance();
    } else {
        _state = StopSelling::getInstance();
    }

    MachineState::_onChangedCallback(_state->data);
}

void FlowBendingMachine::pressKey(const char key) {
    _state = _state->pressKey(key);
    MachineState::_onChangedCallback(_state->data);
}

void FlowBendingMachine::releaseKey(const char key) {
    _state = _state->releaseKey(key);
    MachineState::_onChangedCallback(_state->data);
}

void FlowBendingMachine::recognizeBanknote(const int bankNote) {
    _state = _state->recognizeBanknote(bankNote);
    MachineState::_onChangedCallback(_state->data);
}

void FlowBendingMachine::timeout(const int signal) {
    _state = _state->timeout(signal);
    MachineState::_onChangedCallback(_state->data);
}