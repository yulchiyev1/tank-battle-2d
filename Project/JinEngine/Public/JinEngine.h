#pragma once
#include "EngineContext.h"

class JinEngine
{
public:
    JinEngine() = default;

    [[nodiscard]] bool Init(int windowWidth, int windowHeight);

    void Run();

    void RequestQuit();

    [[nodiscard]] EngineContext& GetEngineContext() { return engineContext; }

    void RenderDebugDraws(bool shouldShow) { showDebugDraw = shouldShow; }

    [[nodiscard]] bool ShouldRenderDebugDraws() const { return showDebugDraw; }
private:
    void Free() const;

    void SetEngineContext();

    EngineContext engineContext;
    StateManager stateManager;
    WindowManager windowManager;
    InputManager inputManager;
    RenderManager renderManager;
    SoundManager soundManager;
    bool shouldRun = true;
    bool showDebugDraw = false;
};
