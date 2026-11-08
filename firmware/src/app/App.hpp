#pragma once

#include <memory>
#include <vector>

#include "InputManager.hpp"
#include "audio/AudioPipeline.hpp"
#include "audio/IAudioOutput.hpp"
#include "controls/IControl.hpp"


class App {
public:
    ~App();
    void setup();   // construct pipeline, output, inputs, controls (see below)
    void run();     // create FreeRTOS tasks, then idle
    // facade used by controls:
    void nextInput(); void setVolume(float); void toggleMute();
    float volume() const; const char* activeInputName() const;
private:
    AudioPipeline pipeline_;
    InputManager inputs_;
    std::unique_ptr<IAudioOutput> output_;   // Pcm5102AOutput OR MA12070POutput
    std::vector<std::unique_ptr<IControl>> controls_;
};
