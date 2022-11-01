#include "ListOfAdditionalStock.h"

#include "VendingMachineModeSetting.h"

void ListOfAdditionalStock::initialize() {
    // init data
    _data.clear();
    _data["state"] = "ListOfAdditionalStock";
    _page = 0;
    loadListOfAdditionalStock();
}

ListOfAdditionalStock* ListOfAdditionalStock::getInstance() {
    static ListOfAdditionalStock singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* ListOfAdditionalStock::pressKey(const char key) {
    MachineState* next = this;
    switch ( key ) {
    case '1':
        if (_page == 0)
            break;

        _page -= 1;
        loadListOfAdditionalStock();
        break;
    case '2':
        _page += 1;
        if (16*_page > _database->getNumberOfColumns()) {
            _page -= 1;
            break;
        }
        loadListOfAdditionalStock();
        break;
    case '*':
        next = VendingMachineModeSetting::getInstance();
        break;
    }
    return next;
}

void ListOfAdditionalStock::loadListOfAdditionalStock() {
    // int column = 0;
    // int numOfRelay = _database->getNumberOfRelays();
    // for (int i = 0; i < numOfRelay; i++) {
    //     int numOfChannels = _database->getNumberOfChannels(i);
    //     int motorType = _database->getMotorType(i);
    //     for (int j = 0; j < numOfChannels; j++) {
    //         int channel = _database->getChannel(column);

    //         column += 1;
    //     }
    // }

    // Not implemented yet

}