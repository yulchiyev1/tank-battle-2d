#include "Player.h"
#include "Turret.h"
#include "Engine.h"
#include "Projectile.h"
#include "Item.h"
#include "Portal.h"

void Player::Init(const EngineContext& engineContext)
{
    transform2D.SetScale(glm::vec2(90.f));
    SetMesh(engineContext, "[EngineMesh]default");
    SetCollider(std::make_unique<CircleCollider>(this, 50.f));
    SetMaterial(engineContext, "[Material]Animation");
    SetRenderLayer("[Layer]TankBody");

    ObjectManager& objManager = engineContext.stateManager->GetCurrentState()->GetObjectManager();

    // turret setup
    myTurret = static_cast<Turret*>(objManager.AddObject(std::make_unique<Turret>(this)));

    if (this->GetTag() == "[Object]Player1")
    {
        // player 1 
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
        // player 2
        transform2D.SetPosition(glm::vec2(550.f, 0.f));
        GetCollider()->SetUseTransformScale(false);
        SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Player2",
            { "[Object]Portal", "[Object]Wall", "[Object]Player1", "[Object]Bullet", "[Object]Item", "[Object]Border_H", "[Object]Border_V" });

        moveSpritesheetR = engineContext.renderManager->GetSpriteSheetByTag("[SpriteSheet]RedTank");
        moveSpritesheetR->AddClip("[Clip]TankR", { 0, 1 }, 0.1f);
        AttachAnimator(std::make_unique<SpriteAnimator>(moveSpritesheetR, 0.1f, true));

        myTurret->SetControls(KEY_NUMPAD_1, KEY_NUMPAD_2, KEY_NUMPAD_3);
    }

    lastTrackPos = transform2D.GetPosition();

    // hp bar setup
    hpBg = static_cast<GameObject*>(objManager.AddObject(std::make_unique<GameObject>(), "[Object]HpBg_" + this->GetTag()));
    hpBg->SetMesh(engineContext, "[EngineMesh]default");
    hpBg->SetMaterial(engineContext, "[Material]HpBg");
    hpBg->GetTransform2D().SetScale({ hpBaseWidth, hpBaseHeight });
    hpBg->SetRenderLayer("[Layer]UI");

    hpFill = static_cast<GameObject*>(objManager.AddObject(std::make_unique<GameObject>(), "[Object]HpFill_" + this->GetTag()));
    hpFill->SetMesh(engineContext, "[EngineMesh]default");

    if (this->GetTag() == "[Object]Player1") {
        hpFill->SetMaterial(engineContext, "[Material]HpBlue");
    }
    else {
        hpFill->SetMaterial(engineContext, "[Material]HpRed");
    }

    hpFill->GetTransform2D().SetScale({ hpBaseWidth, hpBaseHeight });
    hpFill->SetRenderLayer("[Layer]HPUI");
}

void Player::LateInit(const EngineContext& engineContext) {}
void Player::Draw(const EngineContext& engineContext) {}
void Player::Free(const EngineContext& engineContext) {}
void Player::LateFree(const EngineContext& engineContext) {}

