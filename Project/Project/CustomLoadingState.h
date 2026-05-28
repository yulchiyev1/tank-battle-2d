#pragma once
#include <functional>
#include "Engine.h"
#include "AsyncResourceLoader.h"

class CustomLoadingState : public LoadingState
{
public:
    explicit CustomLoadingState(std::function<std::unique_ptr<GameState>()> nextFactory);

    void Load(const EngineContext& engineContext) override;

    void Init(const EngineContext& engineContext) override;

    void LateInit(const EngineContext& engineContext) override;

    void Update(float dt, const EngineContext& engineContext) override;

    void LateUpdate(float dt, const EngineContext& engineContext) override;

    void Draw(const EngineContext& engineContext) override;

    void Free(const EngineContext& engineContext) override;

    void Unload(const EngineContext& engineContext) override;
};
