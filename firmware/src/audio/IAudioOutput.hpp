#pragma once

#include <array>
#include <cstdint>
#include <cstdio>
#include <span>

class IAudioOutput {
public:
    virtual ~IAudioOutput() = default;
    virtual auto begin() -> bool = 0;                               // init I2S (+ I2C device)
    virtual void writeFrames(std::span<const int16_t> frames) = 0;  // blocking I2S write
    virtual void setVolume(float v01) = 0;  // hw volume if supported, else software
    virtual auto sampleRate() const -> uint32_t = 0;
};
