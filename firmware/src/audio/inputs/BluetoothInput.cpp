#include "audio/inputs/BluetoothInput.hpp"

#include <algorithm>
#include <cstdint>
#include <span>

#include "config/Config.hpp"

namespace {
AudioPipeline* a2dp_pipeline = nullptr;
BluetoothInput* a2dp_input = nullptr;

void a2dp_stream(const uint8_t* data, uint32_t len) {
    if (a2dp_pipeline != nullptr) {
        a2dp_pipeline->write(
            std::span<const int16_t>{reinterpret_cast<const int16_t*>(data), len / sizeof(int16_t)}
        );
    }
}

void a2dp_volume_changed(int volume) {
    if (a2dp_input != nullptr) {
        a2dp_input->dispatchRemoteVolume(volume);
    }
}
}  // namespace

auto BluetoothInput::begin() -> bool {
    if (this->sink_ == nullptr) {
        return false;
    }
    a2dp_pipeline = this->sink_;
    a2dp_input = this;
    this->a2dp_sink_.set_volume_control(&this->volume_control_);
    this->a2dp_sink_.set_avrc_rn_volumechange(a2dp_volume_changed);
    this->a2dp_sink_.set_stream_reader(a2dp_stream, false);
    this->a2dp_sink_.start("BoomBox");
    this->running_ = true;
    return this->running_;
}

void BluetoothInput::end() {
    this->a2dp_sink_.stop();
    a2dp_input = nullptr;
    a2dp_pipeline = nullptr;
    this->running_ = false;
}

void BluetoothInput::setRemoteVolume(float v01) {
    if (!this->a2dp_sink_.is_connected()) {
        return;
    }
    const float clamped = std::clamp(v01, 0.0F, 1.0F);
    this->a2dp_sink_.set_volume(static_cast<uint8_t>(clamped * AVRCP_VOLUME_MAX + 0.5F));
}

void BluetoothInput::dispatchRemoteVolume(int volume) {
    if (this->remote_volume_callback_) {
        this->remote_volume_callback_(
            static_cast<float>(volume) / static_cast<float>(AVRCP_VOLUME_MAX)
        );
    }
}