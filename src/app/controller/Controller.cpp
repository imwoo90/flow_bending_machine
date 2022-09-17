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

int Controller::setupKeypad() {
    auto keypadCallback = [&](const char key) {
        Message msg;
        msg.type = MessageKeypadPress;
        msg.data = key;
        xQueueSend(_q, &msg, 10);
    };
    Wire.setClock(400000);
    Wire.setSCL(17);
    Wire.setSDA(16);
    _keypad = Keypad_4x3::getInstance();
    _keypad->subscribe(keypadCallback);
    return 0;
}

int Controller::setupKBankNoteReader() {
    static SoftwareSerial ssBankNote(2, 3);

    return 0;
}

int setupMotor() {

    return 0;
}

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

void Controller::setup() {
    // Controller loop Queue Create
    _q = xQueueCreate(16, sizeof(Message));

    // Setup device
    setupKeypad();
    setupKBankNoteReader();

    setupModel();
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
