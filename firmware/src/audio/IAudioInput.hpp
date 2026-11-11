#pragma once

#include "audio/AudioPipeline.hpp"

enum class AudioSourceType { Bluetooth, Wifi, Optical, Jack, Usb };

class IAudioInput {
public:
    virtual ~IAudioInput() = default;
    virtual auto begin() -> bool = 0;  // start hardware/BT stack
    virtual void end() = 0;            // stop + release
    virtual auto type() const -> AudioSourceType = 0;
    virtual auto name() const -> const char* = 0;
    virtual auto isActive() const -> bool = 0;  // e.g. BT peer connected
    // data callback target set by InputManager:
    void attachSink(AudioPipeline* pipeline) { this->sink_ = pipeline; };

protected:
    AudioPipeline* sink_{nullptr};
};