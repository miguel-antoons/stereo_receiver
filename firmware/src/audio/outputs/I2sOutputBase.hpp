#pragma once

#include <driver/gpio.h>
#include <driver/i2s_std.h>

#include "audio/IAudioOutput.hpp"

class I2sOutputBase : public IAudioOutput {
protected:
    void configureI2s(gpio_num_t bclk, gpio_num_t lrck, gpio_num_t dout, uint32_t rate);
    i2s_chan_handle_t i2s_handle_;
};