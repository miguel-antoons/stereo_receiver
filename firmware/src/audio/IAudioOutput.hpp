#pragma once

#include <cstdint>
#include <cstdio>

class IAudioOutput {
public:
    virtual ~IAudioOutput() = default;
    virtual auto begin() -> bool = 0;                                   // init I2S (+ I2C device)
    virtual void writeFrames(const int16_t* frames, size_t count) = 0;  // blocking I2S write
    virtual void setVolume(float v01) = 0;  // hw volume if supported, else software
    virtual auto sampleRate() const -> uint32_t = 0;
};
