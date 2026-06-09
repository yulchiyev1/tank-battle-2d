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
}

void Item::LateInit(const EngineContext& engineContext)
{
}

void Item::Update(float dt, const EngineContext& engineContext)
{
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
            isUnlocked = true;
            other->Kill();

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
                    hitPlayer->AddAmmo(6);  
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
