#include "App.hpp"

#include <esp32-hal-log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/idf_additions.h>
#include <freertos/projdefs.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>

#include "audio/inputs/BluetoothInput.hpp"
#include "audio/outputs/MA12070POutput.hpp"
#include "audio/outputs/MAX98357AOutput.hpp"
#include "config/Config.hpp"

void App::audioTask_(void* app_ptr) {  // runs on core 0 forever
    App* app = static_cast<App*>(app_ptr);
    std::array<int16_t, AUDIO_TASK_BUFFER_SIZE> buf;
    for (;;) {
        if (size_t frames_read = app->pipeline_.read(buf); frames_read > 0) {
            app->output_->writeFrames(
                std::span<const int16_t>{buf}.first(frames_read * CHANNELS)
            );  // blocking I2S write paces the loop
        } else {
            vTaskDelay(1);
        }
    }
}

void App::uiTask_(void* app_ptr) {  // runs on core 1 forever
    App* app = static_cast<App*>(app_ptr);
    for (;;) {
        for (auto& c : app->controls_) c->poll();
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS_M));
    }
}

void App::setup() {
    output_ = std::make_unique<MAX98357AOutput>();
    if (!output_->begin()) {
        log_e("Output device failed to initialize. Restarting ...");
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS_M));
        ESP.restart();
    }

    inputs_.attachSink(&this->pipeline_);
    auto bt_input = std::make_unique<BluetoothInput>();
    bt_input->setRemoteVolumeCallback([this](float v01) { this->applyVolume_(v01, false); });
    this->bt_input_ = bt_input.get();
    inputs_.registerInput(std::move(bt_input));

    if (!inputs_.select(AudioSourceType::Bluetooth)) {
        log_e("Unable to select initial interface.");
    }

    // controls_.push_back(std::make_unique<RotaryControl>(...));
    for (auto& c : controls_) c->begin(*this);

    this->pipeline_.setGain(INITAL_GAIN);
}

void App::run() {
    if (int error = xTaskCreatePinnedToCore(
            audioTask_, "audio", AUDIO_TASK_STACK_SIZE, this, AUDIO_TASK_PRIORITY, nullptr, 0
        );
        error != pdPASS) {
        log_e("Error with code %d occured during pinning of audio task. Restarting ...", error);
        ESP.restart();
    }
    if (int error = xTaskCreatePinnedToCore(
            uiTask_, "ui", UI_TASK_STACK_SIZE, this, UI_TASK_PRIORITY, nullptr, 1
        );
        error != pdPASS) {
        log_e("Error with code %d occured during pinning of UI task. Restarting ...", error);
        ESP.restart();
    }
}

void App::setVolume(float v01) { this->applyVolume_(v01, true); }

auto App::volume() const -> float { return this->volume_.load(std::memory_order_acquire); }

void App::applyVolume_(float v01, bool notify_remote) {
    const float v = std::clamp(v01, 0.0F, 1.0F);
    this->volume_.store(v, std::memory_order_release);
    this->pipeline_.setGain(v);
    if (this->output_ != nullptr) {
        this->output_->setVolume(v);
    }
    if (notify_remote && this->bt_input_ != nullptr) {
        this->bt_input_->setRemoteVolume(v);
    }
}