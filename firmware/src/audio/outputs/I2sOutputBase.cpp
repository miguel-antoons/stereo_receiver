#include "audio/outputs/I2sOutputBase.hpp"

#include <esp_err.h>

#include "config/Config.hpp"

void I2sOutputBase::configureI2s(
    gpio_num_t bclk,  // NOLINT(bugprone-easily-swappable-parameters)
    gpio_num_t lrck,
    gpio_num_t dout,
    uint32_t rate
) {
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    chan_cfg.dma_desc_num = DEFAULT_DMA_DESC;
    chan_cfg.dma_frame_num = DEFAULT_DMA_FRAME;
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &this->i2s_handle_, nullptr));

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(rate),
        .slot_cfg =
            I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = bclk,
            .ws = lrck,
            .dout = dout,
            .din = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(this->i2s_handle_, &std_cfg));

    ESP_ERROR_CHECK(i2s_channel_enable(this->i2s_handle_));
}