#pragma once

#include "AudioTools.h"
#include "AudioTools/Communication/A2DPStream.h"
#include "audio/IAudioInput.hpp"

class BluetoothInput : public IAudioInput {
    // uses the ESP32 A2DP sink via audio-tools' A2DPStream; provides 44.1 kHz
    // S16 stereo as an AudioStream
public:
    auto begin() -> bool override;                                      // start the A2DP sink
    void end() override;                                                // stop the A2DP sink
    auto isActive() const -> bool override { return this->running_; };  // peer connected?
    auto type() const -> AudioSourceType override { return AudioSourceType::Bluetooth; };
    auto name() const -> const char* override { return "Bluetooth"; };
    auto stream() -> audio_tools::AudioStream& override { return this->a2dp_; };

private:
    bool running_ = false;
    audio_tools::A2DPStream a2dp_;
};
