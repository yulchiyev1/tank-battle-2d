#include "Player.h"
#include "Turret.h"
#include "Engine.h"
#include "Projectile.h"
#include "Item.h"
#include "Portal.h"

void Player::Init(const EngineContext& engineContext)
{
    // --- Basic Physics & Render Setup ---
    transform2D.SetScale(glm::vec2(90.f));
    SetMesh(engineContext, "[EngineMesh]default");
    SetCollider(std::make_unique<CircleCollider>(this, 50.f));
    SetMaterial(engineContext, "[Material]Animation");
    SetRenderLayer("[Layer]TankBody");

    // --- UI Setup: Health Points ---
    ObjectManager& objManager = engineContext.stateManager->GetCurrentState()->GetObjectManager();
    hpText = static_cast<TextObject*>(objManager.AddObject(std::make_unique<TextObject>(
        engineContext.renderManager->GetFontByTag("[Font]defaultkr"),
        std::to_string(hp)
    )));
    hpText->SetRenderLayer("[Layer]UI");

    // --- Turret Setup ---
    myTurret = static_cast<Turret*>(objManager.AddObject(std::make_unique<Turret>(this)));

    // --- Player Specific Configurations ---
    if (this->GetTag() == "[Object]Player1")
    {
        // Player 1 (Blue Tank) Setup
        transform2D.SetPosition(glm::vec2(-550.f, 0.f));
        GetCollider()->SetUseTransformScale(false);
        SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Player1",
            { "[Object]Portal", "[Object]Wall", "[Object]Player2", "[Object]Bullet", "[Object]Item", "[Object]Border_H", "[Object]Border_V" });

        moveSpritesheetB = engineContext.renderManager->GetSpriteSheetByTag("[SpriteSheet]BlueTank");
        moveSpritesheetB->AddClip("[Clip]TankB", { 0, 1 }, 0.1f);
        AttachAnimator(std::make_unique<SpriteAnimator>(moveSpritesheetB, 0.1f, true));

        myTurret->SetControls(KEY_B, KEY_N, KEY_M);
    }
    else if (this->GetTag() == "[Object]Player2")
    {
        // Player 2 (Red Tank) Setup
        transform2D.SetPosition(glm::vec2(550.f, 0.f));
        GetCollider()->SetUseTransformScale(false);
        SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Player2",
            { "[Object]Portal", "[Object]Wall", "[Object]Player1", "[Object]Bullet", "[Object]Item", "[Object]Border_H", "[Object]Border_V" });

        moveSpritesheetR = engineContext.renderManager->GetSpriteSheetByTag("[SpriteSheet]RedTank");
        moveSpritesheetR->AddClip("[Clip]TankR", { 0, 1 }, 0.1f);
        AttachAnimator(std::make_unique<SpriteAnimator>(moveSpritesheetR, 0.1f, true));

        myTurret->SetControls(KEY_NUMPAD_1, KEY_NUMPAD_2, KEY_NUMPAD_3);
    }
}

void Player::LateInit(const EngineContext& engineContext) {}
void Player::Draw(const EngineContext& engineContext) {}
void Player::Free(const EngineContext& engineContext) {}
void Player::LateFree(const EngineContext& engineContext) {}

