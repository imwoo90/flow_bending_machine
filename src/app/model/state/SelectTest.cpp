#include "SelectTest.h"

#include "MainManagement.h"
#include "ColumnTestManul.h"
#include "ColumnTestSection.h"
#include "ColumnTestAll.h"

void SelectTest::initialize() {
    _data.clear();
    _data["state"] = "SelectTest";
}

SelectTest* SelectTest::getInstance() {
    static SelectTest singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* SelectTest::pressKey(const char key) {
    const bool changePasword = true;
    MachineState* next = this;
    switch ( key ) {
    case '*':
        next = MainManagement::getInstance();
        break;
    case '1':
        next = ColumnTestManul::getInstance();
        break;
    case '2':
        next = ColumnTestSection::getInstance();
        break;
    case '3':
        next = ColumnTestAll::getInstance();
        break;
    }
    return next;
}
