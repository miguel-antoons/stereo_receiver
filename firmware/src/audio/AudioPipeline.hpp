#pragma once

#include <cstdint>
#include <cstdio>


class AudioPipeline {
public:
    bool write(const int16_t* frames, size_t count);  // called from input/BT callback (core-agnostic)
    size_t read(int16_t* frames, size_t count);      // called from audio task on core 0
    void setGain(float g);                            // software volume (atomic)
    void flush();
    size_t available() const;
};