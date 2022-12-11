#include <Controller.h>

Controller* ctrl;

void setup()
{
  ctrl = Controller::getInstance();
  ctrl->setup();
}

void loop()
{
  ctrl->loop();
}
