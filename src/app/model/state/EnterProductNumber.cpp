#include "EnterProductNumber.h"
#include "Selling.h"

static void keyTimeoutCallBack( TimerHandle_t xTimer ) {
    MachineState::_timeoutCallback(0);
}

EnterProductNumber::EnterProductNumber() {
    _timer = xTimerCreate(
         /* Just a text name, not used by the RTOS
        kernel. */
        "Timer",
        /* The timer period in ticks, must be
        greater than 0. */
        pdMS_TO_TICKS(10*1000),
        /* The timers will auto-reload themselves
        when they expire. */
        pdFALSE,
        /* The ID is used to store a count of the
        number of times the timer has expired, which
        is initialised to 0. */
        ( void * ) 0,
        /* Each timer calls the same callback when
        it expires. */
        keyTimeoutCallBack
    );
}

void EnterProductNumber::initialize() {
    // init data
    data.clear();
    data["state"] = "EnterProductNumber";
    data["param_0"] = "000";

    //key timout start
    xTimerStart(_timer, 0);
}

EnterProductNumber* EnterProductNumber::getInstance() {
    static EnterProductNumber singleton_instance;
    singleton_instance.initialize();
    return &singleton_instance;
}

MachineState* EnterProductNumber::pressKey(const char key) {
    if ( xTimerIsTimerActive(_timer) == pdFALSE )
        return this;
    xTimerStop(_timer, 0);

    MachineState* next = this;
    switch ( key ) {
    case '#':
        xTimerStart(_timer, 0);
        break;
    case '*':
        next = Selling::getInstance();
        break;
    default: //1~9
        std::string &param_0 = data["param_0"];
        if(param_0.length() == 3) {
            param_0[0] = param_0[1];
            param_0[1] = param_0[2];
            param_0[2] = key;
        } else {
            param_0.push_back(key);
        }
        xTimerStart(_timer, 0);
        break;
    }
    return next;
}

MachineState* EnterProductNumber::timeout(const int signal) {
    return Selling::getInstance();
}