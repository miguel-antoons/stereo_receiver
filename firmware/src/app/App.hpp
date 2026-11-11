#pragma once

#include <memory>
#include <vector>

#include "InputManager.hpp"
#include "audio/AudioPipeline.hpp"
#include "audio/IAudioOutput.hpp"
#include "controls/IControl.hpp"

class App {
public:
    void setup();
    void run();
    void nextInput();
    void setVolume(float);
    void toggleMute();
    auto volume() const -> float;
    auto activeInputName() const -> char*;

private:
    AudioPipeline pipeline_;
    InputManager inputs_;
    std::unique_ptr<IAudioOutput> output_;  // Pcm5102AOutput OR MA12070POutput
    std::vector<std::unique_ptr<IControl>> controls_;

    static void audioTask_(void* app_ptr);
    static void uiTask_(void* app_ptr);
};
