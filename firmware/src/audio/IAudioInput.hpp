#pragma once


class AudioPipeline;

enum class AudioSourceType { Bluetooth, Wifi, Optical, Jack, Usb };

class IAudioInput {
public:
    virtual ~IAudioInput() = default;
    virtual bool begin() = 0;                 // start hardware/BT stack
    virtual void end() = 0;                   // stop + release
    virtual AudioSourceType type() const = 0;
    virtual const char* name() const = 0;
    virtual bool isActive() const = 0;        // e.g. BT peer connected
    // data callback target set by InputManager:
    void attachSink(AudioPipeline* pipeline);
};