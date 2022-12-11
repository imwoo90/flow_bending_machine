#include <Controller.h>

Controller* ctrl;
static TaskHandle_t _mainTaskHandle = NULL;

static void mainTask(void *params) {
  ctrl = Controller::getInstance();
  ctrl->setup();

  while(1) {
    ctrl->loop();
  }
}

void setup()
{
  ctrl = Controller::getInstance();
  ctrl->setup();
  // xTaskCreate(mainTask, "mainTask", 4096, NULL, configMAX_PRIORITIES-1, &_mainTaskHandle);
  // vTaskCoreAffinitySet(_mainTaskHandle, 1 << 1);
}

void loop()
{
  ctrl->loop();
}
