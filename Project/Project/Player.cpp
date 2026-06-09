#include "Player.h"
#include "Turret.h"
#include "Engine.h"
#include "Projectile.h"
#include "Item.h"

void Player::Init(const EngineContext& engineContext)
{
    // Basic setup
    transform2D.SetScale(glm::vec2(90.f));
    SetMesh(engineContext, "[EngineMesh]default");
    SetCollider(std::make_unique<AABBCollider>(this, glm::vec2{ 55.f, 55.f }));
    SetMaterial(engineContext, "[Material]Animation");
    SetRenderLayer("[Layer]TankBody");

    // UI Setup: Health points text
    ObjectManager& objManager = engineContext.stateManager->GetCurrentState()->GetObjectManager();
    hpText = static_cast<TextObject*>(objManager.AddObject(std::make_unique<TextObject>(
        engineContext.renderManager->GetFontByTag("[Font]defaultkr"),
        std::to_string(hp)
    )));
    hpText->SetRenderLayer("[Layer]UI");

    // Turret setup
    myTurret = static_cast<Turret*>(objManager.AddObject(std::make_unique<Turret>(this)));

    // Player specific configuration
    if (this->GetTag() == "[Object]Player1")
    {
        // Player 1 setup (Blue Tank)
        transform2D.SetPosition(glm::vec2(-550.f, 0.f));

        GetCollider()->SetUseTransformScale(false);
        SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Player1", { "[Object]Wall", "[Object]Player2", "[Object]Bullet", "[Object]Item", "[Object]Border_H", "[Object]Border_V" });

        moveSpritesheetB = engineContext.renderManager->GetSpriteSheetByTag("[SpriteSheet]BlueTank");
        moveSpritesheetB->AddClip("[Clip]TankB", { 0, 1 }, 0.1f);

        AttachAnimator(std::make_unique<SpriteAnimator>(moveSpritesheetB, 0.1f, true));

        myTurret->SetControls(KEY_B, KEY_N, KEY_SPACE);
    }
    else if (this->GetTag() == "[Object]Player2")
    {
        // Player 2 setup (Red Tank)
        transform2D.SetPosition(glm::vec2(550.f, 0.f));

        GetCollider()->SetUseTransformScale(false);
        SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Player2", { "[Object]Wall", "[Object]Player1", "[Object]Bullet", "[Object]Item", "[Object]Border_H", "[Object]Border_V" });

        moveSpritesheetR = engineContext.renderManager->GetSpriteSheetByTag("[SpriteSheet]RedTank");
        moveSpritesheetR->AddClip("[Clip]TankR", { 0, 1 }, 0.1f);

        AttachAnimator(std::make_unique<SpriteAnimator>(moveSpritesheetR, 0.1f, true));

        myTurret->SetControls(KEY_NUMPAD_1, KEY_NUMPAD_2, KEY_NUMPAD_3);
    }
}

void Player::LateInit(const EngineContext& engineContext)
{
}

void Player::Update(float dt, const EngineContext& engineContext)
{
    // Update funksiyasi boshiga:
    if (flashTimer > 0.0f)
    {
        flashTimer -= dt;
        if (flashTimer <= 0.0f)
        {
            speedMultiplier = 1.0f; // Vaqt tugadi, tezlik yana 1x ga qaytdi
            JIN_LOG("(FLASH EFFECT FINISHED)");
        }
    }

    // Update HP text position (follow tank)
    if (hpText != nullptr)
    {
        hpText->GetTransform2D().SetPosition(GetTransform2D().GetPosition() + glm::vec2{ -25.f, 80.f });
    }

    // Handle movement animations
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

    // Save previous position to prevent wall clipping
    oldPos = transform2D.GetPosition();

    // Calculate movement
    float speed = 140.f * speedMultiplier; //  TANK 2X 
    glm::vec2 pos = transform2D.GetPosition();

    if (engineContext.inputManager->IsKeyDown(upKey)) { pos.y += speed * dt; }
    if (engineContext.inputManager->IsKeyDown(downKey)) { pos.y -= speed * dt; }
    if (engineContext.inputManager->IsKeyDown(leftKey)) { pos.x -= speed * dt; }
    if (engineContext.inputManager->IsKeyDown(rightKey)) { pos.x += speed * dt; }

    transform2D.SetPosition(pos);
}

void Player::Draw(const EngineContext& engineContext)
{
}

void Player::Free(const EngineContext& engineContext)
{
}

void Player::LateFree(const EngineContext& engineContext)
{
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
    if (other->GetTag() == "[Object]Wall" || other->GetTag() == "[Object]Border_H" || other->GetTag() == "[Object]Border_V")
    {
        transform2D.SetPosition(oldPos);
        myTurret->GetTransform2D().SetPosition(oldPos);
    }
    else if (other->GetTag() == "[Object]Player1" || other->GetTag() == "[Object]Player2")
    {
        transform2D.SetPosition(oldPos);
        myTurret->GetTransform2D().SetPosition(oldPos); 
    }
    else if (other->GetTag() == "[Object]Item")
    {
        Item* hitItem = dynamic_cast<Item*>(other);

        if (hitItem != nullptr && !hitItem->IsUnlocked())
        {
            transform2D.SetPosition(oldPos);
            myTurret->GetTransform2D().SetPosition(oldPos);
        }
    }
}

void Player::TakeDamage(int damageAmount)
{
    hp -= damageAmount;

    if (hp < 0)
    {
        hp = 0;
    }

    if (hpText != nullptr)
    {
        hpText->SetText(std::to_string(hp));
    }
}

void Player::AddHealth(int amount)
{
    hp += amount;
    if (hp > 100) hp = 100;

    if (hpText != nullptr) {
        hpText->SetText(std::to_string(hp));
    }
}

void Player::AddAmmo(int amount)
{
    if (myTurret != nullptr) {
        myTurret->AddAmmo(amount);
    }
}

void Player::IncreaseBulletSize()
{
    if (myTurret != nullptr) {
        myTurret->EnableBigBullet();
    }
}

void Player::IncreaseSpeed()
{
    speedMultiplier = 1.8f; // Tezlikni 2 barobar qilib belgilaymiz
    flashTimer = 8.0f;     // 12 soniya vaqt beramiz
}