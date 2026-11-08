#pragma once

#include <cstddef>
#include <cstdint>

class AudioPipeline {
public:
    auto write(const int16_t* frames, size_t count)
        -> bool;  // called from input/BT callback (core-agnostic)
    auto read(int16_t* frames, size_t count) -> size_t;  // called from audio task on core 0
    void setGain(float gain);                            // software volume (atomic)
    void flush();
    auto available() const -> size_t;
};