#pragma once

#include <memory>

#include "audio/IAudioInput.hpp"

class InputManager {
public:
    void registerInput(std::unique_ptr<IAudioInput> input);  // adding a source = one line here
    auto select(AudioSourceType type) -> bool;               // ends current, begins new
    void selectNext();  // button cycles through registered inputs
    auto active() -> IAudioInput*;
};