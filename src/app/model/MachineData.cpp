#include <MachineData.h>
#include <LittleFS.h>

#define X(name) #name,
static const char* preFixStaticData[] = {
    STATIC_DATA
};
static const char* preFixColumData[] = {
    COLUMN_DATA
};

static const char* preFixRelayData[] = {
    RELAY_DATA
};
#undef X

String root = "/";

void MachineData::defineDefaultsData() {
    delay(5000);
    Serial.println("little fs format start");
    if ( !LittleFS.format() ) {
        Serial.println("little fs format failed");
        return ;
    }
    Serial.println("little fs format end");

    //default static length data
    const int numOfDefaultRelay = 2;
    setStaticData(IsInit, 1);
    setPasswordOfSystemManagement(999);
    setPasswordOfSystemSetting(1234);
    setPasswordOfVendingMachineModeSetting(12345678);
    setPasswordOfMainManagement(123456);
    setPasswordOfAdditionalStock(123456);
    setPasswordOfManualSales(123456);
    setPasswordOfPasswordChange(123456);
    setBanknoteReaderMode(1);
    setNumberOfRelay(numOfDefaultRelay);
    setMoneyOfTotalSales(0);
    setNumberOfTotalSales(0);
    setMoneyOfManualSales(0);
    setNumberOfManualSales(0);

    //default relay settings
    const int numOfDefaultChannels = 16;
    const int defaultMotorType = 1;
    int nRelay = getNumberOfRelay();
    for (int i = 0; i < nRelay; i++) {
        setNumberOfChannels(i, numOfDefaultChannels);
        setRelayType(i, defaultMotorType);
    }

    //default column settings
    const int numOfColumns = numOfDefaultRelay*numOfDefaultChannels;
    for (int i = 0; i < numOfColumns; i++) {
        setMotorType(i, 1);
        setQuantity(i, 0);
        setPrice(i, 0);
        setChannel(i, i);
        setAdditional(i, 0);
        setSalesAmount(i, 0);
    }
}
void MachineData::initialize() {
    if ( !LittleFS.begin() ) {
        Serial.println("LittleFS.begin() failed");
        return;
    }

    // LittleFS.remove("/IsInit");

    if ( !LittleFS.exists(root+preFixStaticData[IsInit]) ) {
        defineDefaultsData();
    }

    _numberOfRelays = getNumberOfRelay();
    _numberOfColumns = 0;
    for(int i = 0; i < _numberOfRelays; i++) {
        _numberOfColumns += getNumberOfChannels(i);
    }
}

void MachineData::setStaticData(StaticData id, uint32_t data) {
    File _file = LittleFS.open(root+preFixStaticData[id], "w");
    _file.write((uint8_t*)&data, sizeof(data));
    _file.flush();
    // _file.close();
}
uint32_t MachineData::getStaticData(StaticData id) {
    uint32_t buf = 0;
    File _file = LittleFS.open(root+preFixStaticData[id], "r");
    _file.read((uint8_t*)&buf, sizeof(buf));
    // _file.close();
    return buf;
}
uint32_t MachineData::getColumnData(int idx, ColumData id) {
    char tmp[32];
    String path = root + preFixColumData[id];
    String idx_str = itoa(idx, tmp, 10);
    File _file = LittleFS.open(path+idx_str, "r");

    uint32_t buf = 0;
    _file.read((uint8_t*)&buf, sizeof(buf));
    // _file.close();
    return buf;
}
uint32_t MachineData::getRelayData(int idx, RelayData id) {
    char tmp[32];
    String path = root + preFixRelayData[id];
    String idx_str = itoa(idx, tmp, 10);
    File _file = LittleFS.open(path+idx_str, "r");

    uint32_t buf = 0;
    _file.read((uint8_t*)&buf, sizeof(buf));
    // _file.close();
    return buf;
}
void MachineData::setColumnData(int idx, ColumData id, uint32_t data) {
    char tmp[32];
    String path = root + preFixColumData[id];
    String idx_str = itoa(idx, tmp, 10);
    File _file = LittleFS.open(path+idx_str, "w");

    uint32_t buf = 0;
    _file.write((uint8_t*)&data, sizeof(data));
    _file.flush();
    // _file.close();
}
void MachineData::setRelayData(int idx, RelayData id, uint32_t data) {
    char tmp[32];
    String path = root + preFixRelayData[id];
    String idx_str = itoa(idx, tmp, 10);
    File _file = LittleFS.open(path+idx_str, "w");

    uint32_t buf = 0;
    _file.write((uint8_t*)&data, sizeof(data));
    _file.flush();
    // _file.close();
}

