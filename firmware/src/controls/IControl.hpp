#pragma once

class App;

class IControl {
public:
    virtual ~IControl() = default;
    virtual void begin(App& app) = 0;  // store App facade, attach callbacks
    virtual void poll() = 0;           // non-blocking, called from UI task loop
};