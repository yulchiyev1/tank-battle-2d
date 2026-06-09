#pragma once
#include "GameState.h"
#include "TextObject.h"

class MainMenu : public GameState 
{
private:
    TextObject* titleText = nullptr;
    TextObject* pressStartText = nullptr;

public:
    virtual void Load(const EngineContext& engineContext) override;
    virtual void Init(const EngineContext& engineContext) override;
    virtual void LateInit(const EngineContext& engineContext) override;
    virtual void Update(float dt, const EngineContext& engineContext) override;
    virtual void LateUpdate(float dt, const EngineContext& engineContext) override;
    virtual void Draw(const EngineContext& engineContext) override;
    virtual void PostProcessing(const EngineContext& engineContext) override;
    virtual void Free(const EngineContext& engineContext) override;
    virtual void Unload(const EngineContext& engineContext) override;
};