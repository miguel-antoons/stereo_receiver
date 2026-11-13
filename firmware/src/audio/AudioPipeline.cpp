#include "audio/AudioPipeline.hpp"

#include <esp32-hal-log.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

#include "config/Config.hpp"

auto AudioPipeline::write(std::span<const int16_t> frames) -> bool {
    size_t samples_count = frames.size();
    if (AUDIO_BUFFER_SIZE - this->availableSamples_() <= samples_count) {
        this->dropped_.fetch_add(1, std::memory_order_relaxed);
        uint32_t drops = this->dropped_.load(std::memory_order_relaxed);
        if (drops % INTERVAL_DROP_MSG == 0) {
            log_d("Total A2DP packet drops: %u", drops);
        }
        return false;
    }

    const size_t head = this->head_.load(std::memory_order_relaxed);
    const size_t first = std::min(samples_count, AUDIO_BUFFER_SIZE - head);

    memcpy(&this->audio_buffer_[head], frames.data(), first * sizeof(int16_t));
    if (samples_count > first) {
        memcpy(
            &this->audio_buffer_[0], frames.data() + first,
            (samples_count - first) * sizeof(int16_t)
        );
    }

    this->head_.store((head + samples_count) % AUDIO_BUFFER_SIZE, std::memory_order_release);
    return true;
}

auto AudioPipeline::read(std::span<int16_t> frames) -> size_t {
    const size_t samples_count = frames.size();
    if (this->flush_requested_.exchange(false, std::memory_order_acquire)) {
        this->tail_.store(this->head_.load(std::memory_order_acquire), std::memory_order_release);
    }

    const size_t available_samples = this->availableSamples_();
    if (available_samples == 0) {
        return 0;
    }

    size_t to_read = std::min(samples_count, available_samples);
    // avoid odd reads
    to_read -= to_read % CHANNELS;
    if (!to_read) {
        return 0;
    }

    const size_t tail = this->tail_.load(std::memory_order_relaxed);
    const size_t first = std::min(to_read, AUDIO_BUFFER_SIZE - tail);

    memcpy(frames.data(), &this->audio_buffer_[tail], first * sizeof(int16_t));
    if (to_read > first) {
        memcpy(frames.data() + first, &this->audio_buffer_[0], (to_read - first) * sizeof(int16_t));
    }

    const float gain = this->gain_.load(std::memory_order_acquire);
    if (gain != 1.0F) {
        for (size_t i = 0; i < to_read; i++) {
            const float scaled = static_cast<float>(frames.data()[i]) * gain;
            frames.data()[i] = static_cast<int16_t>(std::clamp(scaled, -32768.0F, 32767.0F));
        }
    }

    this->tail_.store((tail + to_read) % AUDIO_BUFFER_SIZE, std::memory_order_release);
    return to_read / CHANNELS;
}

auto AudioPipeline::availableSamples_() const -> size_t {
    const size_t head = this->head_.load(std::memory_order_acquire);
    const size_t tail = this->tail_.load(std::memory_order_acquire);
    return head < tail ? AUDIO_BUFFER_SIZE - tail + head : head - tail;
}