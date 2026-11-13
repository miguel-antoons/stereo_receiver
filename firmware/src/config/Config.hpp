#pragma once

#include <cstddef>
#include <cstdint>

constexpr uint32_t SAMPLE_RATE_44100 = 44100;
constexpr size_t AUDIO_BUFFER_FRAMES = SAMPLE_RATE_44100 / 5;  // 200 ms
constexpr uint8_t CHANNELS = 2;                                // stereo channels
constexpr size_t AUDIO_BUFFER_SIZE = AUDIO_BUFFER_FRAMES * CHANNELS;
constexpr uint8_t INTERVAL_DROP_MSG = 10;
constexpr uint16_t AUDIO_TASK_STACK_SIZE = 8192;
constexpr uint8_t AUDIO_TASK_PRIORITY = 5;
constexpr uint16_t UI_TASK_STACK_SIZE = 4096;
constexpr uint8_t UI_TASK_PRIORITY = 2;

constexpr uint8_t DELAY_MS_M = 10;
constexpr uint16_t AUDIO_TASK_BUFFER_FRAMES = 256;
constexpr uint16_t AUDIO_TASK_BUFFER_SIZE = AUDIO_TASK_BUFFER_FRAMES * CHANNELS;

constexpr float INITAL_GAIN = 0.8F;
constexpr uint8_t AVRCP_VOLUME_MAX = 127;  // AVRCP absolute volume range is 0..127

constexpr uint16_t DEFAULT_DMA_DESC = 6;
constexpr uint16_t DEFAULT_DMA_FRAME = 240;