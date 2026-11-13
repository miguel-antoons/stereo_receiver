#include "App.hpp"

#include <esp32-hal-log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/idf_additions.h>
#include <freertos/projdefs.h>

#include <cstdint>
#include <memory>

#include "audio/inputs/BluetoothInput.hpp"
#include "audio/outputs/I2sStreamOutput.hpp"
#include "config/Config.hpp"

void App::audioTask_(void* app_ptr) {  // runs on core 0 forever
    App* app = static_cast<App*>(app_ptr);
    for (;;) {
        if (app->copier_.copy() == 0) {
            vTaskDelay(1);  // no data available (e.g. BT peer disconnected)
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
    output_ = std::make_unique<I2sStreamOutput>();  // any I2S DAC; volume is software-only
    if (!output_->begin()) {
        log_e("Output device failed to initialize. Restarting ...");
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS_M));
        ESP.restart();
    }

    this->volume_.setOutput(output_->stream());
    this->volume_.begin(audio_tools::AudioInfo{SAMPLE_RATE_44100, CHANNELS, BITS_PER_SAMPLE});
    this->setVolume(INITAL_GAIN);

    inputs_.setOnInputChanged([this](IAudioInput& input) {
        this->copier_.begin(this->volume_, input.stream());
    });
    auto bt_input = std::make_unique<BluetoothInput>();
    inputs_.registerInput(std::move(bt_input));

    if (!inputs_.select(AudioSourceType::Bluetooth)) {
        log_e("Unable to select initial interface.");
    }

    // controls_.push_back(std::make_unique<RotaryControl>(...));
    for (auto& c : controls_) c->begin(*this);
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

void App::setVolume(float volume) { this->volume_.setVolume(volume); }

void App::toggleMute() {
    if (this->muted_) {
        this->setVolume(this->volume_before_mute_);
        this->muted_ = false;
    } else {
        this->volume_before_mute_ = this->volume();
        this->setVolume(0.0F);
        this->muted_ = true;
    }
}

auto App::volume() -> float { return this->volume_.volume(); }
