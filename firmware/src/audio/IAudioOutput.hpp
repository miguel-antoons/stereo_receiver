#pragma once

#include <cstdint>

#include "AudioTools.h"

class IAudioOutput {
public:
    virtual ~IAudioOutput() = default;
    virtual auto begin() -> bool = 0;                        // init I2S (+ I2C device)
    virtual auto stream() -> audio_tools::AudioStream& = 0;  // the pipeline writes here
    // no hardware volume: volume is done in software (VolumeStream), so every
    // DAC behaves identically and hw volume stays at max
    virtual auto sampleRate() const -> uint32_t = 0;
};
