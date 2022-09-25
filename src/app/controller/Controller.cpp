#include <Controller.h>
#include <SoftwareSerial.h>

typedef enum {
    MessageKeypadPress,
    MessageKeypadRelease,
    MessageBanknoteRecognize,
    MessageTimeout,
} MessageType;

struct Message {
    MessageType type;
    int data;
};

int Controller::setupModel() {
    _machine = FlowVendingMachine::getInstance();
    auto onChangedCallback = [&](std::unordered_map<std::string, std::string> data) {
        Serial.printf("state: %s\n\r", data["state"].c_str());
        std::string param = "param_0";
        for(int i = 0; i < 9; i++) {
            param[6] = '0' + i;
            if (data.find(param) == data.end())
                break;
            Serial.printf("param_%d=%s, ", i, data[param].c_str());
        }
        Serial.printf("\n\r");
        Serial.printf("\n\r");
    };
    auto timeoutCallback = [&](const int signal) {
        Message msg;
        msg.type = MessageTimeout;
        msg.data = signal;
        xQueueSend(_q, &msg, 10);
    };
    _machine->begin(1, onChangedCallback, timeoutCallback);

    return 0;
}


enum {
    KEYPAD_I2C_SCL = 17,
    KEYPAD_I2C_SDA = 16,
};
int Controller::setupKeypad() {
    auto keypadCallback = [&](const char key) {
        Message msg;
        msg.type = MessageKeypadPress;
        msg.data = key;
        xQueueSend(_q, &msg, 10);
    };
    Wire.setClock(400000);
    Wire.setSCL(KEYPAD_I2C_SCL);
    Wire.setSDA(KEYPAD_I2C_SDA);
    _keypad = Keypad_4x3::getInstance();
    _keypad->subscribe(keypadCallback);
    return 0;
}


enum {
    BANK_NOTE_RX = 2,
    BANK_NOTE_TX = 3,
};
int Controller::setupKBankNoteReader() {
    static SoftwareSerial ssBankNote(BANK_NOTE_RX, BANK_NOTE_TX);
    ssBankNote.setTimeout(ULONG_MAX);
    ssBankNote.begin(9600);

    int readerMode = _machine->_database->getBanknoteReaderMode();
    if (readerMode == 1) {
        _bankNoteReader = OBH_K03S::getInstance(ssBankNote);
    } else {
        Serial.println("Reader Mode setting error");
        return -1;
    }

    auto onRecognizedBankNote = [&](const int billData) {
        Message msg;
        msg.type = MessageBanknoteRecognize;
        msg.data = billData;
        xQueueSend(_q, &msg, 10);
    };

    _bankNoteReader->registerBillDataCallBack(onRecognizedBankNote);
    _bankNoteReader->initialized("OBH_K03S");
    return 0;
}


enum {
    MAX485_DE_RE = 6,
    MAX485_RO = 5,
    MAX485_DI = 4,
};
static void preTx() {
    digitalWrite(MAX485_DE_RE, 1);
}
static void postTx() {
    digitalWrite(MAX485_DE_RE, 0);
}
int Controller::setupRelays() {
    int nRelays = _machine->_database->getNumberOfRelays();
    _relays.clear();

    pinMode(MAX485_DE_RE, OUTPUT);
    digitalWrite(MAX485_DE_RE, 0);
    Serial2.setRX(MAX485_RO);
    Serial2.setTX(MAX485_DI);
    Serial2.begin(9600);
    for (int i = 0; i <= nRelays; i++) { // Unuser index 0
        _relays.push_back(Relay(i, Serial2, preTx, postTx));
    }

    return 0;
}

void Controller::setup() {
    delay(5000);
    // Controller loop Queue Create
    _q = xQueueCreate(16, sizeof(Message));
    setupModel();

    // Setup device
    setupKBankNoteReader();
    setupKeypad();
    setupRelays();
}

void Controller::loop() {
    Message msg;
    xQueueReceive(_q, &msg, portMAX_DELAY);
    switch(msg.type) {
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
