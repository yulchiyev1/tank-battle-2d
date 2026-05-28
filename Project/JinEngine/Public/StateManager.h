#pragma once
#include <memory>

class GameState;
class JinEngine;
struct EngineContext;

class StateManager
{
    friend JinEngine;
    friend WindowManager;
public:
    [[nodiscard]] GameState* GetCurrentState() const;

    void ChangeState(std::unique_ptr<GameState> newState);

private:

    void Update(float dt, const EngineContext& engineContext);

    void Draw(const EngineContext& engineContext);

    void Free(const EngineContext& engineContext);

    std::unique_ptr<GameState> currentState;
    std::unique_ptr<GameState> nextState;
};
