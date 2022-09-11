#ifndef BANKNOTE_READER_H
#define BANKNOTE_READER_H
#include <FreeRTOS.h>
#include <task.h>

#include <functional>
#include <vector>

class BanknoteReader {
private:
    std::vector<std::function<void(const int)> > _buillDataCallbacks;
    TaskHandle_t _eventTask;
protected:
    virtual int getBillData();
public:
    void notifyBillData();

    void registerBillDataCallBack(std::function<void(const int)> billdata);
    virtual int initialized(const char* taskName);
    virtual void enable();
    virtual void disable();
};

#endif