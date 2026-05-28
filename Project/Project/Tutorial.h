#pragma once
#include"GameState.h"
#include"Player.h"
namespace TutorialState
{
    void AsyncLoad(const EngineContext& engineContext, LoadingState* loading);
}
class Tutorial :public GameState
{
public:
    void Load(const EngineContext& engineContext) override;

    void Init(const EngineContext& engineContext) override;

    void LateInit(const EngineContext& engineContext) override;

    void Update(float dt, const EngineContext& engineContext) override;

    void LateUpdate(float dt, const EngineContext& engineContext) override;

    void Draw(const EngineContext& engineContext) override;

    void PostProcessing(const EngineContext& engineContext) override;

    void Free(const EngineContext& engineContext) override;

    void Unload(const EngineContext& engineContext) override;

private:
    Player* player1;
    Player* player2;
    GameObject* background;
};


