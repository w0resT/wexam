#pragma once

class IImGuiManager {
public:
    virtual void Init() = 0;
    virtual void Shutdown() = 0;
    virtual void NewFrame() = 0;
    virtual void Render() = 0;

    virtual ~IImGuiManager() {}
};