#pragma once

#include <MachineState.h>
#include <MachineData.h>

class ListOfColumnData : public MachineState {
private:
    int _page;

    ListOfColumnData() {}
    virtual void initialize();

    void loadListOfColumnData();
public:
    static ListOfColumnData* getInstance();

    virtual MachineState* pressKey(const char key);
};
