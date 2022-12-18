#include <MachineData.h>
#include <LittleFS.h>

#define STATIC_DATA_PATH "/static_data"
#define COLUMN_DATA_PATH "/ColumData"
#define RELAY_DATA_PATH "/RelayData"

void MachineData::defineDefaultsData() {
    delay(5000);
    Serial.println("little fs format start");
    if ( !LittleFS.format() ) {
        Serial.println("little fs format failed");
        return ;
    }
    Serial.println("little fs format end");

    // initialize
    File _file = LittleFS.open(STATIC_DATA_PATH, "w");
    _file.truncate(4*NumOfStaticData);
    _file = LittleFS.open(COLUMN_DATA_PATH, "w");
    _file.truncate(4*NumofColumnData*MaxNumOfColumn);
    _file = LittleFS.open(RELAY_DATA_PATH, "w");
    _file.truncate(4*NumOfRelayData*MaxNumOfRelay);

    //default static length data
    setPasswordOfSystemManagement(999);
    setPasswordOfSystemSetting(1234);
    setPasswordOfVendingMachineModeSetting(12345678);
    setPasswordOfMainManagement(123456);
    setPasswordOfAdditionalStock(123456);
    setPasswordOfManualSales(123456);
    setPasswordOfPasswordChange(123456);
    setBanknoteReaderMode(1);
    setNumberOfRelay(0);
    setMoneyOfTotalSales(0);
    setNumberOfTotalSales(0);
    setMoneyOfManualSales(0);
    setNumberOfManualSales(0);
    setStaticData(IsInit, 1);
}
void MachineData::initialize() {
    if ( !LittleFS.begin() ) {
        Serial.println("LittleFS.begin() failed");
        return;
    }

    // setStaticData(IsInit, 0);

    if ( !LittleFS.exists(STATIC_DATA_PATH) || !getStaticData(IsInit)) {
        defineDefaultsData();
    }

    _numberOfRelays = getNumberOfRelay();
    _numberOfColumns = 0;
    for(int i = 0; i < _numberOfRelays; i++) {
        _numberOfColumns += getNumberOfChannels(i);
    }
}

void MachineData::setStaticData(StaticData id, uint32_t data) {
    File _file = LittleFS.open(STATIC_DATA_PATH, "r+");
    _file.seek(4*id, SeekSet);
    _file.write((uint8_t*)&data, sizeof(data));
    // _file.flush();
}
uint32_t MachineData::getStaticData(StaticData id) {
    uint32_t buf = 0;
    File _file = LittleFS.open(STATIC_DATA_PATH, "r");
    _file.seek(4*id, SeekSet);
    _file.read((uint8_t*)&buf, sizeof(buf));
    return buf;
}
void MachineData::initColumnData(int s, int e) {
    // std::vector<uint32_t> _init((e-s)*NumofColumnData);
    uint32_t* _init = new uint32_t[(e-s)*NumofColumnData];
    int init_idx = 0;
    for (int i = s; i < e; i++) {
        _init[init_idx++] = 1;
        _init[init_idx++] = 0;
        _init[init_idx++] = 0;
        _init[init_idx++] = i;
        _init[init_idx++] = 0;
        _init[init_idx++] = 0;
    }
    File _file = LittleFS.open(COLUMN_DATA_PATH, "r+");
    _file.seek(4*NumofColumnData*s, SeekSet);
    _file.write((uint8_t*)_init, 4*NumofColumnData*(e-s));
    delete _init;
}
void MachineData::setColumnData(int idx, ColumData id, uint32_t data) {
    FSInfo info;
    LittleFS.info(info);
    Serial.printf("total %d, used %d", info.totalBytes, info.usedBytes);

    File _file = LittleFS.open(COLUMN_DATA_PATH, "r+");
    _file.seek(4*NumofColumnData*idx + 4*id, SeekSet);
    _file.write((uint8_t*)&data, sizeof(data));
    // _file.flush();
}
uint32_t MachineData::getColumnData(int idx, ColumData id) {
    File _file = LittleFS.open(COLUMN_DATA_PATH, "r");

    uint32_t buf = 0;
    _file.seek(4*NumofColumnData*idx + 4*id, SeekSet);
    _file.read((uint8_t*)&buf, sizeof(buf));
    return buf;
}
void MachineData::setRelayData(int idx, RelayData id, uint32_t data) {
    FSInfo info;
    LittleFS.info(info);
    Serial.printf("total %d, used %d", info.totalBytes, info.usedBytes);

    File _file = LittleFS.open(RELAY_DATA_PATH, "r+");

    uint32_t buf = 0;
    _file.seek(4*NumOfRelayData*idx + 4*id, SeekSet);
    _file.write((uint8_t*)&data, sizeof(data));
    // _file.flush();
}
uint32_t MachineData::getRelayData(int idx, RelayData id) {
    File _file = LittleFS.open(RELAY_DATA_PATH, "r");

    uint32_t buf = 0;
    _file.seek(4*NumOfRelayData*idx + 4*id, SeekSet);
    _file.read((uint8_t*)&buf, sizeof(buf));
    return buf;
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
    int _s, _e;
    int delta = data - getNumberOfChannels(idx);
    if (delta == 0) {
        return;
    } else if (delta > 0 ) { // Increase column
        _s = _numberOfColumns;
        _e = _s + delta;
    } else { // Decrease column
        _e = _numberOfColumns;
        _s = _e + delta;
    }

    initColumnData(_s, _e);
    _numberOfColumns += delta;
    setRelayData(idx, NumberOfChannels, data);
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
    int delta = number - _numberOfRelays;
    if (delta == 0) {
        return;
    } else if (delta > 0) { // Increase number of relays
        int _s = _numberOfRelays;
        int _e = number;
        for(int i = _s; i < _e; i++) {
            setNumberOfChannels(i, 0); // Default channel number is 0
            setRelayType(i, 1); // Default relay type is 1
        }
    } else { // Decrease number of relays
        int _s = _numberOfRelays;
        int _e = number;
        for(int i = _s; i >= _e; i--) {
            setNumberOfChannels(i, 0);
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