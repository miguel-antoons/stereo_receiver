#pragma once

#include <cstdint>

#include "AudioTools.h"
#include "audio/IAudioOutput.hpp"
#include "config/Config.hpp"

// I2S DAC output based on audio-tools' I2SStream (replaces the raw i2s_std
// boilerplate); the pipeline writes PCM to stream(). Volume is software-only
// (VolumeStream in the pipeline), so the DAC itself always runs at max.
class I2sStreamOutput : public IAudioOutput {
public:
    auto begin() -> bool override;
    auto stream() -> audio_tools::AudioStream& override { return this->i2s_; };
    auto sampleRate() const -> uint32_t override { return SAMPLE_RATE_44100; };

protected:
    audio_tools::I2SStream i2s_;
};
