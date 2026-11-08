#pragma once

#include "audio/IAudioInput.hpp"


class BluetoothInput : public IAudioInput {
    // uses ESP32 A2DP sink; data callback pushes 44.1 kHz S16 stereo into AudioPipeline
    bool begin() override;   // register a2dp callbacks, start BT controller
    void end() override;
    bool isActive() const override; // peer connected?
};