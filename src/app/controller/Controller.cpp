#include <Controller.h>

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

void Controller::setup() {
    // Controller loop Queue Create
    _q = xQueueCreate(16, sizeof(Message));

    //Keypad setup
    auto keypadCallback = [&](const char key) {
        Message msg;
        msg.type = MessageKeypadPress;
        msg.data = key;
        Serial.println(key);
        xQueueSend(_q, &msg, 10);
    };
    Wire.setClock(400000);
    Wire.setSCL(17);
    Wire.setSDA(16);
    _keypad = Keypad_4x3::getInstance();
    _keypad->subscribe(keypadCallback);

    //State Machine Setup
    _machine = FlowBendingMachine::getInstance();
    auto onChangedCallback = [&](std::unordered_map<std::string, std::string> data) {
        Serial.printf("state: %s\n\r", data["state"]);
        std::string param = "param_0";
        Serial.printf("param: ");
        for(int i = 0; i < 9; i++) {
            param[6] = '0' + i;
            if (data.find(param) == data.end())
                break;
            Serial.printf("%d=%s", i, data[param]);
        }
        Serial.printf("\n\r");
    };
    auto timeoutCallback = [&](const int signal) {
        Message msg;
        msg.type = MessageTimeout;
        msg.data = signal;
        xQueueSend(_q, &msg, 10);
    };
    _machine->begin(1, onChangedCallback, timeoutCallback);
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


Controller flowBendingMachineController;

