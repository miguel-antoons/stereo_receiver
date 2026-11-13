#pragma once

#include <cstdint>

#define PRODUCT_NAME "BoomBox"

constexpr uint32_t SAMPLE_RATE_44100 = 44100;
constexpr uint8_t CHANNELS = 2;          // stereo channels
constexpr uint8_t BITS_PER_SAMPLE = 16;  // S16 PCM
constexpr uint16_t AUDIO_TASK_STACK_SIZE = 8192;
constexpr uint8_t AUDIO_TASK_PRIORITY = 5;
constexpr uint16_t UI_TASK_STACK_SIZE = 4096;
constexpr uint8_t UI_TASK_PRIORITY = 2;

constexpr uint8_t DELAY_MS_M = 10;

constexpr float INITAL_GAIN = 0.8F;

constexpr uint16_t DEFAULT_DMA_DESC = 6;     // number of DMA buffers
constexpr uint16_t DEFAULT_DMA_FRAME = 240;  // frames per DMA buffer
