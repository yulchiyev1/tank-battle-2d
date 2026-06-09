#pragma once
#include "GameState.h"
#include "TextObject.h"
#include <string>

class GameOver : public GameState
{
public:
    GameOver(std::string winner) : winnerName(winner) {}

    virtual void Init(const EngineContext& engineContext) override;
    virtual void Update(float dt, const EngineContext& engineContext) override;
    virtual void Draw(const EngineContext& engineContext) override;

private:
    std::string winnerName;
    TextObject* resultText = nullptr;

};