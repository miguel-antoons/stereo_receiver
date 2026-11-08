#include <freertos/FreeRTOS.h>
#include "app/App.hpp"

App app;

void setup() {
    app.setup();
    app.run();
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}