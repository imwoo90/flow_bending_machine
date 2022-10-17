#include <Controller.h>
#include <SoftwareSerial.h>

#include <device/BanknoteReader/OBH_K03S.h>
#include <device/BanknoteReader/OBH_K03P.h>

#include <device/Relay/UPUS_SKB.h>
#include <device/Relay/R4D3B16.h>

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

        if (data["state"] == "Selling") {
            if (_isInitOk) {
                _bankNoteReader->disable();
            }
        } else if (data["state"] == "InputMoney") {
            int column = std::stoi(data["param_2"]) - 1;
            int price = _machine->_database->getPrice(column);
            int inputMoney = std::stoi(data["param_0"]);
            if (price <= inputMoney) {
                int channel = _machine->_database->getChannel(column);
                int numOfRelay = _machine->_database->getNumberOfRelays();
                for (int i = 0; i < numOfRelay; i++) {
                    int nc = _machine->_database->getNumberOfChannels(i);
                    if (channel < nc) {
                        _relays[i]->open(channel);
                        delay(1000);
                        _relays[i]->close(channel);
                        break;
                    }
                    channel -= nc;
                }
            } else {
                _bankNoteReader->enable();
            }
        } else if (data["state"] == "ColumnTestManul") {
            if (data["param_0"] == "start") {
                int column = std::stoi(data["param_1"]) - 1;
                int channel = _machine->_database->getChannel(column);
                int numOfRelay = _machine->_database->getNumberOfRelays();
                for (int i = 0; i < numOfRelay; i++) {
                    int nc = _machine->_database->getNumberOfChannels(i);
                    if (channel < nc) {
                        _relays[i]->open(channel);
                        delay(1000);
                        _relays[i]->close(channel);
                        break;
                    }
                    channel -= nc;
                }
            }
        } else if (data["state"] == "ColumnTestSection") {
            if (data["param_0"] == "start") {
                int column = std::stoi(data["param_1"]) - 1;
                int nColumns = std::stoi(data["param_2"]) - 1;
                int numOfRelay = _machine->_database->getNumberOfRelays();
                std::vector<int> nChannels;
                for(int i = 0; i < numOfRelay; i++) {
                    int nc = _machine->_database->getNumberOfChannels(i);
                    nChannels.push_back(nc);
                }

                for (int i = column; i <= nColumns; i++) {
                    int channel = _machine->_database->getChannel(i);
                    for(int j = 0; j < nChannels.size(); j++) {
                        int nc = nChannels[j];
                        if( channel < nc) {
                            _relays[j]->open(channel);
                            delay(1000);
                            _relays[j]->close(channel);
                            break;
                        }
                        channel -= nc;
                    }
                }
            }
        } else if (data["state"] == "ColumnTestAll") {
            if (data["param_0"] == "start") {
                int column = std::stoi(data["param_1"]) - 1;
                int nColumns =  _machine->_database->getNumberOfColumns();
                int numOfRelay = _machine->_database->getNumberOfRelays();
                std::vector<int> nChannels;
                for(int i = 0; i < numOfRelay; i++) {
                    int nc = _machine->_database->getNumberOfChannels(i);
                    nChannels.push_back(nc);
                }

                for (int i = column; i < nColumns; i++) {
                    int channel = _machine->_database->getChannel(i);
                    for(int j = 0; j < nChannels.size(); j++) {
                        int nc = nChannels[j];
                        if( channel < nc) {
                            _relays[j]->open(channel);
                            delay(1000);
                            _relays[j]->close(channel);
                            break;
                        }
                        channel -= nc;
                    }
                }
            }
        }

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


int Controller::setupKBankNoteReader() {
    int readerMode = _machine->_database->getBanknoteReaderMode();
    if (readerMode == 1) {
        enum { BANK_NOTE_RX = 2, BANK_NOTE_TX = 3,};
        static SoftwareSerial ssBankNote(BANK_NOTE_RX, BANK_NOTE_TX);
        ssBankNote.begin(9600);
        _bankNoteReader = OBH_K03S::getInstance(ssBankNote);
    } else if(readerMode == 2) {
        _bankNoteReader = OBH_K03P::getInstance(13, 11, 12);
    } else {
        Serial.println("Reader Mode setting error");
        return -1;
    }

    // _bankNoteReader = OBH_K03P::getInstance(13, 11, 12);

    auto onRecognizedBankNote = [&](const int billData) {
        Message msg;
        msg.type = MessageBanknoteRecognize;
        msg.data = billData;
        xQueueSend(_q, &msg, 10);
    };

    _bankNoteReader->registerBillDataCallBack(onRecognizedBankNote);
    return _bankNoteReader->initialized();
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
    for (int i = 0; i < nRelays; i++) { //i + 1 is relay address
        _relays.push_back(new R4D3B16(i+1, Serial2, preTx, postTx));
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
    _isInitOk = true;
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
