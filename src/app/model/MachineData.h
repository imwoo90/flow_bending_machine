#pragma once

#include <Arduino.h>

typedef enum {
    Quantity,
    Price = Quantity+4,
    Channel = Price+4,
    Additional = Channel+4,
    SalesAmount = Additional+4,
} ColumDataPos;

typedef enum {
    NumberOfChannels,
    MotorType = NumberOfChannels+4,
} RelayDataPos;

typedef enum {
    PasswordOfSystemManagement = 0,
    PasswordOfSystemSetting = PasswordOfSystemManagement+4,
    PasswordOfVendingMachineModeSetting = PasswordOfSystemSetting+4,
    PasswordOfMainManagement = PasswordOfVendingMachineModeSetting+4,
    PasswordOfAdditionalStock = PasswordOfMainManagement+4,
    PasswordOfManualSales = PasswordOfAdditionalStock+4,
    PasswordOfPasswordChange = PasswordOfManualSales+4,
    ModeOfBanknoteReader = PasswordOfPasswordChange+4,
    NumberOfRelay = ModeOfBanknoteReader+4,
    MoneyOfTotalSales = NumberOfRelay+4,
    NumberOfTotalSales = MoneyOfTotalSales+4,
    MoneyOfManualSales = NumberOfTotalSales+4,
    NumberOfManualSales = MoneyOfManualSales+4,
} StaticLengthDataPos;

class MachineData {
private:
    const uint32_t _kRelayDataSize = MotorType+4;
    const uint32_t _kColumeDataSize = SalesAmount+4;
    uint32_t _numberOfRelays = 0;
    uint32_t _numberOfColumns = 0;

    //static length data
    void setStaticData(StaticLengthDataPos pos, uint32_t data);
    uint32_t getStaticData(StaticLengthDataPos pos);

    //dynamic length data
    uint32_t getRelayData(int idx, RelayDataPos pos);
    uint32_t getColumnData(int idx, ColumDataPos pos);
    void setRelayData(int idx, RelayDataPos pos, uint32_t data);
    void setColumnData(int idx, ColumDataPos pos, uint32_t data);

    MachineData() {}
public:
    static MachineData* getInstance() {
        static MachineData singleton_instance;
        return &singleton_instance;
    }
    inline uint32_t getNumberOfColumns() { return _numberOfColumns;}
    inline uint32_t getNumberOfRelays() { return _numberOfRelays;}

    void defineDefaultsData();
    void initialize();

    //ColumData
    uint32_t getQuantity(int idx);
    uint32_t getPrice(int idx);
    uint32_t getChannel(int idx);
    uint32_t getAdditional(int idx);
    uint32_t getSalesAmount(int idx);
    void setQuantity(int idx, uint32_t data);
    void setPrice(int idx, uint32_t data);
    void setChannel(int idx, uint32_t data);
    void setAdditional(int idx, uint32_t data);
    void setSalesAmount(int idx, uint32_t data);

    //RelayData
    uint32_t getNumberOfChannels(int idx);
    uint32_t getMotorType(int idx);
    void setNumberOfChannels(int idx, uint32_t data);
    void setMotorType(int idx, uint32_t data);

    // StaticLengthData
    uint32_t getPasswordOfSystemManagement();
    uint32_t getPasswordOfSystemSetting();
    uint32_t getPasswordOfVendingMachineModeSetting();
    uint32_t getPasswordOfMainManagement();
    uint32_t getPasswordOfAdditionalStock();
    uint32_t getPasswordOfManualSales();
    uint32_t getPasswordOfPasswordChange();
    uint32_t getModeOfBanknoteReader();
    uint32_t getNumberOfRelay();
    uint32_t getMoneyOfTotalSales();
    uint32_t getNumberOfTotalSales();
    uint32_t getMoneyOfManualSales();
    uint32_t getNumberOfManualSales();
    void setPasswordOfSystemManagement(uint32_t password);
    void setPasswordOfSystemSetting(uint32_t password);
    void setPasswordOfVendingMachineModeSetting(uint32_t password);
    void setPasswordOfMainManagement(uint32_t password);
    void setPasswordOfAdditionalStock(uint32_t password);
    void setPasswordOfManualSales(uint32_t password);
    void setPasswordOfPasswordChange(uint32_t password);
    void setModeOfBanknoteReader(uint32_t mode);
    void setNumberOfRelay(uint32_t number);
    void setMoneyOfTotalSales(uint32_t money);
    void setNumberOfTotalSales(uint32_t number);
    void setMoneyOfManualSales(uint32_t money);
    void setNumberOfManualSales(uint32_t number);


};