void Player::Update(float dt, const EngineContext& engineContext)
{
    // hp bar update
    if (hpBg != nullptr && hpFill != nullptr)
    {
        glm::vec2 myPos = GetTransform2D().GetPosition();
        float barY = myPos.y + 45.0f;

        hpBg->GetTransform2D().SetPosition({ myPos.x, barY });

        float hpPercent = std::max(0.0f, (float)hp / 100.0f);
        float currentWidth = hpBaseWidth * hpPercent;

        hpFill->GetTransform2D().SetScale({ currentWidth, hpBaseHeight });

        float offsetX = (hpBaseWidth - currentWidth) / 2.0f;
        hpFill->GetTransform2D().SetPosition({ myPos.x - offsetX, barY });

        if (hpPercent >= 0.75f) {
            hpFill->SetMaterial(engineContext, "[Material]HpGreen");
        }
        else if (hpPercent >= 0.20f) {
            hpFill->SetMaterial(engineContext, "[Material]HpBlue");
        }
        else {
            hpFill->SetMaterial(engineContext, "[Material]HpRed");
        }
    }

    // teleportation logic
    if (tpState != TeleportState::NONE)
    {
        tpTimer -= dt;
        float t = tpTimer / tpDuration;
        if (t < 0.0f) t = 0.0f;

        if (tpState == TeleportState::ENTERING)
        {
            float currentScale = 90.0f * t;
            transform2D.SetScale(glm::vec2(currentScale));

            glm::vec2 newPos = tpCenter + (tpStartPos - tpCenter) * t;
            transform2D.SetPosition(newPos);

            if (myTurret != nullptr) {
                myTurret->GetTransform2D().SetScale(glm::vec2(currentScale));
                myTurret->GetTransform2D().SetPosition(newPos);
            }

            if (inPortal != nullptr) {
                inPortal->GetTransform2D().SetScale(glm::vec2(40.0f + (30.0f * t)));
            }

            if (tpTimer <= 0.0f)
            {
                tpState = TeleportState::EXITING;
                tpTimer = tpDuration;
                transform2D.SetPosition(tpDest);
            }
        }
        else if (tpState == TeleportState::EXITING)
        {
            float currentScale = 90.0f * (1.0f - t);
            transform2D.SetScale(glm::vec2(currentScale));

            if (myTurret != nullptr) {
                myTurret->GetTransform2D().SetScale(glm::vec2(currentScale));
                myTurret->GetTransform2D().SetPosition(transform2D.GetPosition());
            }

            if (outPortal != nullptr) {
                outPortal->GetTransform2D().SetScale(glm::vec2(100.0f + (30.0f * t)));
            }

            std::string otherTag = (this->GetTag() == "[Object]Player1") ? "[Object]Player2" : "[Object]Player1";
            Object* otherObj = engineContext.stateManager->GetCurrentState()->GetObjectManager().FindByTag(otherTag);
            if (otherObj != nullptr)
            {
                Player* otherPlayer = static_cast<Player*>(otherObj);
                otherPlayer->PushAwayFrom(transform2D.GetPosition(), 100.0f);
            }

            if (tpTimer <= 0.0f)
            {
                tpState = TeleportState::NONE;
                transform2D.SetScale(glm::vec2(90.0f));
                if (myTurret != nullptr) myTurret->GetTransform2D().SetScale(glm::vec2(90.0f));

                if (inPortal) inPortal->GetTransform2D().SetScale(glm::vec2(70.0f));
                if (outPortal) outPortal->GetTransform2D().SetScale(glm::vec2(70.0f));
                inPortal = nullptr;
                outPortal = nullptr;
            }
        }
        return;
    }

    // item effects & timers
    if (flashTimer > 0.0f)
    {
        flashTimer -= dt;
        if (flashTimer <= 0.0f)
        {
            speedMultiplier = 1.0f;
            JIN_LOG("(FLASH EFFECT FINISHED)");
        }
    }

    // movement animations
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
    // physics & movement logic
    oldPos = transform2D.GetPosition();

    float speed = 130.f * speedMultiplier;
    float diagSpeed = speed * 0.707f;

    glm::vec2 pos = transform2D.GetPosition();
    float currentAngle = transform2D.GetRotation();
    float targetAngle = currentAngle;
    bool isMoving = false;

    if (engineContext.inputManager->IsKeyDown(upKey) && engineContext.inputManager->IsKeyDown(leftKey)) {
        pos.y += diagSpeed * dt; pos.x -= diagSpeed * dt;
        targetAngle = glm::radians(45.0f); isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(upKey) && engineContext.inputManager->IsKeyDown(rightKey)) {
        pos.y += diagSpeed * dt; pos.x += diagSpeed * dt;
        targetAngle = glm::radians(-45.0f); isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(downKey) && engineContext.inputManager->IsKeyDown(leftKey)) {
        pos.y -= diagSpeed * dt; pos.x -= diagSpeed * dt;
        targetAngle = glm::radians(135.0f); isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(downKey) && engineContext.inputManager->IsKeyDown(rightKey)) {
        pos.y -= diagSpeed * dt; pos.x += diagSpeed * dt;
        targetAngle = glm::radians(-135.0f); isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(upKey)) {
        pos.y += speed * dt;
        targetAngle = glm::radians(0.0f); isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(downKey)) {
        pos.y -= speed * dt;
        targetAngle = glm::radians(-180.0f); isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(leftKey)) {
        pos.x -= speed * dt;
        targetAngle = glm::radians(90.0f); isMoving = true;
    }
    else if (engineContext.inputManager->IsKeyDown(rightKey)) {
        pos.x += speed * dt;
        targetAngle = glm::radians(-90.0f); isMoving = true;
    }

    // smooth rotation
    if (isMoving)
    {
        float diff = targetAngle - currentAngle;
        diff = atan2(sin(diff), cos(diff));
        currentAngle += diff * 2.0f * dt;
        transform2D.SetRotation(currentAngle);
    }


    if (glm::distance(pos, lastTrackPos) > trackDistance)
    {
        ObjectManager& objManager = engineContext.stateManager->GetCurrentState()->GetObjectManager();

        glm::vec2 forward = glm::vec2(-sin(currentAngle), cos(currentAngle));
        glm::vec2 rightDir = glm::vec2(cos(currentAngle), sin(currentAngle));

        glm::vec2 rearCenter = pos - forward * trackRearOffset;

        float sideOffset = 15.0f;

        glm::vec2 leftPos = rearCenter - rightDir * sideOffset;
        glm::vec2 rightPos = rearCenter + rightDir * sideOffset;

        // track left
        GameObject* trackL = static_cast<GameObject*>(
            objManager.AddObject(std::make_unique<GameObject>(), "[Object]Track")
            );

        trackL->SetMesh(engineContext, "[EngineMesh]default");
        trackL->SetMaterial(engineContext, "[Material]Track");
        trackL->GetTransform2D().SetPosition(leftPos);
        trackL->GetTransform2D().SetRotation(currentAngle);
        trackL->GetTransform2D().SetScale({ 9.0f, 7.0f });
        trackL->SetRenderLayer("[Layer]Items");
        activeTracks.push_back({ trackL, trackLifeTime });

        // track right
        GameObject* trackR = static_cast<GameObject*>(
            objManager.AddObject(std::make_unique<GameObject>(), "[Object]Track")
            );

        trackR->SetMesh(engineContext, "[EngineMesh]default");
        trackR->SetMaterial(engineContext, "[Material]Track");
        trackR->GetTransform2D().SetPosition(rightPos);
        trackR->GetTransform2D().SetRotation(currentAngle);
        trackR->GetTransform2D().SetScale({ 9.0f, 7.0f });
        trackR->SetRenderLayer("[Layer]Items");
        activeTracks.push_back({ trackR, trackLifeTime });

        lastTrackPos = pos;
    }


    // fade and remove tracks
    for (auto it = activeTracks.begin(); it != activeTracks.end(); )
    {
        it->second -= dt;

        if (it->second > 0.0f)
        {
            float alpha = it->second / trackLifeTime;
            alpha = glm::clamp(alpha, 0.0f, 1.0f);

            it->first->SetColor({ 1.0f, 1.0f, 1.0f, alpha });
            ++it;
        }
        else
        {
            it->first->Kill();
            it = activeTracks.erase(it);
        }
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

// collision handling
void Player::OnCollision(Object* other, const EngineContext& engineContext)
{
    if (tpState != TeleportState::NONE) return;

    if (other->GetTag() == "[Object]Wall" || other->GetTag() == "[Object]Border_H" || other->GetTag() == "[Object]Border_V")
    {
        JIN_LOG("WALL COLLISION");
        transform2D.SetPosition(oldPos);
        myTurret->GetTransform2D().SetPosition(oldPos);
    }

    if (other->GetTag() == "[Object]Item")
    {
        JIN_LOG("ITEM COLLISION");
        Item* hitItem = dynamic_cast<Item*>(other);

        if (hitItem != nullptr && !hitItem->IsUnlocked())
        {
            transform2D.SetPosition(oldPos);
            myTurret->GetTransform2D().SetPosition(oldPos);
            return;
        }
    }

    if (other->GetTag() == "[Object]Player1" || other->GetTag() == "[Object]Player2")
    {
        JIN_LOG("PLAYER COLLISION");
        glm::vec2 myPos = transform2D.GetPosition();
        glm::vec2 otherPos = other->GetTransform2D().GetPosition();

        glm::vec2 pushDir = myPos - otherPos;
        float distance = glm::length(pushDir);

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

// player stats & item effects
void Player::TakeDamage(int damageAmount)
{
    hp -= damageAmount;
    if (hp < 0) hp = 0;
}

void Player::AddHealth(int amount)
{
    hp += amount;
    if (hp > 100) hp = 100;
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
    speedMultiplier = 1.8f;
    flashTimer = 8.0f;
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

        myPos += dir * 4.0f;

        transform2D.SetPosition(myPos);
        if (myTurret != nullptr) {
            myTurret->GetTransform2D().SetPosition(myPos);
        }
    }
}