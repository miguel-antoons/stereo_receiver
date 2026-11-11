#include "MA12070POutput.hpp"

#include <esp32-hal-log.h>

#include <span>

auto MA12070POutput::begin() -> bool { return true; }

void MA12070POutput::writeFrames(std::span<const int16_t> frames) {
    const size_t sample_count = frames.size();
    // temp, to shut up the linter
    log_i("%d", this->volume_);
    if (sample_count == 0) {
        return;
    }
}

void MA12070POutput::setVolume(float v01) {}

auto MA12070POutput::sampleRate() const -> uint32_t { return 0; }