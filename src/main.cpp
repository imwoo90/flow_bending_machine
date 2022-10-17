#include <Controller.h>

Controller* ctr;
void setup()
{
  ctr = Controller::getInstance();
  ctr->setup();
}

void loop()
{
  ctr->loop();
}
