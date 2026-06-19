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
    SetMaterial(engineContext, "[Material]Animation");

    ammo = 40;

    // Ammo text
    ammoText = new TextObject(
        engineContext.renderManager->GetFontByTag("[Font]defaultkr"),
        " Ammo: " + std::to_string(ammo)
    );
    ammoText->SetRenderLayer("[Layer]UI");
    ammoText->GetTransform2D().SetScale(glm::vec2(0.5f, 0.5f));
    ammoText->SetIgnoreCamera(true);

    // Animation & Position setup based on Player type
    if (parentPlayer->GetTag() == "[Object]Player1")
    {
        // Spritesheet pl1
        shootSpritesheet = engineContext.renderManager->GetSpriteSheetByTag("[SpriteSheet]TurretBlue");
        shootSpritesheet->AddClip("[Clip]TurretIdleB", { 0 }, 1.0f);
        shootSpritesheet->AddClip("[Clip]ShootB", { 0, 1, 2, 3, 4, 5, 6, 7 }, 0.05f);

        AttachAnimator(std::make_unique<SpriteAnimator>(shootSpritesheet, 1.0f, true));
        GetSpriteAnimator()->PlayClip("[Clip]TurretIdleB");

        // Text position for Player1 (Top-Left)
        ammoText->GetTransform2D().SetPosition(glm::vec2(-600.f, 320.f));
    }
    else if (parentPlayer->GetTag() == "[Object]Player2")
    {
        // Spritesheet pl2
        shootSpritesheet = engineContext.renderManager->GetSpriteSheetByTag("[SpriteSheet]TurretRed");
        shootSpritesheet->AddClip("[Clip]TurretIdleR", { 0 }, 1.0f);
        shootSpritesheet->AddClip("[Clip]ShootR", { 0, 1, 2, 3, 4, 5, 6, 7 }, 0.05f);

        AttachAnimator(std::make_unique<SpriteAnimator>(shootSpritesheet, 1.0f, true));
        GetSpriteAnimator()->PlayClip("[Clip]TurretIdleR");

        // Text position for Player2 (Top-Right)
        ammoText->GetTransform2D().SetPosition(glm::vec2(500.f, 320.f));
    }

    SetRenderLayer("[Layer]Turret");

    // Register UI object to engine
    engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(std::unique_ptr<Object>(ammoText));
}

void Turret::LateInit(const EngineContext& engineContext)
{
}

void Turret::Update(float dt, const EngineContext& engineContext)
{
    // extra bullet in 8 sec when bullet < 5
    ammoRegenTimer -= dt;

    if (ammoRegenTimer <= 0.0f)
    {
        if (ammo < maxAmmo)
        {
            ammo++;
            if (ammoText != nullptr)
            {
                ammoText->SetText("Ammo: " + std::to_string(ammo));
            }
        }

        ammoRegenTimer = 8.0f;
    }

    // Animation state update
    if (isShooting)
    {
        animTimer -= dt;

        if (animTimer <= 0.0f)
        {
            isShooting = false;

            if (parentPlayer != nullptr)
            {
                if (parentPlayer->GetTag() == "[Object]Player1")
                    GetSpriteAnimator()->PlayClip("[Clip]TurretIdleB");
                else
                    GetSpriteAnimator()->PlayClip("[Clip]TurretIdleR");
            }
        }
    }

    if (parentPlayer != nullptr)
    {
        // parent tank position에 따하가기
        transform2D.SetPosition(parentPlayer->GetTransform2D().GetPosition());

        // Rotation
        float rotationSpeed = 1.5f;
        float currentAngle = transform2D.GetRotation();

        if (engineContext.inputManager->IsKeyDown(leftKey)) { currentAngle += rotationSpeed * dt; }
        if (engineContext.inputManager->IsKeyDown(rightKey)) { currentAngle -= rotationSpeed * dt; }

        transform2D.SetRotation(currentAngle);

        // Fire timer update
        if (fireTimer > 0.0f) { fireTimer -= dt; }

        // Shooting 
        if (engineContext.inputManager->IsKeyPressed(shootKey))
        {
            if (ammo > 0 && fireTimer <= 0.0f)
            {
                // bullet 생선
                Projectile* bullet = new Projectile();
                bullet->Init(engineContext);
                bullet->SetSpeedMultiplier(parentPlayer->GetSpeedMultiplier());
                if (isBigBullet) {
                    bullet->MakeBig(); // material, collider change in once
                    isBigBullet = false;
                }
                bullet->Init(engineContext);
                bullet->SetSpeedMultiplier(parentPlayer->GetSpeedMultiplier());
                ObjectManager& objManager = engineContext.stateManager->GetCurrentState()->GetObjectManager();

                // bullet collision 설치
                if (parentPlayer->GetTag() == "[Object]Player1")
                {
                    bullet->SetCollision(objManager, "[Object]Bullet", { "[Object]Wall", "[Object]Border_H", "[Object]Border_V", "[Object]Player2", "[Object]Item" });
                }
                else if (parentPlayer->GetTag() == "[Object]Player2")
                {
                    bullet->SetCollision(objManager, "[Object]Bullet", { "[Object]Wall", "[Object]Border_H", "[Object]Border_V", "[Object]Player1", "[Object]Item" });
                }

                // Calculate bullet spawn position & angle
                float offsetDistance = 50.0f;
                float mathAngle = currentAngle + glm::radians(90.0f);

                glm::vec2 turretPos = transform2D.GetPosition();
                glm::vec2 spawnPos;
                spawnPos.x = turretPos.x + (std::cos(mathAngle) * offsetDistance);
                spawnPos.y = turretPos.y + (std::sin(mathAngle) * offsetDistance);

                bullet->SetSpawnPosition(spawnPos);
                bullet->SetDirection(currentAngle);

                // Register bullet 
                engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(std::unique_ptr<Object>(bullet), "[Object]Bullet");

                // Update ammo
                ammo--;
                fireTimer = 0.4f;

                if (ammoText != nullptr)
                {
                    ammoText->SetText("Ammo: " + std::to_string(ammo));
                }

                // Trigger shooting animation
                isShooting = true;
                animTimer = 0.4f;

                GetSpriteAnimator()->SetPlaybackSpeed(1.0f);

                if (parentPlayer->GetTag() == "[Object]Player1")
                    GetSpriteAnimator()->PlayClip("[Clip]ShootB");
                else
                    GetSpriteAnimator()->PlayClip("[Clip]ShootR");
            }
        }
    }
}

void Turret::Draw(const EngineContext& engineContext)
{
}

void Turret::Free(const EngineContext& engineContext)
{
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

void Turret::AddAmmo(int amount)
{
    ammo += amount;
    if (ammoText != nullptr) {
        ammoText->SetText("Ammo: " + std::to_string(ammo));
    }
}

void Turret::EnableBigBullet()
{
    isBigBullet = true; 
}