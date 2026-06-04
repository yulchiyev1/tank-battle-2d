#include "Turret.h"
#include "Engine.h"
#include "Player.h"
#include "Projectile.h"

Turret::Turret(Player* player) : parentPlayer(player)
{
}

void Turret::Init(const EngineContext& engineContext)
{
    transform2D.SetScale(glm::vec2(90.f));
    SetMesh(engineContext, "[EngineMesh]default");

    
    if (parentPlayer->GetTag() == "[Object]Player1")
    {
        SetMaterial(engineContext, "[Material]TurretB");
    }
    else if (parentPlayer->GetTag() == "[Object]Player2")
    {
        SetMaterial(engineContext, "[Material]TurretR");
    }
    SetRenderLayer("[Layer]UI");

    ammo = 30; //////BULLET의 갯수
}

void Turret::LateInit(const EngineContext& engineContext)
{
    JIN_LOG("Turret LateInit Called");
}

void Turret::Update(float dt, const EngineContext& engineContext)
{
    if (parentPlayer != nullptr)
    {
        //Turret은 tank body에 따라가기
        transform2D.SetPosition(parentPlayer->GetTransform2D().GetPosition());

        // Rotation
        float rotationSpeed = 1.5f;
        float angle = transform2D.GetRotation();

        if (engineContext.inputManager->IsKeyDown(leftKey))
        {
            angle += rotationSpeed * dt;
        }
        if (engineContext.inputManager->IsKeyDown(rightKey))
        {
            angle -= rotationSpeed * dt;
        }

        transform2D.SetRotation(angle);

        //Aiming
        if (fireTimer > 0.0f)
        {
            fireTimer -= dt;
        }

        if (engineContext.inputManager->IsKeyPressed(shootKey))
        {
            JIN_LOG("shootkey: " << shootKey);
            if (ammo > 0 && fireTimer <= 0.0f)
            {
                Projectile* bullet = new Projectile();
                bullet->Init(engineContext);

                float offsetDistance = 50.0f; 
                float angle = transform2D.GetRotation();

                
                // ASOSIY YECHIM: Paydo bo'lish joyini ham 90 gradusga buramiz!
                float mathAngle = angle + glm::radians(90.0f);
              

                glm::vec2 turretPos = transform2D.GetPosition();

                glm::vec2 spawnPos;
        
                spawnPos.x = turretPos.x + (std::cos(mathAngle) * offsetDistance);
                spawnPos.y = turretPos.y + (std::sin(mathAngle) * offsetDistance);

                bullet->GetTransform2D().SetPosition(spawnPos);
                bullet->SetDirection(angle);

                engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(std::unique_ptr<Object>(bullet));

                ammo--;
                fireTimer = 0.15f;

                JIN_LOG("남은 총알 : " + std::to_string(ammo));
            }
        
        }
    }
}

void Turret::Draw(const EngineContext& engineContext)
{
}

void Turret::Free(const EngineContext& engineContext)
{
    JIN_LOG("Turret Free Called");
}

void Turret::LateFree(const EngineContext& engineContext)
{

}

void Turret::SetControls(int left, int right, int shoot)
{
    leftKey = left;
    rightKey = right;
    shootKey = shoot;
}

