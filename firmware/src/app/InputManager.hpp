#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "audio/AudioPipeline.hpp"
#include "audio/IAudioInput.hpp"

class InputManager {
public:
    void registerInput(std::unique_ptr<IAudioInput> input);  // adding a source = one line here
    auto select(AudioSourceType type) -> bool;               // ends current, begins new
    void selectNext();  // button cycles through registered inputs
    auto active() -> IAudioInput*;
    void attachSink(AudioPipeline* pipeline) { this->sink_ = pipeline; };

private:
    uint8_t selected_input_idx_{0};
    std::vector<std::unique_ptr<IAudioInput>> inputs_;
    AudioPipeline* sink_{nullptr};

    void prepareInput_(uint8_t next_input);
};