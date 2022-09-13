#include <MachineData.h>
#include <LittleFS.h>

#define STATIC_LENGTH_DATA "/static_length_data.bin"
#define COLUMN_DATA "/column_data.bin"
#define RELAY_DATA "/relay_data.bin"

void MachineData::defineDefaultsData() {
    if ( !LittleFS.format() ) {
        Serial.println("little fs format failed");
        return ;
    }

    //default static length data
    const int numOfDefaultRelay = 2;
    File staticF = LittleFS.open(STATIC_LENGTH_DATA, "w");
    staticF.truncate(NumberOfManualSales+4);
    staticF.close();
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
    File relayF = LittleFS.open(RELAY_DATA, "w");
    relayF.close();
    const int numOfDefaultChannels = 16;
    const int defaultMotorType = 1;
    int nRelay = getNumberOfRelay();
    for (int i = 0; i < nRelay; i++) {
        setNumberOfChannels(i, numOfDefaultChannels);
        setMotorType(i, defaultMotorType);
    }

    //default column settings
    File columnF = LittleFS.open(COLUMN_DATA, "w");
    columnF.close();
    const int numOfColumns = numOfDefaultRelay*numOfDefaultChannels;
    for (int i = 0; i < numOfColumns; i++) {
        setQuantity(i, 0);
        setPrice(i, 0);
        setChannel(i, i);
        setAdditional(i, 0);
        setSalesAmount(i, 0);
    }
}
void MachineData::initialize() {
    delay(5000);
    if ( !LittleFS.begin() ) {
        Serial.println("LittleFS.begin() failed");
        return;
    }
    if ( !LittleFS.exists(STATIC_LENGTH_DATA) ) {
        defineDefaultsData();
    }

    _numberOfRelays = getNumberOfRelay();
    _numberOfColumns = 0;
    for(int i = 0; i < _numberOfRelays; i++) {
        _numberOfColumns += getNumberOfChannels(i);
    }
}

void MachineData::setStaticData(StaticLengthDataPos pos, uint32_t data) {
    File staticF = LittleFS.open(STATIC_LENGTH_DATA, "r+");
    staticF.seek(pos);
    staticF.write((uint8_t*)&data, sizeof(data));
    staticF.close();
}
uint32_t MachineData::getStaticData(StaticLengthDataPos pos) {
    uint32_t buf = 0;
    File staticF = LittleFS.open(STATIC_LENGTH_DATA, "r");
    staticF.seek(pos);
    staticF.read((uint8_t*)&buf, sizeof(buf));
    staticF.close();
    return buf;
}
uint32_t MachineData::getColumnData(int idx, ColumDataPos pos) {
    if ( _numberOfColumns < idx) {
        Serial.println("Colume Index Overflow");
        return -1;
    }

    uint32_t buf = 0;
    File columnF = LittleFS.open(COLUMN_DATA, "r+");
    int diff =  _numberOfColumns*_kColumeDataSize - columnF.size();
    if ( diff > 0 ) {
        columnF.seek(SeekEnd);
        columnF.truncate(_numberOfColumns*_kColumeDataSize);
        std::vector<uint8_t> dummy(diff, 0);
        columnF.write(&dummy[0], dummy.size());
    } else if ( diff < 0 )
        columnF.truncate(_numberOfColumns*_kColumeDataSize);

    columnF.seek(_kColumeDataSize*idx+pos);
    columnF.read((uint8_t*)&buf, sizeof(buf));
    columnF.close();
    return buf;
}
uint32_t MachineData::getRelayData(int idx, RelayDataPos pos) {
    if ( _numberOfRelays < idx ) {
        Serial.println("Relay Index Overflow");
        return -1;
    }

    uint32_t buf = 0;
    File relayF = LittleFS.open(RELAY_DATA, "r+");
    int diff = _numberOfRelays*_kRelayDataSize - relayF.size();
    if ( diff > 0 ) {
        relayF.seek(SeekEnd);
        relayF.truncate(_numberOfRelays*_kRelayDataSize);
        std::vector<uint8_t> dummy(diff, 0);
        relayF.write(&dummy[0], dummy.size());
    } else if ( diff < 0 )
        relayF.truncate(_numberOfRelays*_kRelayDataSize);

    relayF.seek(_kRelayDataSize*idx+pos);
    relayF.read((uint8_t*)&buf, sizeof(buf));
    relayF.close();
    return buf;
}
void MachineData::setColumnData(int idx, ColumDataPos pos, uint32_t data) {
    if ( _numberOfColumns < idx) {
        Serial.println("Colume Index Overflow");
        return;
    }

    File columnF = LittleFS.open(COLUMN_DATA, "r+");
    int diff =  _numberOfColumns*_kColumeDataSize - columnF.size();
    if ( diff > 0 ) {
        columnF.seek(SeekEnd);
        columnF.truncate(_numberOfColumns*_kColumeDataSize);
        std::vector<uint8_t> dummy(diff, 0);
        columnF.write(&dummy[0], dummy.size());
    } else if ( diff < 0 )
        columnF.truncate(_numberOfColumns*_kColumeDataSize);

    columnF.seek(_kColumeDataSize*idx+pos);
    columnF.write((uint8_t*)&data, sizeof(data));
    columnF.close();
}
void MachineData::setRelayData(int idx, RelayDataPos pos, uint32_t data) {
    if ( _numberOfRelays < idx ) {
        Serial.println("Relay Index Overflow");
        return;
    }

    File relayF = LittleFS.open(RELAY_DATA, "r+");
    int diff = _numberOfRelays*_kRelayDataSize - relayF.size();
    if ( diff > 0 ) {
        relayF.seek(SeekEnd);
        relayF.truncate(_numberOfRelays*_kRelayDataSize);
        std::vector<uint8_t> dummy(diff, 0);
        relayF.write(&dummy[0], dummy.size());
    } else if ( diff < 0 )
        relayF.truncate(_numberOfRelays*_kRelayDataSize);

    relayF.seek(_kRelayDataSize*idx+pos);
    relayF.write((uint8_t*)&data, sizeof(data));
    relayF.close();
}

//ColumData
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
uint32_t MachineData::getMotorType(int idx) {
    return getRelayData(idx, MotorType);
}
void MachineData::setNumberOfChannels(int idx, uint32_t data) {
    int before = getNumberOfChannels(idx);
    setRelayData(idx, NumberOfChannels, data);
    _numberOfColumns -= (before-data);
}
void MachineData::setMotorType(int idx, uint32_t data) {
    setRelayData(idx, MotorType, data);
}

// StaticLengthData
uint32_t MachineData::getPasswordOfSystemManagement() {
    return getStaticData(PasswordOfSystemManagement);
}

uint32_t MachineData::getPasswordOfSystemSetting() {
    return getStaticData(PasswordOfSystemSetting);
}

uint32_t MachineData::getPasswordOfVendingMachineModeSetting() {
    return getStaticData(PasswordOfVendingMachineModeSetting);
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
    setStaticData(PasswordOfVendingMachineModeSetting, password);
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