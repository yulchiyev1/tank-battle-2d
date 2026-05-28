#pragma once
#include "Object.h"


class GameObject : public Object
{
public:
    GameObject() : Object(ObjectType::GAME) {}
    ~GameObject() override = default;

    void Init([[maybe_unused]] const EngineContext& engineContext) override {}

    void LateInit([[maybe_unused]] const EngineContext& engineContext) override {}

    void Update([[maybe_unused]] float dt, [[maybe_unused]] const EngineContext& engineContext) override {}

    void Draw([[maybe_unused]] const EngineContext& engineContext) override {}

    void Free([[maybe_unused]] const EngineContext& engineContext) override {}

    void LateFree([[maybe_unused]] const EngineContext& engineContext) override {}
};
