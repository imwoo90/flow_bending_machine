#include "InputMoney.h"

#include "Selling.h"


static void inputMoneyTimeoutCallBack( TimerHandle_t xTimer ) {
    MachineState::_timeoutCallback(TimeoutInputMoney);
}

InputMoney::InputMoney() {
    _timer = xTimerCreate(
         /* Just a text name, not used by the RTOS
        kernel. */
        "InputMoney",
        /* The timer period in ticks, must be
        greater than 0. */
        pdMS_TO_TICKS(30*1000),
        /* The timers will auto-reload themselves
        when they expire. */
        pdFALSE,
        /* The ID is used to store a count of the
        number of times the timer has expired, which
        is initialised to 0. */
        ( void * ) 0,
        /* Each timer calls the same callback when
        it expires. */
        inputMoneyTimeoutCallBack
    );
}

void InputMoney::initialize() {
    char buf[32];

    // init data
    _data.clear();
    _data["state"] = "InputMoney_0";
    _data["param_0"] = itoa(_database->getPrice(_column), buf, 10);
    _data["BanknoteReader"] = "enable";
    xTimerChangePeriod(_timer, 30*1000, 0);
    //key timout start
    xTimerStart(_timer, 0);
}

InputMoney* InputMoney::getInstance(int column) {
    static InputMoney singleton_instance;
    singleton_instance._column = column;
    singleton_instance._inputMoney = 0;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* InputMoney::recognizeBanknote(const int banknote) {
    char buf[32];
    MachineState* next = this;
    if ( xTimerIsTimerActive(_timer) == pdFALSE )
        return this;
    xTimerStop(_timer, 0);

    int price = _database->getPrice(_column);
   _inputMoney += banknote;

    _data["state"] = "InputMoney_1";
    _data["param_0"] = itoa(_inputMoney, buf, 10);
    _data["param_1"] = itoa(price, buf, 10);
    if (price <= _inputMoney) {
        int quantity = _database->getQuantity(_column) - 1;
        _database->setQuantity(_column, quantity);
        _database->setNumberOfTotalSales(1 + _database->getNumberOfTotalSales());
        _database->setMoneyOfTotalSales(price + _database->getMoneyOfTotalSales());
        _data["BanknoteReader"] = "disable";

        _data["LockerType"] = itoa(_database->getMotorType(_column), buf, 10);
        _data["LockerChannel"] = itoa(_database->getChannel(_column), buf, 10);
        // To do this
        // _data["LEDState"] = quantity ? "on" : "off";
        // _data["LEDChannel"] = "";
        if (_data["LockerType"] == "1") {
            xTimerChangePeriod(_timer, 2*1000, 0);
        } else if (_data["LockerType"] == "2") {
            xTimerChangePeriod(_timer, 8*1000, 0);
        }
    }

    xTimerStart(_timer, 0);
    return next;
}

MachineState* InputMoney::pressKey(const char key) {
    MachineState* next = this;
    _data["LockerType"] = "";
    _data["LockerChannel"] = "";
    // To do this
    // _data["LEDState"] = quantity ? "on" : "off";
    // _data["LEDChannel"] = "";
    switch ( key ) {
    case '*':
        int price = _database->getPrice(_column);
        if (price <= _inputMoney) {
            break;
        }

        if ( xTimerIsTimerActive(_timer) == pdFALSE )
            return this;
        xTimerStop(_timer, 0);
        next = Selling::getInstance();
        break;
    }
    return next;
}

MachineState* InputMoney::timeout(const int signal) {
    int price = _database->getPrice(_column);
    return Selling::getInstance();
}
