#include "Keypad_4x3.h"
#include <vector>

#include <FreeRTOS.h>
#include <task.h>

static std::vector<std::function<void(const char)> > _callbacks;

void Keypad_4x3::pin_mode(byte pinNum, byte mode) {
    // pcf8574.pinMode(pinNum, mode);
}
void Keypad_4x3::pin_write(byte pinNum, boolean level) {
    pcf8574.digitalWrite(pinNum, level);
}
int  Keypad_4x3::pin_read(byte pinNum) {
    int read = pcf8574.digitalRead(pinNum, true);
    return read;
}

static void notify() {
    Keypad_4x3* kp = Keypad_4x3::getInstance();
    char key = kp->getKey();
    if (key == NO_KEY)
        return;

    // if (key == 27) // esc
    //     key = '*';
    // else if (key == 9) // tab
    //     key = '#';

    for (auto i = 0; i < _callbacks.size(); i++) {
        _callbacks[i](key);
    }
}

static void scan_task(void *params) {
    while(1) {
        notify();
        delay(50);
    }
}

static void task_initialize() {
    enum {
        Priority = configMAX_PRIORITIES / 2 - 1,
    };

    static TaskHandle_t _keypadTask;
    xTaskCreate(scan_task, "KEYPAD_4X3", 1024, 0, Priority, &_keypadTask);
}

Keypad_4x3::Keypad_4x3(char *userKeymap, byte *row, byte *col, byte numRows, byte numCols) :
    Keypad(userKeymap, row, col, numRows, numCols), pcf8574(0x20) {
    for(int r = 0; r < numRows; r++)
        pcf8574.pinMode(row[r], INPUT_PULLUP);
    for(int c = 0 ; c < numCols; c++) {
        pcf8574.pinMode(col[c], OUTPUT);
        pcf8574.digitalWrite(col[c], HIGH);
    }

    //LED pin settings
    pcf8574.pinMode(P7, OUTPUT);

    pcf8574.begin();
    pinMode(3, OUTPUT);

    task_initialize();
}

Keypad_4x3* Keypad_4x3::getInstance() {
    static const byte _ROWS = 4; //four rows
    static const byte _COLS = 3; //three columns
    // static byte _rowPins[_ROWS] = {P0, P1, P2, P3}; //connect to the row pinouts of the keypad
    // static byte _colPins[_COLS] = {P6, P5, P4}; //connect to the column pinouts of the keypad
    static byte _rowPins[_ROWS] = {P6, P5, P4, P3};
    static byte _colPins[_COLS] = {P0, P1, P2};
    static char _keys[_ROWS][_COLS] = {
        {'1','2','3'},
        {'4','5','6'},
        {'7','8','9'},
        {'*','0','#'}
    };
    static Keypad_4x3* singleton_instance = NULL;

    if(singleton_instance == NULL) {
        singleton_instance = new Keypad_4x3(makeKeymap(_keys), _rowPins, _colPins, _ROWS, _COLS);
    }
    return singleton_instance;
}

void Keypad_4x3::onLED() {
    digitalWrite(3, LOW);
}

void Keypad_4x3::offLED() {
    digitalWrite(3, HIGH);
}

void Keypad_4x3::subscribe(std::function<void(const char)> callback) {
    _callbacks.push_back(callback);
}