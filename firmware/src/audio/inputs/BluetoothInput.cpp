#include "audio/inputs/BluetoothInput.hpp"

#include "config/Config.hpp"

auto BluetoothInput::begin() -> bool {
    auto cfg = this->a2dp_.defaultConfig(audio_tools::RX_MODE);
    cfg.name = PRODUCT_NAME;
    cfg.wait_for_connection = false;
    this->running_ = this->a2dp_.begin(cfg);
    return this->running_;
}

void BluetoothInput::end() {
    this->a2dp_.end();
    this->running_ = false;
}
