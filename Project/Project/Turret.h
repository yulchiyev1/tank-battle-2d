#pragma once

#include "Engine.h"

class Player;

class Turret : public GameObject 
{

public:
    virtual void Init(const EngineContext& engineContext) override;
    virtual void LateInit(const EngineContext& engineContext) override;
    virtual void Update(float dt, const EngineContext& engineContext) override;
    virtual void Draw(const EngineContext& engineContext) override;
    virtual void Free(const EngineContext& engineContext) override;
    virtual void LateFree(const EngineContext& engineContext) override;

    Turret(Player* player);
    virtual ~Turret() = default;

    //turret rotation keys:
    int leftKey, rightKey, shootKey;
    int ammo;
    void SetControls(int left, int right, int shoot);

private:
    Player* parentPlayer;

    float fireTimer = 0.0f; //Qayta o'qlash taymeri
};