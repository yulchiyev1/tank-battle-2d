#include "Projectile.h"
#include "EngineContext.h"
#include "GameState.h"
#include "Player.h"
#include "Item.h"   

void Projectile::Init(const EngineContext& engineContext)
{
    SetMesh(engineContext, "[EngineMesh]default");
    SetMaterial(engineContext, "[Material]Bullet");

    // bullet collision 
    if (isSuperBig)
    {
        // if big bullet item on -> bigger bullet
        transform2D.SetScale(glm::vec2(40.f));
        SetCollider(std::make_unique<CircleCollider>(this, 40.f ));
        lifeTimer = 6.0f;
    }
    else
    {
        transform2D.SetScale(glm::vec2(15.f));
        SetCollider(std::make_unique<CircleCollider>(this, 15.f));
        lifeTimer = 2.0f;
    }

    GetCollider()->SetUseTransformScale(false);

    //SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Bullet", { "[Object]Wall", "[Object]Border_H", "[Object]Border_V", "[Object]Player1", "[Object]Player2", "[Object]Item" });
    SetRenderLayer("[Layer]Projectile");

    speed = 600.0f;
    
}

void Projectile::LateInit(const EngineContext& engineContext)
{

}

void Projectile::Update(float dt, const EngineContext& engineContext)
{
    // 2x bullet size
    if (isSuperBig)
    {
        transform2D.SetScale(glm::vec2(40.0f)); // O'qni doimiy katta ushlab turamiz
    }

    lifeTime += dt;

    // Tanaffus vaqtini kamaytirib boramiz
    if (wallCooldown > 0.0f) {
        wallCooldown -= dt;
    }

    // oldPos for reflection
    oldPos = transform2D.GetPosition();

    // timer for bullet
    lifeTimer -= dt;
    if (lifeTimer <= 0.0f)
    {
        Kill();
    }

    // bullet moving
    glm::vec2 currentPos = transform2D.GetPosition();
    currentPos.x += velocityX * dt;
    currentPos.y += velocityY * dt;
    transform2D.SetPosition(currentPos);
}

void Projectile::Draw(const EngineContext& engineContext)
{

}

void Projectile::Free(const EngineContext& engineContext)
{

}

void Projectile::LateFree(const EngineContext& engineContext)
{

}

void Projectile::OnCollision(Object* other, const EngineContext& engineContext)
{
    JIN_LOG("Collision : " + other->GetTag());

    // tank collision
    if (other->GetTag() == "[Object]Player1" || other->GetTag() == "[Object]Player2")
    {
        Player* hitPlayer = dynamic_cast<Player*>(other);

        if (hitPlayer != nullptr)
        {
            hitPlayer->TakeDamage(8);
        }
        Kill();
    }
    // item collision
    else if (other->GetTag() == "[Object]Item")
    {
        Item* hitItem = dynamic_cast<Item*>(other);
        if (hitItem != nullptr && !hitItem->IsUnlocked())
        {
            Kill();
        }
    }
    // top/bottom border collision
    else if (other->GetTag() == "[Object]Border_H")
    {
        transform2D.SetPosition(oldPos);

        glm::vec2 bulletPos = transform2D.GetPosition();
        glm::vec2 wallPos = other->GetTransform2D().GetPosition();
        float dy = bulletPos.y - wallPos.y;

        // Matematik himoya: Faqat o'q devorga qarab ketayotgan bo'lsa buramiz (yopishib qolmasligi uchun)
        if (dy * velocityY < 0) {
            velocityY = -velocityY;
        }
    }
    // left/right border collision
    else if (other->GetTag() == "[Object]Border_V")
    {
        transform2D.SetPosition(oldPos);

        glm::vec2 bulletPos = transform2D.GetPosition();
        glm::vec2 wallPos = other->GetTransform2D().GetPosition();
        float dx = bulletPos.x - wallPos.x;

        // Faqat o'q devorga qarab ketayotgan bo'lsa buramiz
        if (dx * velocityX < 0) {
            velocityX = -velocityX;
        }
    }
    // 경기장 안 random wall blocks
    else if (other->GetTag() == "[Object]Wall")
    {
        if (wallCooldown > 0.0f) return;

        if (lifeTime < 0.0001f)
        {
            Kill();
            return;
        }

        transform2D.SetPosition(oldPos);

        glm::vec2 bulletPos = transform2D.GetPosition();
        glm::vec2 wallPos = other->GetTransform2D().GetPosition();

        float dx = std::abs(bulletPos.x - wallPos.x);
        float dy = std::abs(bulletPos.y - wallPos.y);

        if (dx > dy)
        {
            velocityX = -velocityX;
        }
        else if (dy > dx)
        {
            velocityY = -velocityY;
        }
        else
        {
            velocityX = -velocityX;
            velocityY = -velocityY;
        }
        wallCooldown = 0.05f;
    }
}

void Projectile::SetDirection(float startAngle)
{
    transform2D.SetRotation(startAngle);
    float mathAngle = startAngle + glm::radians(90.0f);

    float actualSpeed = speed * speedMultiplier;

    velocityX = glm::cos(mathAngle) * actualSpeed;
    velocityY = glm::sin(mathAngle) * actualSpeed;
}


void Projectile::SetSpawnPosition(glm::vec2 pos)
{
    transform2D.SetPosition(pos);
    oldPos = pos;
}

void Projectile::SetSpeedMultiplier(float mult)
{
    speedMultiplier = mult;
}

void Projectile::MakeBig()
{
    isSuperBig = true; 
}