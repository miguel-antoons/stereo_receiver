#pragma once

#include "BluetoothA2DPSink.h"
#include "audio/IAudioInput.hpp"

class BluetoothInput : public IAudioInput {
    // uses ESP32 A2DP sink; data callback pushes 44.1 kHz S16 stereo into
    // AudioPipeline
public:
    auto begin() -> bool override;  // register a2dp callbacks, start BT controller
    void end() override;
    auto isActive() const -> bool override { return this->running_; };  // peer connected?
    auto type() const -> AudioSourceType override { return AudioSourceType::Bluetooth; };
    auto name() const -> const char* override { return "Bluetooth"; };

private:
    bool running_ = false;
    BluetoothA2DPSink a2dp_sink_;
};