#include "Item.h"
#include "Engine.h"
#include "Player.h"
#include <random>

void Item::Init(const EngineContext& engineContext)
{
    SetMesh(engineContext, "[EngineMesh]default");
    SetMaterial(engineContext, "[Material]ItemBox");

    transform2D.SetScale(glm::vec2(30.f));

    SetCollider(std::make_unique<AABBCollider>(this, glm::vec2{ 30.f, 30.f }));
    GetCollider()->SetUseTransformScale(false);

    SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Item", { "[Object]Player1", "[Object]Player2", "[Object]Bullet" });

    // random selection
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 3);

    currentType = static_cast<ItemType>(dist(gen));

    SetRenderLayer("[Layer]Items");

    // original scale (for breath effect)
    baseScale = transform2D.GetScale(); 

    //auto crashed = engineContext.renderManager->GetSpriteSheetByTag("[SpriteSheet]Explosion");
    //crashed->AddClip("[Clip]Smoke", { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 }, 0.11f);
    //AttachAnimator(std::make_unique<SpriteAnimator>(crashed, 1.0f, false));
}

void Item::LateInit(const EngineContext& engineContext)
{
}

void Item::Update(float dt, const EngineContext& engineContext)
{
    // breath effect
    float maxScaleOffset = 8.0f; 
    float speedMultiplier = 15.0f; 

    if (isGrowing)
    {
        currentScaleOffset += breathSpeed * speedMultiplier * dt;
        if (currentScaleOffset >= maxScaleOffset)
        {
            currentScaleOffset = maxScaleOffset;
            isGrowing = false;
        }
    }
    else
    {
        currentScaleOffset -= breathSpeed * speedMultiplier * dt;
        if (currentScaleOffset <= 0.0f)
        {
            currentScaleOffset = 0.0f;
            isGrowing = true;
        }
    }
    GetTransform2D().SetScale(baseScale + glm::vec2(currentScaleOffset, currentScaleOffset));
}

void Item::Draw(const EngineContext& engineContext)
{
}

void Item::Free(const EngineContext& engineContext)
{
}

void Item::LateFree(const EngineContext& engineContext)
{
}

void Item::OnCollision(Object* other, const EngineContext& engineContext)
{
    if (other->GetTag() == "[Object]Bullet")
    {
        if (!isUnlocked)
        {
            engineContext.soundManager->Play("[Sound]BoxCrash", 1.0f, 0.0f); 
            isUnlocked = true;
            other->Kill();

            
            //GetSpriteAnimator()->PlayClip("[Clip]Smoke");


            if (currentType == ItemType::HEALTH)
                SetMaterial(engineContext, "[Material]ItemHealth");
            else if (currentType == ItemType::AMMO)
                SetMaterial(engineContext, "[Material]ItemAmmo");
            else if (currentType == ItemType::SIZE)
                SetMaterial(engineContext, "[Material]ItemSize");
            else if (currentType == ItemType::FLASH)
                SetMaterial(engineContext, "[Material]Item2X");
        }
    }
    else if (other->GetTag() == "[Object]Player1" || other->GetTag() == "[Object]Player2")
    {
        if (isUnlocked)
        {
            Player* hitPlayer = dynamic_cast<Player*>(other);
            engineContext.soundManager->Play("[Sound]ItemEffect", 0.2f, 0.0f); //sound
            if (hitPlayer != nullptr)
            {
                if (currentType == ItemType::HEALTH)
                {
                    JIN_LOG("(HEALTH POINTS +++)");
                    hitPlayer->AddHealth(10); 
                }
                else if (currentType == ItemType::AMMO)
                {
                    JIN_LOG("(5 AMMO ++++)");
                    hitPlayer->AddAmmo(5);  
                }
                else if (currentType == ItemType::SIZE)
                {
                    JIN_LOG("(2.5x BULLET SIZE)");
                    hitPlayer->IncreaseBulletSize();
                }
                else if (currentType == ItemType::FLASH)
                {
                    JIN_LOG("(FLASH 2X FASTER)");
                    hitPlayer->IncreaseSpeed();
                }
            }

            Kill();
        }
    }
}
