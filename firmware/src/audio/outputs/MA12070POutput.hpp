#pragma once

#include <cstdint>
#include <span>

#include "audio/outputs/I2sOutputBase.hpp"

class MA12070POutput : public I2sOutputBase {
public:
    auto begin() -> bool override;                               // init I2S (+ I2C device)
    void writeFrames(std::span<const int16_t> frames) override;  // blocking I2S write
    void setVolume(float v01) override;  // hw volume if supported, else software
    auto sampleRate() const -> uint32_t override;

private:
    float volume_{1};
};