#include "audio/outputs/I2sStreamOutput.hpp"

#include "config/Pins.hpp"

auto I2sStreamOutput::begin() -> bool {
    auto cfg = this->i2s_.defaultConfig(audio_tools::TX_MODE);
    cfg.pin_bck = PIN_I2S_BCLK;
    cfg.pin_ws = PIN_I2S_LRCK;
    cfg.pin_data = PIN_I2S_DOUT;
    cfg.sample_rate = SAMPLE_RATE_44100;
    cfg.channels = CHANNELS;
    cfg.bits_per_sample = BITS_PER_SAMPLE;
    cfg.buffer_count = DEFAULT_DMA_DESC;
    cfg.buffer_size = DEFAULT_DMA_FRAME;
    return this->i2s_.begin(cfg);
}
