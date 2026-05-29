#pragma once

#include"Gameobject.h"

class Projectile : public GameObject
{
public:
    void Init([[maybe_unused]] const EngineContext& engineContext) override;
    void LateInit([[maybe_unused]] const EngineContext& engineContext) override;
    void Update([[maybe_unused]] float dt, [[maybe_unused]] const EngineContext& engineContext) override;
    void Draw([[maybe_unused]] const EngineContext& engineContext) override;
    void Free([[maybe_unused]] const EngineContext& engineContext) override;
    void LateFree([[maybe_unused]] const EngineContext& engineContext) override;
    virtual void OnCollision(Object* other, const EngineContext& engineContext) override;
private:
};