void Player::Update(float dt, const EngineContext& engineContext)
{
    // TELEPORTATION STATE 
    if (tpState != TeleportState::NONE)
    {
        tpTimer -= dt;
        float t = tpTimer / tpDuration; // Decreases from 1.0 to 0.0
        if (t < 0.0f) t = 0.0f;

        if (tpState == TeleportState::ENTERING)
        {
            // Shrink animation (90 to 0)
            float currentScale = 90.0f * t;
            transform2D.SetScale(glm::vec2(currentScale));

            // Smoothly pull tank to portal center (Lerp)
            glm::vec2 newPos = tpCenter + (tpStartPos - tpCenter) * t;
            transform2D.SetPosition(newPos);

            // Scale and move turret along with the tank
            if (myTurret != nullptr) {
                myTurret->GetTransform2D().SetScale(glm::vec2(currentScale));
                myTurret->GetTransform2D().SetPosition(newPos);
            }

            // Shrink the entry portal (70 down to 40)
            if (inPortal != nullptr) {
                inPortal->GetTransform2D().SetScale(glm::vec2(40.0f + (30.0f * t)));
            }

            // Entering finished, start exiting from the other portal
            if (tpTimer <= 0.0f)
            {
                tpState = TeleportState::EXITING;
                tpTimer = tpDuration;
                transform2D.SetPosition(tpDest); // Move to linked portal
            }
        }
        else if (tpState == TeleportState::EXITING)
        {
            // Grow animation (0 to 90)
            float currentScale = 90.0f * (1.0f - t);
            transform2D.SetScale(glm::vec2(currentScale));

            if (myTurret != nullptr) {
                myTurret->GetTransform2D().SetScale(glm::vec2(currentScale));
                myTurret->GetTransform2D().SetPosition(transform2D.GetPosition());
            }

            // Expand the exit portal (100 down to 70)
            if (outPortal != nullptr) {
                outPortal->GetTransform2D().SetScale(glm::vec2(70.0f + (30.0f * t))); // Fixed 130.0f typo to 30.0f
            }

            // Smoothly push other players away while exiting
            std::string otherTag = (this->GetTag() == "[Object]Player1") ? "[Object]Player2" : "[Object]Player1";
            Object* otherObj = engineContext.stateManager->GetCurrentState()->GetObjectManager().FindByTag(otherTag);
            if (otherObj != nullptr)
            {
                Player* otherPlayer = static_cast<Player*>(otherObj);
                otherPlayer->PushAwayFrom(transform2D.GetPosition(), 100.0f);
            }

            // Animation complete, reset states
            if (tpTimer <= 0.0f)
            {
                tpState = TeleportState::NONE;
                transform2D.SetScale(glm::vec2(90.0f));
                if (myTurret != nullptr) myTurret->GetTransform2D().SetScale(glm::vec2(90.0f));

                // Reset portals to default scale and clear references
                if (inPortal) inPortal->GetTransform2D().SetScale(glm::vec2(70.0f));
                if (outPortal) outPortal->GetTransform2D().SetScale(glm::vec2(70.0f));
                inPortal = nullptr;
                outPortal = nullptr;
            }
        }

        // IMPORTANT: Skip movement and shooting logic while teleporting
        return;
    }

    // ITEM EFFECTS & TIMERS
    if (flashTimer > 0.0f)
    {
        flashTimer -= dt;
        if (flashTimer <= 0.0f)
        {
            speedMultiplier = 1.0f; // Reset speed
            JIN_LOG("(FLASH EFFECT FINISHED)");
        }
    }

    // UI UPDATES
    if (hpText != nullptr)
    {
        hpText->GetTransform2D().SetPosition(GetTransform2D().GetPosition() + glm::vec2{ -25.f, 80.f });
    }

    // MOVEMENT ANIMATIONS
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

    // PHYSICS & MOVEMENT LOGIC
    oldPos = transform2D.GetPosition(); // Save previous position for collisions

    float speed = 130.f * speedMultiplier;
    float diagSpeed = speed * 0.707f; // Diagonal speed

    glm::vec2 pos = transform2D.GetPosition();
    float currentAngle = transform2D.GetRotation();
    float targetAngle = currentAngle;
    bool isMoving = false;

    // --- Diagonal Movement ---
    if (engineContext.inputManager->IsKeyDown(upKey) && engineContext.inputManager->IsKeyDown(leftKey))
    {
        pos.y += diagSpeed * dt;
        pos.x -= diagSpeed * dt;
        targetAngle = glm::radians(45.0f);
        isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(upKey) && engineContext.inputManager->IsKeyDown(rightKey))
    {
        pos.y += diagSpeed * dt;
        pos.x += diagSpeed * dt;
        targetAngle = glm::radians(-45.0f);
        isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(downKey) && engineContext.inputManager->IsKeyDown(leftKey))
    {
        pos.y -= diagSpeed * dt;
        pos.x -= diagSpeed * dt;
        targetAngle = glm::radians(135.0f);
        isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(downKey) && engineContext.inputManager->IsKeyDown(rightKey))
    {
        pos.y -= diagSpeed * dt;
        pos.x += diagSpeed * dt;
        targetAngle = glm::radians(-135.0f);
        isMoving = true;
    }
    // --- Straight Movement ---
    else if (engineContext.inputManager->IsKeyDown(upKey))
    {
        pos.y += speed * dt;
        targetAngle = glm::radians(0.0f);
        isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(downKey))
    {
        pos.y -= speed * dt;
        targetAngle = glm::radians(-180.0f);
        isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(leftKey))
    {
        pos.x -= speed * dt;
        targetAngle = glm::radians(90.0f);
        isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(rightKey))
    {
        pos.x += speed * dt;
        targetAngle = glm::radians(-90.0f);
        isMoving = true;
    }

    // --- Smooth Rotation ---
    if (isMoving)
    {
        float diff = targetAngle - currentAngle;
        diff = atan2(sin(diff), cos(diff)); // Normalize angle difference
        currentAngle += diff * 2.0f * dt;
        transform2D.SetRotation(currentAngle);
    }

    transform2D.SetPosition(pos);
}

