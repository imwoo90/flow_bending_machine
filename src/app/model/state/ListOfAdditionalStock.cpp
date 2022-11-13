#include "ListOfAdditionalStock.h"

#include "MainManagement.h"

#define NUMBER_PER_PAGE 8

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
        if (NUMBER_PER_PAGE*_page >= _database->getNumberOfColumns()) {
            _page -= 1;
            break;
        }
        loadListOfAdditionalStock();
        break;
    case '#':
        //5초간 눌렀을때 초기화 구현 해야함
        for (int column = 0; column < _database->getNumberOfColumns(); column++)
            _database->setAdditional(column, 0);

        loadListOfAdditionalStock();
        break;
    case '*':
        next = MainManagement::getInstance();
        break;
    }
    return next;
}

void ListOfAdditionalStock::loadListOfAdditionalStock() {
    char buf[32];
    int param_index = 0;
    std::string param;
    std::string prefix = "param_";
    for (int i = 0; i < NUMBER_PER_PAGE; i++) {
        int column = _page*NUMBER_PER_PAGE + i;
        int price = _database->getPrice(column);
        int additional = _database->getAdditional(column);

        column += 1;// match user's recognize column
        param = prefix + itoa(param_index++, buf, 10);
        _data[param] = itoa(column, buf, 10);
        param = prefix + itoa(param_index++, buf, 10);
        _data[param] = itoa(price/1000, buf, 10);
        param = prefix + itoa(param_index++, buf, 10);
        _data[param] = itoa(additional, buf, 10);
    }
    param = prefix + itoa(param_index++, buf, 10);
    _data[param] = std::string("p")+itoa(_page+1, buf, 10);
}