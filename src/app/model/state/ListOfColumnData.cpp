#include "ListOfColumnData.h"

#include "VendingMachineModeSetting.h"

#define NUMBER_PER_PAGE 8

void ListOfColumnData::initialize() {
    // init data
    _data.clear();
    _data["state"] = "ListOfColumnData";
    _page = 0;
    loadListOfColumnData();
}

ListOfColumnData* ListOfColumnData::getInstance() {
    static ListOfColumnData singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* ListOfColumnData::pressKey(const char key) {
    MachineState* next = this;
    switch ( key ) {
    case '1':
        if (_page == 0)
            break;

        _page -= 1;
        loadListOfColumnData();
        break;
    case '2':
        _page += 1;
        if (NUMBER_PER_PAGE*_page >= _database->getNumberOfColumns()) {
            _page -= 1;
            break;
        }
        loadListOfColumnData();
        break;
    case '*':
        next = VendingMachineModeSetting::getInstance();
        break;
    }
    return next;
}

void ListOfColumnData::loadListOfColumnData() {
    char buf[32];
    int param_index = 0;
    std::string param;
    std::string prefix = "param_";
    for (int i = 0; i < NUMBER_PER_PAGE; i++) {
        int column = _page*NUMBER_PER_PAGE + i;
        int channel = _database->getChannel(column);
        int motorType = _database->getMotorType(column);

        column += 1; channel += 1; // match user's recognize column and channel
        param = prefix + itoa(param_index++, buf, 10);
        _data[param] = itoa(channel, buf, 10);
        param = prefix + itoa(param_index++, buf, 10);
        _data[param] = itoa(column, buf, 10);
        param = prefix + itoa(param_index++, buf, 10);
        _data[param] = itoa(motorType, buf, 10);
    }
    param = prefix + itoa(param_index++, buf, 10);
    _data[param] = std::string("p")+itoa(_page+1, buf, 10);
}