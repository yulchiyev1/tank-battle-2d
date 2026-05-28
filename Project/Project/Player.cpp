#include "Player.h"
#include "Engine.h"


void Player::Init(const EngineContext& engineContext)
{
    transform2D.SetScale(glm::vec2(70.f));
    SetMesh(engineContext, "[EngineMesh]default");
    SetMaterial(engineContext, "[Material]Tank1");
    SetRenderLayer("[Layer]UI");
    SetTag("[Object]Player1");
    transform2D.SetPosition(glm::vec2(100.f, 300.f)); 
}

void Player::LateInit(const EngineContext& engineContext)
{
    JIN_LOG("Player LateInit Called");
}

void Player::Update(float dt, const EngineContext& engineContext) 
{
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
    JIN_LOG("Player Draw Called");
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