#pragma once
#include "GameState.h"
#include "TextObject.h"
#include <string>

class GameOver : public GameState
{
public:
    GameOver(int winner) : winnerID(winner) {}
    virtual void Load(const EngineContext& engineContext) override;
    virtual void Init(const EngineContext& engineContext) override;
    virtual void Update(float dt, const EngineContext& engineContext) override;
    virtual void Draw(const EngineContext& engineContext) override;

private:
    int winnerID;

    // tanks
    GameObject* winnerTank = nullptr;
    GameObject* loserTank = nullptr;

    // buttons
    GameObject* btnReplay = nullptr;
    GameObject* btnExit = nullptr;

    // Mouse cursor
    GameObject* cursor = nullptr;

    // replay btn
    glm::vec2 btnReplayBaseScale;

    // exit btn
    glm::vec2 btnExitBaseScale;
};