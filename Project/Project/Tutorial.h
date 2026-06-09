#pragma once
#include "GameState.h"
#include "Player.h"
#include "TextObject.h" // Yozuv chiqarish uchun qo'shildi

namespace TutorialState
{
    void AsyncLoad(const EngineContext& engineContext, LoadingState* loading);
}

class Tutorial : public GameState
{
public:
    void Load(const EngineContext& engineContext) override;
    void Init(const EngineContext& engineContext) override;
    void LateInit(const EngineContext& engineContext) override;
    void Update(float dt, const EngineContext& engineContext) override;
    void LateUpdate(float dt, const EngineContext& engineContext) override;
    void Draw(const EngineContext& engineContext) override;
    void Free(const EngineContext& engineContext) override;
    void Unload(const EngineContext& engineContext) override;

    float screenW;
    float screenH;

private:
    Player* player1;
    Player* player2;
    GameObject* background;
    
    // UI Obyektlari
    TextObject* timerTextObj = nullptr;

    void WallBlock(float x, float y, float size, const EngineContext& engineContext, bool isBorder = false);

    float itemSpawnTimer = 12.0f;
    float roundTimer = 5.0f; // 120 sekundlik raund
};