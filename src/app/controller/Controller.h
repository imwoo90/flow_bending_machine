#pragma ocee

#include <Arduino.h>
#include <FlowVendingMachine.h>
#include <device/Keypad_4x3/Keypad_4x3.h>
#include <device/BanknoteReader/BanknoteReader.h>
#include <device/Relay/Relay.h>
#include <Display.h>

#include <FreeRTOS.h>
#include <queue.h>
#include <timers.h>

typedef enum {
    MessageInitial,
    MessageKeypadPress,
    MessageKeypadRelease,
    MessageBanknoteRecognize,
    MessageTimeout,

    MessageBanknoteReaderEnable,
    MessageBanknoteReaderDisable,
    MessageRelayOpen,
    MessageRelayClose,
    MessageRunning,
    MessageTEST,
} MessageType;

struct Message {
    MessageType type;
    int data;
};

class Controller {
private:
    int _isInitOk = true;
    QueueHandle_t _q;
    TimerHandle_t _keypadOffLedTimer;

    Display* _display;

    FlowVendingMachine* _machine;
    Keypad_4x3* _keypad;
    BanknoteReader* _bankNoteReader;
    std::vector<Relay*> _relays;

    int setupKeypad();
    int setupBankNoteReader();
    int setupRelays();
    int setupMachine();

    std::pair<int, int> convertChannelToRelayFromModel(int channel) {
        /*
            param channel is started from 1
            and relay address start is 1 and the relay index is 0
            so param channel is 1 convert to relay idx 0 and relay channel is 1
        */
        int idx = 0;
        // channel -= 1; // change channel start to 0
        while(channel >= _relays[idx]->_numOfChannels) {
            channel -= _relays[idx]->_numOfChannels;
            idx += 1;
        }
        return std::pair<int, int>(idx, channel+1);
    }

    void processModel(Message &message);
    void operateDevice(Message &message);

    Controller() {}
public:
    static Controller* getInstance() {
        static Controller singleton_instance;
        return &singleton_instance;
    }

    bool _isISR = false;
    void putMessage(MessageType type, int data, int delay_ms = 0);

    void setup();
    void loop();
};
