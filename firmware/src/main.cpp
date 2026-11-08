#include <freertos/FreeRTOS.h>

#include "app/App.hpp"

constexpr int LOOP_WAIT_MS = 1000;

App app;

void setup() {
    app.setup();
    app.run();
}

void loop() { vTaskDelay(pdMS_TO_TICKS(LOOP_WAIT_MS)); }