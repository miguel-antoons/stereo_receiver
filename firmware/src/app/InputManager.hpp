#pragma once

#include <memory>

#include "audio/IAudioInput.hpp"


class InputManager {
public:
    void registerInput(std::unique_ptr<IAudioInput> input); // adding a source = one line here
    bool select(AudioSourceType type);   // ends current, begins new
    void selectNext();                   // button cycles through registered inputs
    IAudioInput* active();
};