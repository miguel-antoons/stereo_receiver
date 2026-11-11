#include "app/InputManager.hpp"

#include <esp32-hal-log.h>

#include <cstddef>
#include <cstdint>

#include "audio/IAudioInput.hpp"

void InputManager::registerInput(std::unique_ptr<IAudioInput> input) {
    this->inputs_.push_back(std::move(input));
}

auto InputManager::select(AudioSourceType type) -> bool {
    for (size_t i = 0; i < this->inputs_.size(); i++) {
        if (this->inputs_[i]->type() == type) {
            this->prepareInput_(i);
            return true;
        }
    }
    return false;
}

void InputManager::selectNext() {
    if (this->inputs_.empty()) {
        log_i("No inputs to select, please register one first.");
        return;
    }
    uint8_t next_input = (this->selected_input_idx_ + 1) % this->inputs_.size();
    this->prepareInput_(next_input);
}

auto InputManager::active() -> IAudioInput* {
    if (this->inputs_.empty()) {
        log_i("No active input, because no inputs are registered.");
        return nullptr;
    }
    return this->inputs_[this->selected_input_idx_].get();
}

void InputManager::prepareInput_(uint8_t next_input) {
    if (this->inputs_[this->selected_input_idx_]->isActive()) {
        this->inputs_[this->selected_input_idx_]->end();

        if (this->sink_ != nullptr) {
            this->sink_->flush();
        }
    }
    this->selected_input_idx_ = next_input;
    this->inputs_[next_input]->attachSink(this->sink_);
    this->inputs_[next_input]->begin();
}
