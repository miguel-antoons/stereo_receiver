#pragma once

#include <memory>
#include <vector>

#include "AudioTools.h"
#include "InputManager.hpp"
#include "audio/IAudioOutput.hpp"
#include "controls/IControl.hpp"

class App {
public:
    void setup();
    void run();
    void setVolume(float volume);
    void toggleMute();
    auto volume() -> float;

private:
    std::unique_ptr<IAudioOutput> output_;  // I2sStreamOutput (any I2S DAC)
    audio_tools::VolumeStream volume_;      // software volume, output = output_ stream
    audio_tools::StreamCopy copier_;        // pulls from the active input's stream
    InputManager inputs_;
    std::vector<std::unique_ptr<IControl>> controls_;
    bool muted_{false};
    float volume_before_mute_{0.0F};

    static void audioTask_(void* app_ptr);
    static void uiTask_(void* app_ptr);
};
