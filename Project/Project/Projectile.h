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

    //총알의 direction
    void SetDirection(float startAngle);

    //preventing bullet error (devorga taqalgandagi)
    void SetSpawnPosition(glm::vec2 pos);

    void SetSpeedMultiplier(float mult);

    void MakeBig();

    bool isExploding = false;
    float explodeTimer = 0.5f; // Animatsiya 0.5 soniya davom etadi
private:
    float speed;
    float lifeTimer; //몇초후 bullet 죽이기를 위하여
    float velocityX;
    float velocityY;

    //bullet reflection
    glm::vec2 oldPos;
    float lifeTime = 0.0f; 
    float wallCooldown = 0.0f;

    float speedMultiplier = 1.0f;
    bool isSuperBig = false;
};