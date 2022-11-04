#include <Controller.h>

#include <device/BanknoteReader/OBH_K03S.h>
#include <device/BanknoteReader/OBH_K03P.h>
#include <device/Relay/UPUS_SKB.h>
#include <device/Relay/R4D3B16.h>

typedef enum {
    MessageInitial,
    MessageKeypadPress,
    MessageKeypadRelease,
    MessageBanknoteRecognize,
    MessageTimeout,
} MessageType;

struct Message {
    MessageType type;
    int data;
};

int Controller::setupMachine() {
    _machine = FlowVendingMachine::getInstance();
    auto onChangedCallback = [&](std::unordered_map<std::string, std::string> data) {
        Serial.printf("%s\r\n", data["state"].c_str());

        _display->show(data);

        //Reader enable disable controll
        if (data["BanknoteReader"] == "enable") {
            _bankNoteReader->enable();
        } else if (data["BanknoteReader"] == "disable") {
            _bankNoteReader->disable();
        }

        //relay controll
    };
    auto onTimeoutCallback = [&](const int signal) {
        Message msg = {.type = MessageTimeout, .data = signal};
        xQueueSend(_q, &msg, 10);
    };

    _machine->initialize(onChangedCallback, onTimeoutCallback);
    Message msg = {.type = MessageInitial, .data = 0};
    xQueueSend(_q, &msg, 0);
    return 0;
}

int Controller::setupKeypad() {
    enum {KEYPAD_I2C_SCL = 5, KEYPAD_I2C_SDA = 4,};
    auto onKeypadCallback = [&](const char key) {
        Message msg = {.type = MessageKeypadPress, .data = key};
        xQueueSend(_q, &msg, 10);
    };
    Wire.setClock(400000);
    Wire.setSCL(KEYPAD_I2C_SCL);
    Wire.setSDA(KEYPAD_I2C_SDA);
    _keypad = Keypad_4x3::getInstance();
    _keypad->subscribe(onKeypadCallback);
    return 0;
}

int Controller::setupBankNoteReader() {
    int readerMode = _machine->_database->getBanknoteReaderMode();
    if (readerMode == 1) {
        Serial2.setPinout(8, 9);
        Serial2.begin(9600);
        _bankNoteReader = OBH_K03S::getInstance(Serial2);
    } else if(readerMode == 2) {
        _bankNoteReader = OBH_K03P::getInstance()->setPins(12, 10, 11);
    } else {
        Serial.println("Reader Mode setting error");
        return -1;
    }

    auto onRecognizedBankNote = [&](const int billData) {
        Message msg = {.type = MessageBanknoteRecognize, .data = billData};
        xQueueSend(_q, &msg, 10);
    };

    _bankNoteReader->registerBillDataCallBack(onRecognizedBankNote);
    return _bankNoteReader->initialized();
}

int Controller::setupRelays() {
    enum {MAX485_RO = 1, MAX485_DI = 0,};
    int nRelays = _machine->_database->getNumberOfRelays();
    _relays.clear();

    Serial1.setRX(MAX485_RO);
    Serial1.setTX(MAX485_DI);
    Serial1.begin(9600);

    // To do relay initialize

    // for (int i = 0; i < nRelays; i++) { //i + 1 is relay address
    //     _relays.push_back(new R4D3B16(i+1, Serial1, preTx, postTx));
    // }

    // R4D3B16 test(1, Serial1);
    // for(int i = 0; i < 10; i++) {
    //     test.open(0x05);
    //     delay(1000);
    //     test.close(0x05);
    //     delay(1000);
    //     Serial.println("relay test");
    // }
    return 0;
}

void Controller::setup() {
    _display = Display::getInstance();
    _display->begin();
    delay(1000);
    // Controller loop Queue Create
    _q = xQueueCreate(16, sizeof(Message));
    if (setupMachine() < 0) {
        return;
    }
    delay(1000);
    // Setup device
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

    //total setup time is 5sec for easy firmware update when running binary is dead right now after started
}

void Controller::loop() {
    Message msg;
    xQueueReceive(_q, &msg, portMAX_DELAY);
    switch(msg.type) {
    case MessageInitial:
        _machine->begin(_isInitOk);
        break;
    case MessageKeypadPress:
        _machine->pressKey(msg.data);
        break;
    case MessageKeypadRelease:
        _machine->releaseKey(msg.data);
        break;
    case MessageBanknoteRecognize:
        _machine->recognizeBanknote(msg.data);
        break;
    case MessageTimeout:
        _machine->timeout(msg.data);
        break;
    }
}
