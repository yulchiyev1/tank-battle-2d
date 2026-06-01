#include "Player.h"
#include "Engine.h"


void Player::Init(const EngineContext& engineContext)
{
    transform2D.SetScale(glm::vec2(90.f));
    SetMesh(engineContext, "[EngineMesh]default");
    SetCollider(std::make_unique<AABBCollider>(this, glm::vec2{ 55.f, 55.f }));
    SetMaterial(engineContext, "[Material]Animation");

    if(this->GetTag() == "[Object]Player1")
    {
        transform2D.SetPosition(glm::vec2(-550.f, 0.f));

        GetCollider()->SetUseTransformScale(false);
        SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Player1", { "[Object]Wall", "[Object]Player2" });

        moveSpritesheetB = engineContext.renderManager->GetSpriteSheetByTag("[SpriteSheet]BlueTank");
        moveSpritesheetB->AddClip("[Clip]TankB", { 0,1 }, 0.1f);

        AttachAnimator(std::make_unique<SpriteAnimator>(moveSpritesheetB, 0.1f, true));
    }

    else if (this->GetTag() == "[Object]Player2")
    {
        transform2D.SetPosition(glm::vec2(550.f, 0.f));

        GetCollider()->SetUseTransformScale(false);
        SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Player2", { "[Object]Wall", "[Object]Player1" });

        moveSpritesheetR = engineContext.renderManager->GetSpriteSheetByTag("[SpriteSheet]RedTank");
        moveSpritesheetR->AddClip("[Clip]TankR", { 0,1 }, 0.1f);

        AttachAnimator(std::make_unique<SpriteAnimator>(moveSpritesheetR, 0.1f, true));
    }
        
    SetRenderLayer("[Layer]UI");
    
}

void Player::LateInit(const EngineContext& engineContext)
{
    JIN_LOG("Player LateInit Called");
}

void Player::Update(float dt, const EngineContext& engineContext) 
{
    //spritesheet

    if (this->GetTag() == "[Object]Player1")
    {
        if (engineContext.inputManager->IsKeyPressed(KEY_W) || engineContext.inputManager->IsKeyPressed(KEY_S))
        {
            GetSpriteAnimator()->SetPlaybackSpeed(1.0f);
            GetSpriteAnimator()->SetSpriteSheet(moveSpritesheetB);
            GetSpriteAnimator()->PlayClip("[Clip]TankB");
        }

    }

    else if (this->GetTag() == "[Object]Player2")
    {
        if (engineContext.inputManager->IsKeyPressed(KEY_DOWN) || engineContext.inputManager->IsKeyPressed(KEY_UP))
        {
            GetSpriteAnimator()->SetPlaybackSpeed(1.0f);
            GetSpriteAnimator()->SetSpriteSheet(moveSpritesheetR);
            GetSpriteAnimator()->PlayClip("[Clip]TankR");
        }
    }



    oldPos = transform2D.GetPosition(); //old position to not cross the wall/another player

    float speed = 100.f;
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