#include "OBH_K03S.h"

#define checksum(x) (byte)(x[1]+x[2]+x[3])

bool OBH_K03S::receiveCommnad(const char* _buf, const char* cmd) {
    return _buf[1] == cmd[0] && _buf[2] == cmd[1];
}
void OBH_K03S::sendCommand(const char* cmd) {
    char _buf[5] = {
        '$', cmd[0], cmd[1], cmd[2]
    };
    _buf[4] = checksum(_buf);
    _serial->write(_buf, 5);
}

OBH_K03S::OBH_K03S(Stream &serial) {
    _serial = &serial;
}

int OBH_K03S::initialized(const char* taskName) {
    char _buf[5];
    //reset
    char cmd[3] = {'R', 'S', 'T'};
    sendCommand(cmd);
    _serial->readBytes(_buf, 5);
    if (receiveCommnad(_buf, "NG")) {
        Serial.println("OHB_K03S reset error");
        return -1;
    }
    delay(2500);

    // full duplex mode, auto stacking, 1000, 5000, 10000
    // set config 0x37
    cmd[0] = 'S'; cmd[1] = 'C'; cmd[2] = 0x37;
    sendCommand(cmd);
    _serial->readBytes(_buf, 5);
    if (receiveCommnad(_buf, "NG")) {
        Serial.println("OHB_K03S config error");
        return -1;
    }
    BanknoteReader::initialized(taskName);
    return 0;
}

int OBH_K03S::getBilldata() {
    char _buf[5];
    while(1) {
        if (process(_buf) < 0)
            continue;

        if (receiveCommnad(_buf, "gb")) { //Receive BillData
            return _buf[3];
        }
    }
    return -1;
}

void OBH_K03S::enable() {
    char cmd[3] = {'S', 'A', 0x0d};
    sendCommand(cmd);
}

void OBH_K03S::disable() {
    char cmd[3] = {'S', 'A', 0x0e};
    sendCommand(cmd);
}

int OBH_K03S::process(char _buf[5]) {
    _serial->readBytes(_buf, 5);
    if (checksum(_buf) != _buf[4]) {
        Serial.println("OHB_K03S checksum error");
        return -1;
    }

    if (receiveCommnad(_buf, "ES")) { //Receive Event Tx
        //Ack
        char ack[3] = {'e', 's', _buf[3]};
        sendCommand(ack);

        ActiveStatus reg = (ActiveStatus)_buf[3];
        activeStatusProcess(reg);
    } else if (receiveCommnad(_buf, "gb")) { //Receive BillData
        Serial.printf("OHB_K03S billData 0x%2x\n\r", _buf[3]);
    } else {
        return -1;
    }
    return 0;
}

int OBH_K03S::activeStatusProcess(ActiveStatus reg) {
    switch (reg)
    {
    case RECOGNITION_END:
        sendCommand("GB?");
        break;
    default:
        break;
    }
    return 0;
}