void Player::SetControls(int up, int down, int left, int right)
{
    upKey = up;
    downKey = down;
    leftKey = left;
    rightKey = right;
}

// COLLISION HANDLING

void Player::OnCollision(Object* other, const EngineContext& engineContext)
{
    // Ignore physical collisions if currently teleporting
    if (tpState != TeleportState::NONE) return;

    // --- Wall & Border Collisions ---
    if (other->GetTag() == "[Object]Wall" || other->GetTag() == "[Object]Border_H" || other->GetTag() == "[Object]Border_V")
    {
        JIN_LOG("WALL COLLISION");
        transform2D.SetPosition(oldPos);
        myTurret->GetTransform2D().SetPosition(oldPos);
    }

    // --- Item Collisions ---
    if (other->GetTag() == "[Object]Item")
    {
        JIN_LOG("ITEM COLLISION");
        Item* hitItem = dynamic_cast<Item*>(other);

        // Block movement if the item box is still locked (unbroken)
        if (hitItem != nullptr && !hitItem->IsUnlocked())
        {
            transform2D.SetPosition(oldPos);
            myTurret->GetTransform2D().SetPosition(oldPos);
            return;
        }
    }

    // --- Player Collisions (Push Effect) ---
    if (other->GetTag() == "[Object]Player1" || other->GetTag() == "[Object]Player2")
    {
        JIN_LOG("PLAYER COLLISION");
        glm::vec2 myPos = transform2D.GetPosition();
        glm::vec2 otherPos = other->GetTransform2D().GetPosition();

        glm::vec2 pushDir = myPos - otherPos;
        float distance = glm::length(pushDir);

        // Distance filter: Prevent overlapping tanks
        if (distance < 5.0f)
        {
            if (distance == 0.0f) pushDir = glm::vec2(1.0f, 1.0f);

            pushDir = glm::normalize(pushDir);
            myPos += pushDir * 2.5f;

            transform2D.SetPosition(myPos);
            myTurret->GetTransform2D().SetPosition(myPos);
        }
        else
        {
            transform2D.SetPosition(oldPos);
            myTurret->GetTransform2D().SetPosition(oldPos);
        }
    }
}

// PLAYER STATS & ITEM EFFECTS

void Player::TakeDamage(int damageAmount)
{
    hp -= damageAmount;
    if (hp < 0) hp = 0;

    if (hpText != nullptr) hpText->SetText(std::to_string(hp));
}

void Player::AddHealth(int amount)
{
    hp += amount;
    if (hp > 100) hp = 100;

    if (hpText != nullptr) hpText->SetText(std::to_string(hp));
}

void Player::AddAmmo(int amount)
{
    if (myTurret != nullptr) myTurret->AddAmmo(amount);
}

void Player::IncreaseBulletSize()
{
    if (myTurret != nullptr) myTurret->EnableBigBullet();
}

void Player::IncreaseSpeed()
{
    speedMultiplier = 1.8f; // 1.8x speed boost
    flashTimer = 8.0f;      // Lasts for 8 seconds
}

void Player::PushAwayFrom(glm::vec2 epicenter, float safeDistance)
{
    glm::vec2 myPos = transform2D.GetPosition();
    glm::vec2 dir = myPos - epicenter;
    float dist = glm::length(dir);

    if (dist < safeDistance)
    {
        if (dist == 0.0f) { dir = glm::vec2(1.0f, 1.0f); dist = 0.1f; }
        dir = glm::normalize(dir);

        // Smooth push by 4.0f per frame
        myPos += dir * 4.0f;

        transform2D.SetPosition(myPos);
        if (myTurret != nullptr) {
            myTurret->GetTransform2D().SetPosition(myPos);
        }
    }
}