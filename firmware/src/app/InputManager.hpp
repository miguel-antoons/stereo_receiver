#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "audio/IAudioInput.hpp"

class InputManager {
public:
    void registerInput(std::unique_ptr<IAudioInput> input);  // adding a source = one line here
    void setOnInputChanged(std::function<void(IAudioInput&)> callback);
    auto select(AudioSourceType type) -> bool;  // ends current, begins + notifies new
    void selectNext();                          // button cycles through registered inputs
    auto active() -> IAudioInput*;

private:
    uint8_t selected_input_idx_{0};
    std::vector<std::unique_ptr<IAudioInput>> inputs_;
    std::function<void(IAudioInput&)> on_input_changed_;

    void prepareInput_(uint8_t next_input);
};
