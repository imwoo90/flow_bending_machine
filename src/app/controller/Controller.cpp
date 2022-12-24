#include <Controller.h>

#include <device/BanknoteReader/OBH_K03S.h>
#include <device/BanknoteReader/OBH_K03P.h>
#include <device/Relay/UPUS_SKB.h>
#include <device/Relay/R4D3B16.h>

struct MessageCallbackData{
    QueueHandle_t q;
     Message msg;
};

static void putMessageCallback( TimerHandle_t xTimer ) {
    MessageCallbackData* p = (MessageCallbackData*)pvTimerGetTimerID(xTimer);
    xQueueSend(p->q, &p->msg, 0);
    delete p;
    xTimerDelete(xTimer, 0);
}

void Controller::putMessage(MessageType type, int data, int delay_ms) {
    MessageCallbackData *p = new MessageCallbackData();
    p->q = _q; p->msg.type = type; p->msg.data = data;

    if (delay_ms <= 0) {
        if(_isISR)
            xQueueSendFromISR(p->q, &p->msg, NULL);
        else
            xQueueSend(p->q, &p->msg, 0);
    } else {
        TimerHandle_t xTimer = xTimerCreate
        ( /* Just a text name, not used by the RTOS
            kernel. */
            "putMessage",
            /* The timer period in ticks, must be
            greater than 0. */
            pdMS_TO_TICKS(delay_ms),
            /* The timers will auto-reload themselves
            when they expire. */
            pdFALSE,
            /* The ID is used to store a count of the
            number of times the timer has expired, which
            is initialised to 0. */
            p,
            /* Each timer calls the same callback when
            it expires. */
            putMessageCallback
        );

        if(_isISR)
            xTimerStartFromISR(xTimer, 0);
        else
            xTimerStart(xTimer, 0);
    }
}

static int channel_swMotorInterrupt;
static void motorRoundCallback(Controller* p) {
    p->_isISR = true;
    if (digitalRead(2) == LOW) {
        p->putMessage(MessageRelayClose, channel_swMotorInterrupt);
    }
    p->_isISR = false;
}

int Controller::setupMachine() {
    _machine = FlowVendingMachine::getInstance();
    pinMode(2, INPUT);
    attachInterrupt(2, motorRoundCallback, FALLING, this);

    auto onChangedCallback = [&](std::unordered_map<std::string, std::string> data) {
        static std::unordered_map<std::string, std::string> _prevData;
        bool isSame = true;
        for (auto &cur : data) {
            auto it = _prevData.find(cur.first);
            if (it == _prevData.end()) {
                isSame = false;
                break;
            }
            if (it->second != cur.second) {
                Serial.printf("%s\r\n", cur.second.c_str());
                isSame = false;
                break;
            }
        }
        if (isSame)
            return;

        Serial.printf("%s\r\n", data["state"].c_str());
        _prevData = data;


        _display->show(data);

        //Reader enable disable controll
        if (data["BanknoteReader"] == "enable") {
            putMessage(MessageBanknoteReaderEnable, 0);
        } else if (data["BanknoteReader"] == "disable") {
            putMessage(MessageBanknoteReaderDisable, 0);
        }

        if(data["LockerType"] == "1") { // relay (Lock)
            int channel = std::stoi(data["LockerChannel"]);
            putMessage(MessageRelayOpen, channel);
            putMessage(MessageRelayClose, channel, 500);
        } else if(data["LockerType"] == "2") { // relay (mortor)
            channel_swMotorInterrupt = std::stoi(data["LockerChannel"]);
            putMessage(MessageRelayOpen, channel_swMotorInterrupt);
            putMessage(MessageRelayClose, channel_swMotorInterrupt, 8000);
            // The motorRoundCallback must be called after above code is working
        }

        if(data["LEDState"] == "on") { // relay (LED)
            int channel = std::stoi(data["LEDChannel"]);
            putMessage(MessageRelayOpen, channel);
        } else if(data["LEDState"] == "off") {
            int channel = std::stoi(data["LEDChannel"]);
            putMessage(MessageRelayClose, channel);
        }

        if(data["sysMsg"] == "DelayCall") {
            int delay = std::stoi(data["sysMsgArg_0"]);
            putMessage(MessageSystemMessage, DelayCall, delay);
        }

        if(data["deinitRelays"] == "Running") {
            deinitRelays();
        }
    };
    auto onTimeoutCallback = [&](const int signal) {
        putMessage(MessageTimeout, signal);
    };

    _machine->initialize(onChangedCallback, onTimeoutCallback);
    putMessage(MessageInitial, 0);
    return 0;
}

static void _keypadOffLED( TimerHandle_t xTimer ) {
    Keypad_4x3* _keypad = (Keypad_4x3*)pvTimerGetTimerID(xTimer);
    _keypad->offLED();
}
int Controller::setupKeypad() {
    enum {KEYPAD_I2C_SCL = 5, KEYPAD_I2C_SDA = 4, KEYPAD_LED = 3};
    auto onKeypadCallback = [&](const KeyState state, const char key) {
        switch(state) {
        case PRESSED:
            putMessage(MessageKeypadPress, key);
            break;
        case RELEASED:
            putMessage(MessageKeypadRelease, key);
            break;
        }
    };
    Wire.setClock(400000);
    Wire.setSCL(KEYPAD_I2C_SCL);
    Wire.setSDA(KEYPAD_I2C_SDA);
    _keypad = Keypad_4x3::getInstance();
    _keypad->subscribe(onKeypadCallback);
    _keypad->offLED();

    //create keypad off led timer
    _keypadOffLedTimer = xTimerCreate
    ( /* Just a text name, not used by the RTOS
        kernel. */
        "keypadOffLed",
        /* The timer period in ticks, must be
        greater than 0. */
        pdMS_TO_TICKS(2*1000),
        /* The timers will auto-reload themselves
        when they expire. */
        pdFALSE,
        /* The ID is used to store a count of the
        number of times the timer has expired, which
        is initialised to 0. */
        _keypad,
        /* Each timer calls the same callback when
        it expires. */
        _keypadOffLED
    );
    _keypad->startPolling();
    return 0;
}