//ColumData
uint32_t MachineData::getMotorType(int idx) {
    return getColumnData(idx, MotorTpye);
}
uint32_t MachineData::getQuantity(int idx) {
    return getColumnData(idx, Quantity);
}
uint32_t MachineData::getPrice(int idx) {
    return getColumnData(idx, Price);
}
uint32_t MachineData::getChannel(int idx) {
    return getColumnData(idx, Channel);
}
uint32_t MachineData::getAdditional(int idx) {
    return getColumnData(idx, Additional);
}
uint32_t MachineData::getSalesAmount(int idx) {
    return getColumnData(idx, SalesAmount);
}
void MachineData::setMotorType(int idx, uint32_t data) {
    setColumnData(idx, MotorTpye, data);
}
void MachineData::setQuantity(int idx, uint32_t data) {
    setColumnData(idx, Quantity, data);
}
void MachineData::setPrice(int idx, uint32_t data) {
    setColumnData(idx, Price, data);
}
void MachineData::setChannel(int idx, uint32_t data) {
    setColumnData(idx, Channel, data);
}
void MachineData::setAdditional(int idx, uint32_t data) {
    setColumnData(idx, Additional, data);
}
void MachineData::setSalesAmount(int idx, uint32_t data) {
    setColumnData(idx, SalesAmount, data);
}

//RelayData
uint32_t MachineData::getNumberOfChannels(int idx) {
    return getRelayData(idx, NumberOfChannels);
}
uint32_t MachineData::getRelayType(int idx) {
    return getRelayData(idx, RelayType);
}
void MachineData::setNumberOfChannels(int idx, uint32_t data) {
    int before = getNumberOfChannels(idx);
    setRelayData(idx, NumberOfChannels, data);
    _numberOfColumns -= (before-data);
}
void MachineData::setRelayType(int idx, uint32_t data) {
    setRelayData(idx, RelayType, data);
}

// StaticLengthData
uint32_t MachineData::getPasswordOfSystemManagement() {
    return getStaticData(PasswordOfSystemManagement);
}

uint32_t MachineData::getPasswordOfSystemSetting() {
    return getStaticData(PasswordOfSystemSetting);
}

uint32_t MachineData::getPasswordOfVendingMachineModeSetting() {
    return getStaticData(PasswordOfVMMS);
}

uint32_t MachineData::getPasswordOfMainManagement() {
    return getStaticData(PasswordOfMainManagement);
}

uint32_t MachineData::getPasswordOfAdditionalStock() {
    return getStaticData(PasswordOfAdditionalStock);
}

uint32_t MachineData::getPasswordOfManualSales() {
    return getStaticData(PasswordOfManualSales);
}

uint32_t MachineData::getPasswordOfPasswordChange() {
    return getStaticData(PasswordOfPasswordChange);
}
uint32_t MachineData::getBanknoteReaderMode() {
    return getStaticData(BanknoteReaderMode);
}
uint32_t MachineData::getNumberOfRelay() {
    return getStaticData(NumberOfRelay);
}
uint32_t MachineData::getMoneyOfTotalSales() {
    return getStaticData(MoneyOfTotalSales);
}
uint32_t MachineData::getNumberOfTotalSales() {
    return getStaticData(NumberOfTotalSales);
}
uint32_t MachineData::getMoneyOfManualSales() {
    return getStaticData(MoneyOfManualSales);
}
uint32_t MachineData::getNumberOfManualSales() {
    return getStaticData(NumberOfManualSales);
}
void MachineData::setPasswordOfSystemManagement(uint32_t password) {
    setStaticData(PasswordOfSystemManagement, password);
}
void MachineData::setPasswordOfSystemSetting(uint32_t password) {
    setStaticData(PasswordOfSystemSetting, password);
}
void MachineData::setPasswordOfVendingMachineModeSetting(uint32_t password) {
    setStaticData(PasswordOfVMMS, password);
}
void MachineData::setPasswordOfMainManagement(uint32_t password) {
    setStaticData(PasswordOfMainManagement, password);
}
void MachineData::setPasswordOfAdditionalStock(uint32_t password) {
    setStaticData(PasswordOfAdditionalStock, password);
}
void MachineData::setPasswordOfManualSales(uint32_t password) {
    setStaticData(PasswordOfManualSales, password);
}
void MachineData::setPasswordOfPasswordChange(uint32_t password) {
    setStaticData(PasswordOfPasswordChange, password);
}
void MachineData::setBanknoteReaderMode(uint32_t mode) {
    setStaticData(BanknoteReaderMode, mode);
}
void MachineData::setNumberOfRelay(uint32_t number) {
    if ( _numberOfRelays == number )
        return;
    else if ( number < _numberOfRelays) {
        for (int i = _numberOfRelays-1; i >= number; i--) {
            _numberOfColumns -= getRelayData(i, NumberOfChannels);
        }
    }

    _numberOfRelays = number;
    setStaticData(NumberOfRelay, number);
}
void MachineData::setMoneyOfTotalSales(uint32_t money) {
    setStaticData(MoneyOfTotalSales, money);
}
void MachineData::setNumberOfTotalSales(uint32_t number) {
    setStaticData(NumberOfTotalSales, number);
}
void MachineData::setMoneyOfManualSales(uint32_t money) {
    setStaticData(MoneyOfManualSales, money);
}
void MachineData::setNumberOfManualSales(uint32_t number) {
    setStaticData(NumberOfManualSales, number);
}