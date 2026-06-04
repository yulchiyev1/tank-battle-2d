#pragma once
#include "Engine.h"
#include "TextObject.h"

class Player;

class Turret : public GameObject
{
public:
    Turret(Player* player);
    virtual ~Turret() = default;

    virtual void Init(const EngineContext& engineContext) override;
    virtual void LateInit(const EngineContext& engineContext) override;
    virtual void Update(float dt, const EngineContext& engineContext) override;
    virtual void Draw(const EngineContext& engineContext) override;
    virtual void Free(const EngineContext& engineContext) override;
    virtual void LateFree(const EngineContext& engineContext) override;

    void SetControls(int left, int right, int shoot);

private:
    Player* parentPlayer;

    // Controls
    int leftKey, rightKey, shootKey;

    // Shooting 
    int ammo;
    float fireTimer = 0.0f;
    bool isShooting = false;

    // Animation
    std::shared_ptr<SpriteSheet> shootSpritesheet;
    float animTimer = 0.0f;

    // UI
    TextObject* ammoText = nullptr;
};