int Controller::setupBankNoteReader() {
    auto onRecognizedBankNote = [&](const int billData) {
        putMessage(MessageBanknoteRecognize, billData);
    };
    int readerMode = _machine->_database->getBanknoteReaderMode();

    if (readerMode == 1) {
        _bankNoteReader = OBH_K03P::getInstance()->setPins(12, 10, 11);
    } else if(readerMode == 2) {
        Serial2.setPinout(8, 9);
        Serial2.begin(9600);
        _bankNoteReader = OBH_K03S::getInstance(Serial2);
    } else {
        Serial.println("Reader Mode setting error");
        return -1;
    }

    // _bankNoteReader = OBH_K03P::getInstance()->setPins(12, 10, 11);

    _bankNoteReader->registerBillDataCallBack(onRecognizedBankNote);
    return _bankNoteReader->initialized();
}

int Controller::deinitRelays() {
    for(auto _r : _relays) {
        delete _r;
    }
    _relays.clear();
    int nRelays = _machine->_database->getNumberOfRelays();
    for (int i = 0; i < nRelays; i++) {
        int _type = _machine->_database->getRelayType(i);
        int numOfCh = _machine->_database->getNumberOfChannels(i);
        if (_type == 1) {
            _relays.push_back(new R4D3B16(i+1, numOfCh, Serial1)); //i + 1 is relay address
        }
    }
    return 0;
}

int Controller::setupRelays() {
    enum {MAX485_RO = 1, MAX485_DI = 0,};
    Serial1.setRX(MAX485_RO);
    Serial1.setTX(MAX485_DI);
    Serial1.begin(9600);

    deinitRelays();
    // for (int i = 0; i < _machine->_database->getNumberOfColumns(); i++) {
    //     putMessage(MessageRelayClose, i);
    // }
    return 0;
}

void Controller::setup() {
    _display = Display::getInstance();
    _display->begin();
    delay(1000);
    // Controller loop Queue Create
    _q = xQueueCreate(256, sizeof(Message));
    if (setupMachine() < 0) {
        return;
    }

    // Setup device
    delay(1000);
    if (setupBankNoteReader() < 0) {
        _isInitOk = false;
    }
    delay(1000);
    if (setupKeypad() < 0) {
        _isInitOk = false;
    }
    delay(1000);
    if (setupRelays() < 0) {
        _isInitOk = false;
    }
    delay(1000);

    pinMode(22, OUTPUT); //running ld pin number is 22
    //total setup time is 5sec for easy firmware update when running binary is dead right now after started
}

void Controller::loop() {
    Message msg;
    xQueueReceive(_q, &msg, portMAX_DELAY);
    _keypad->disablePolling();
    processModel(msg);
    operateDevice(msg);
    _keypad->enablePolling();
}

void Controller::processModel(Message &Message) {
    switch(Message.type) {
    case MessageInitial:
        _machine->begin(_isInitOk);
        putMessage(MessageRunning, 0);
        break;
    case MessageKeypadPress:
        _machine->pressKey(Message.data);
        _keypad->onLED();
        if ( xTimerIsTimerActive(_keypadOffLedTimer))
            xTimerStop(_keypadOffLedTimer, 0);
        break;
    case MessageKeypadRelease:
        _machine->releaseKey(Message.data);
        xTimerStart(_keypadOffLedTimer, 0);
        break;
    case MessageBanknoteRecognize:
        _machine->recognizeBanknote(Message.data);
        break;
    case MessageSystemMessage:
        _machine->systemMessage((SystemMessage)Message.data);
        break;
    case MessageTimeout:
        _machine->timeout(Message.data);
        break;
    }
}

void Controller::operateDevice(Message &Message) {
    static PinStatus running_led = HIGH;
    switch(Message.type) {
    case MessageBanknoteReaderEnable:
        _bankNoteReader->enable();
        break;
    case MessageBanknoteReaderDisable:
        _bankNoteReader->disable();
        break;
    case MessageRunning:
        digitalWrite(22, running_led);
        if (running_led == LOW) {
            running_led = HIGH;
        } else {
            running_led = LOW;
        }
        putMessage(MessageRunning, 0, 250);
        break;
    case MessageRelayOpen:{
        int channel = Message.data;
        auto r_ch = convertChannelToRelayFromModel(channel);
        _relays[r_ch.first]->open(r_ch.second);
        Serial.println("relay open");
        break;
    } case MessageRelayClose: {
        int channel = Message.data;
        auto r_ch = convertChannelToRelayFromModel(channel);
        _relays[r_ch.first]->close(r_ch.second);
        Serial.println("relay close");
        break;
    } default:
        break;
    }
}