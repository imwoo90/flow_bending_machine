#include "BanknoteReader.h"

static void banknoteEventTask(void *params) {
    BanknoteReader* reader = (BanknoteReader*)params;
    while(1) {
        reader->notifyBillData();
    }
}

void BanknoteReader::notifyBillData() {
    int billdata = getBillData();
    for (auto cb : _onRecognizedBankNotes) {
        cb(billdata);
    }
}

void BanknoteReader::registerBillDataCallBack(std::function<void(const int)> onRecognizedBankNote) {
    _onRecognizedBankNotes.push_back(onRecognizedBankNote);
}

int BanknoteReader::initialized() {
    enum {
        Priority = configMAX_PRIORITIES/2 - 1,
    };
    _q = xQueueCreate(16, 0);
    disable();
    xTaskCreate(banknoteEventTask, _name.c_str(), 512, this, Priority, &_eventTask);
    vTaskCoreAffinitySet(_eventTask, 1 << 1);
    return 0;
}
