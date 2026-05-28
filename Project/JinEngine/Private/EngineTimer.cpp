#include "EngineTimer.h"

#include "EngineContext.h"
#include "glfw3.h"

void EngineTimer::Start()
{
    lastTime = static_cast<float>(glfwGetTime());
    fpsTimer = 0.0f;
    frameCount = 0;
}

float EngineTimer::Tick(const EngineContext& engineContext)
{
    float now = static_cast<float>(glfwGetTime());
    float dt = now - lastTime;
    lastTime = now;
    if (dt > 0.05f)
    {
        engineContext.inputManager->Reset();
        dt = 0.0f;
    }
    fpsTimer += dt;
    frameCount++;
    return dt;
}

bool EngineTimer::ShouldUpdateFPS(float& outFPS)
{
    if (fpsTimer >= 0.3f)
    {
        outFPS = frameCount / fpsTimer;
        fpsTimer = 0.0f;
        frameCount = 0;
        return true;
    }
    return false;
}
