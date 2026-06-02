#include "Turret.h"
#include "Engine.h"
#include "Player.h"

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
}

void Turret::LateInit(const EngineContext& engineContext)
{
    JIN_LOG("Turret LateInit Called");
}

void Turret::Update(float dt, const EngineContext& engineContext)
{
    if (parentPlayer != nullptr)
    {
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

void Turret::SetControls(int left, int right)
{
    leftKey = left;
    rightKey = right;
}

