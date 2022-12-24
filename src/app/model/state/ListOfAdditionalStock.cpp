#include "ListOfAdditionalStock.h"

#include "MainManagement.h"

#define NUMBER_PER_PAGE 8

static void deleteListTimeoutCallBack( TimerHandle_t xTimer ) {
    MachineState::_timeoutCallback(TimeoutDeleteList);
}

ListOfAdditionalStock::ListOfAdditionalStock() {
    _timer = xTimerCreate(
         /* Just a text name, not used by the RTOS
        kernel. */
        "ListOfAdditionalStock",
        /* The timer period in ticks, must be
        greater than 0. */
        pdMS_TO_TICKS(5*1000),
        /* The timers will auto-reload themselves
        when they expire. */
        pdFALSE,
        /* The ID is used to store a count of the
        number of times the timer has expired, which
        is initialised to 0. */
        ( void * ) 0,
        /* Each timer calls the same callback when
        it expires. */
        deleteListTimeoutCallBack
    );
}

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

MachineState* ListOfAdditionalStock::timeout(const int signal) {
    //Additional of all columns are setted to 0
    for (int column = 0; column < _database->getNumberOfColumns(); column++)
        _database->setAdditional(column, 0);
    _database->flush(TypeColumnData);
    loadListOfAdditionalStock();
    return this;
}

MachineState* ListOfAdditionalStock::releaseKey(const char key) {
    MachineState* next = this;

    switch ( key ) {
    case '#':
        _isDisablePresskey = false;
        if ( xTimerIsTimerActive(_timer) == pdFALSE )
            break;
        xTimerStop(_timer, 0);
        break;
    }
    return next;
}

MachineState* ListOfAdditionalStock::pressKey(const char key) {
    MachineState* next = this;
    if (_isDisablePresskey)
        return next;

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
        _isDisablePresskey = true;
        xTimerStart(_timer, 0);
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
        bool isOverRange = column >= _database->getNumberOfColumns();

        column += 1;// match user's recognize column
        param = prefix + itoa(param_index++, buf, 10);
        _data[param] = isOverRange ? "" : itoa(column, buf, 10);
        param = prefix + itoa(param_index++, buf, 10);
        _data[param] = isOverRange ? "" : itoa(price/1000, buf, 10);
        param = prefix + itoa(param_index++, buf, 10);
        _data[param] = isOverRange ? "" : itoa(additional, buf, 10);
    }
    param = prefix + itoa(param_index++, buf, 10);
    _data[param] = std::string("p")+itoa(_page+1, buf, 10);
}