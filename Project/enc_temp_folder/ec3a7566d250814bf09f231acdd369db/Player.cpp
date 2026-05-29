#include "Player.h"
#include "Engine.h"


void Player::Init(const EngineContext& engineContext)
{
    transform2D.SetScale(glm::vec2(70.f));
    SetMesh(engineContext, "[EngineMesh]default");

   
    if(this->GetTag() == "[Object]Player1")
    {
        SetMaterial(engineContext, "[Material]Tank1");
        transform2D.SetPosition(glm::vec2(-550.f, 0.f));

        SetCollider(std::make_unique<AABBCollider>(this, glm::vec2{ 65.f, 65.f }));
        GetCollider()->SetUseTransformScale(false);
        SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Player1", { "[Object]Wall", "[Object]Player2" });
    }

    else if (this->GetTag() == "[Object]Player2")
    {
        SetMaterial(engineContext, "[Material]Tank2");
        transform2D.SetPosition(glm::vec2(550.f, 0.f));

        SetCollider(std::make_unique<AABBCollider>(this, glm::vec2{ 65.f, 65.f }));
        GetCollider()->SetUseTransformScale(false);
        SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Player2", { "[Object]Wall", "[Object]Player1" });

    }
        
    SetRenderLayer("[Layer]UI");
    
}

void Player::LateInit(const EngineContext& engineContext)
{
    JIN_LOG("Player LateInit Called");
}

void Player::Update(float dt, const EngineContext& engineContext) 
{
    oldPos = transform2D.GetPosition(); ///////////////////////

    float speed = 200.f;
    glm::vec2 pos = transform2D.GetPosition();

    if (engineContext.inputManager->IsKeyDown(upKey)) { pos.y += speed * dt; }
    if (engineContext.inputManager->IsKeyDown(downKey)) { pos.y -= speed * dt; }
    if (engineContext.inputManager->IsKeyDown(leftKey)) { pos.x -= speed * dt; }
    if (engineContext.inputManager->IsKeyDown(rightKey)) { pos.x += speed * dt; }

    transform2D.SetPosition(pos);

}

void Player::Draw(const EngineContext& engineContext)
{
  //  JIN_LOG("Player Draw Called");
}

void Player::Free(const EngineContext& engineContext)
{
    JIN_LOG("Player Free Called");
}

void Player::LateFree(const EngineContext& engineContext)
{
    JIN_LOG("Player LateFree Called");
}

void Player::SetControls(int up, int down, int left, int right)
{
    upKey = up;
    downKey = down;
    leftKey = left;
    rightKey = right;
}

void Player::OnCollision(Object* other, const EngineContext& engineContext)
{
    JIN_LOG("Collision : " + other->GetTag());

    if (other->GetTag() == "[Object]Wall")
    {
        transform2D.SetPosition(oldPos);
    }
    
    else if (other->GetTag() == "[Object]Player1" || other->GetTag() == "[Object]Player2")
    {
        transform2D.SetPosition(oldPos);
    }
}