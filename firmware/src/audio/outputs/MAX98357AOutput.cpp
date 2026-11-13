
#include "audio/outputs/MAX98357AOutput.hpp"

#include <portmacro.h>

#define BLCK_PIN GPIO_NUM_14  // D6 on ESP32 N16R2
#define LRCK_PIN GPIO_NUM_25  // D2 on ESP32 N16R2
#define DOUT_PIN GPIO_NUM_26  // D3 on ESP32 N16R2

auto MAX98357AOutput::begin() -> bool {
    this->configureI2s(BLCK_PIN, LRCK_PIN, DOUT_PIN, SAMPLE_RATE_44100);
    return true;
}

void MAX98357AOutput::writeFrames(std::span<const int16_t> frames) {
    ESP_ERROR_CHECK(i2s_channel_write(
        this->i2s_handle_, frames.data(), frames.size_bytes(), nullptr, portMAX_DELAY
    ));
}
