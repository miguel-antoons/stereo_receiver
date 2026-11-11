#include "audio/inputs/BluetoothInput.hpp"

#include <cstdint>
#include <span>

#include "config/Config.hpp"

namespace {
AudioPipeline* a2dp_pipeline = nullptr;

void a2dp_stream(const uint8_t* data, uint32_t len) {
    if (a2dp_pipeline != nullptr) {
        a2dp_pipeline->write(
            std::span<const int16_t>{reinterpret_cast<const int16_t*>(data), len / sizeof(int16_t)}
        );
    }
}
}  // namespace

auto BluetoothInput::begin() -> bool {
    if (this->sink_ == nullptr) {
        return false;
    }
    a2dp_pipeline = this->sink_;
    this->a2dp_sink_.set_stream_reader(a2dp_stream, false);
    this->a2dp_sink_.start("BoomBox");
    this->running_ = true;
    return this->running_;
}

void BluetoothInput::end() {
    this->a2dp_sink_.stop();
    a2dp_pipeline = nullptr;
    this->running_ = false;
}