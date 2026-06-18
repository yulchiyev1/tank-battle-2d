#pragma once
#include "Engine.h"

//forward declarations
class Turret;
class Portal;

//teleportation states
enum class TeleportState
{
    NONE,
    ENTERING,
    EXITING
};

class Player : public GameObject
{
public:
    //core Engine Functions
    void Init(const EngineContext& engineContext) override;
    void LateInit(const EngineContext& engineContext) override;
    void Update(float dt, const EngineContext& engineContext) override;
    void Draw(const EngineContext& engineContext) override;
    void Free(const EngineContext& engineContext) override;
    void LateFree(const EngineContext& engineContext) override;
    virtual void OnCollision(Object* other, const EngineContext& engineContext) override;

    //Player Status & Combat 
    int hp = 100;
    void TakeDamage(int damageAmount);

    //Items & Power-ups
    void AddHealth(int amount);
    void AddAmmo(int amount);
    void IncreaseBulletSize();
    void IncreaseSpeed();
    float GetSpeedMultiplier() const { return speedMultiplier; }

    //Movement & Controls
    int upKey, downKey, leftKey, rightKey;
    void SetControls(int up, int down, int left, int right);

    //Teleportation System
    TeleportState tpState = TeleportState::NONE;
    float tpTimer = 0.0f;
    float tpDuration = 0.35f;   // Duration of enter/exit 
    glm::vec2 tpStartPos;       // Starting point of absorption
    glm::vec2 tpCenter;         // Center of entry portal
    glm::vec2 tpDest;           // Center of exit portal

    Portal* inPortal = nullptr; // Reference to entry portal
    Portal* outPortal = nullptr;// Reference to exit portal

    void PushAwayFrom(glm::vec2 epicenter, float safeDistance); // Smooth push effect

private:
    //Internal State
    glm::vec2 oldPos;           // Previous position for collision resolution
    float flashTimer = 0.0f;    // Speed boost duration timer
    float speedMultiplier = 1.0f;

    //Graphics & UI
    std::shared_ptr<SpriteSheet> moveSpritesheetB;
    std::shared_ptr<SpriteSheet> moveSpritesheetR;
    TextObject* hpText;

    //Components
    Turret* myTurret = nullptr;
};