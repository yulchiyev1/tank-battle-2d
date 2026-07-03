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
    float tpDuration = 0.35f;   
    glm::vec2 tpStartPos;       
    glm::vec2 tpCenter;        
    glm::vec2 tpDest;        

    Portal* inPortal = nullptr; // Reference to entry portal
    Portal* outPortal = nullptr;// Reference to exit portal

    void PushAwayFrom(glm::vec2 epicenter, float safeDistance); // Smooth push effect

private:
    //Internal State
    glm::vec2 oldPos;           
    float flashTimer = 0.0f;    
    float speedMultiplier = 1.0f;

    //Graphics & UI
    std::shared_ptr<SpriteSheet> moveSpritesheetB;
    std::shared_ptr<SpriteSheet> moveSpritesheetR;
    Turret* myTurret = nullptr;

    // HEALTH BAR
    GameObject* hpBg = nullptr;
    GameObject* hpFill = nullptr;
    float hpBaseWidth = 50.0f;
    float hpBaseHeight = 12.0f;

    // track tile
    std::vector<std::pair<GameObject*, float>> activeTracks;
    glm::vec2 lastTrackPos;
    float trackDistance = 8.0f; // track 생성 in every
    float trackLifeTime = 1.0f;  // track lifetime
    float trackRearOffset = 20.0f;
};