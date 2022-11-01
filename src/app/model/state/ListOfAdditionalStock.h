#pragma once

#include <MachineState.h>
#include <MachineData.h>

class ListOfAdditionalStock : public MachineState {
private:
    int _page;

    ListOfAdditionalStock() {}
    virtual void initialize();

    void loadListOfAdditionalStock();
public:
    static ListOfAdditionalStock* getInstance();

    virtual MachineState* pressKey(const char key);
};
