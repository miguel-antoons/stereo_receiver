#pragma once

#include <functional>

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
    void setRemoteVolumeCallback(std::function<void(float)> callback) {
        this->remote_volume_callback_ = std::move(callback);
    }
    void setRemoteVolume(float v01);   // push receiver volume to the phone (AVRCP)
    void dispatchRemoteVolume(int volume);  // called from the AVRCP volume-change trampoline

private:
    bool running_ = false;
    BluetoothA2DPSink a2dp_sink_;
    A2DPNoVolumeControl volume_control_;  // lib must not scale: gain is applied in AudioPipeline
    std::function<void(float)> remote_volume_callback_;
};