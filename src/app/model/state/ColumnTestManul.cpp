#include "ColumnTestManul.h"

#include "WorkingTest.h"

void ColumnTestManul::initialize() {
    // init data
    _data.clear();
    _data["state"] = "ColumnTestManul";
    _data["param_0"] = "000";
}

ColumnTestManul* ColumnTestManul::getInstance() {
    static ColumnTestManul singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* ColumnTestManul::pressKey(const char key) {
    MachineState* next = this;
    _data["LockerType"] = "";
    _data["LockerChannel"] = "";
    switch ( key ) {
    case '#':{
        char buf[32];
        int column = std::stoi(_data["param_0"]) - 1;
        if (0 <= column && column < _database->getNumberOfColumns()) {
            _data["LockerType"] = itoa(_database->getMotorType(column), buf, 10);
            _data["LockerChannel"] = itoa(_database->getChannel(column), buf, 10);
        }
        break;}
    case '*':
        next = WorkingTest::getInstance();
        break;
    default: {//1~9
        std::string &param_1 = _data["param_0"];
        rotate(param_1.begin(), param_1.begin()+1, param_1.end());
        param_1[param_1.length()-1] = key;
        break;}
    }
    return next;
}
