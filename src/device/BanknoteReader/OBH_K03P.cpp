#include "OBH_K03P.h"

static void cntPulse(OBH_K03P* p) {
    if ( xTimerIsTimerActive(p->_timer) == pdTRUE )
        xTimerStopFromISR(p->_timer, 0);

    p->_cntPulse += 1;
    //key timout start
    xTimerStartFromISR(p->_timer, 0);
}

static void billDataEndCallback( TimerHandle_t xTimer ) {
    OBH_K03P* p = (OBH_K03P*)pvTimerGetTimerID(xTimer);
    xQueueSend(p->_q, NULL, 0);
}

static void errorOccur(OBH_K03P* p) {

}

void OBH_K03P::enable() {
    digitalWrite(_inhibitPin, HIGH);
}

void OBH_K03P::disable() {
    digitalWrite(_inhibitPin, LOW);
}

int OBH_K03P::getBillData() {
    int billData;
    xQueueReceive(_q, NULL, portMAX_DELAY);
    billData = _cntPulse*1000;
    _cntPulse = 0;
    return billData;
}

int OBH_K03P::initialized(const char* taskName) {
    pinMode(_inhibitPin, OUTPUT);
    pinMode(_vendPin, INPUT_PULLUP);
    pinMode(_errorPin, INPUT_PULLUP);
    disable();

    _q = xQueueCreate(16, 0);
    _timer = xTimerCreate(
         /* Just a text name, not used by the RTOS
        kernel. */
        "EnterProductNumber",
        /* The timer period in ticks, must be
        greater than 0. */
        pdMS_TO_TICKS(300),
        /* The timers will auto-reload themselves
        when they expire. */
        pdFALSE,
        /* The ID is used to store a count of the
        number of times the timer has expired, which
        is initialised to 0. */
        ( void * ) this,
        /* Each timer calls the same callback when
        it expires. */
        billDataEndCallback
    );
    attachInterrupt(_vendPin, cntPulse, FALLING, this);
    attachInterrupt(_errorPin, errorOccur, FALLING, this);
    BanknoteReader::initialized();
    return 0;
}

OBH_K03P* OBH_K03P::getInstance(int inhibitPin, int vendPin, int errorPin) {
    static OBH_K03P singleton_instance;
    singleton_instance._inhibitPin = inhibitPin;
    singleton_instance._vendPin = vendPin;
    singleton_instance._errorPin = errorPin;
    singleton_instance._name = "OBH_K03P";
    return &singleton_instance;
}