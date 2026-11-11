#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <span>

#include "config/Config.hpp"

class AudioPipeline {
public:
    auto write(std::span<const int16_t> frames) -> bool;
    auto read(std::span<int16_t> frames) -> size_t;  // called from audio task on core 0
    void setGain(float gain) { this->gain_.store(gain, std::memory_order_release); };
    void flush() { this->flush_requested_.store(true, std::memory_order_release); };
    auto available() const -> size_t { return this->availableSamples_() / CHANNELS; };
    auto dropped() const -> uint32_t { return dropped_.load(std::memory_order_relaxed); }

private:
    std::atomic<float> gain_{1.0f};
    std::atomic<size_t> head_{0};
    std::atomic<size_t> tail_{0};
    std::atomic<bool> flush_requested_{false};
    std::array<int16_t, AUDIO_BUFFER_SIZE> audio_buffer_;
    std::atomic<uint32_t> dropped_{0};

    auto availableSamples_() const -> size_t;
};