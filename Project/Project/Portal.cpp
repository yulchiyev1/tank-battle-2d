#include "Portal.h"
#include "Engine.h"
#include "Player.h"

void Portal::Init(const EngineContext& engineContext)
{
    transform2D.SetScale(glm::vec2(70.f));
    SetMesh(engineContext, "[EngineMesh]default");
    SetMaterial(engineContext, "[Material]Animation");
    SetRenderLayer("[Layer]Items");

    portalSpritesheet = engineContext.renderManager->GetSpriteSheetByTag("[SpriteSheet]Portal");
    portalSpritesheet->AddClip("[Clip]Portal", { 0, 1, 2, 3, 4, 5 }, 0.1f);
    AttachAnimator(std::make_unique<SpriteAnimator>(portalSpritesheet, 1.0f, true));
    GetSpriteAnimator()->PlayClip("[Clip]Portal");

    SetCollider(std::make_unique<AABBCollider>(this, glm::vec2{ 5.f, 30.f }));
    GetCollider()->SetUseTransformScale(false);
    SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Portal", { "[Object]Player1", "[Object]Player2" });
}

void Portal::LateInit(const EngineContext& engineContext)
{
}

void Portal::Update(float dt, const EngineContext& engineContext)
{
    // if portal is used, timer--dt
    if (cooldownTimer > 0.0f)
    {
        cooldownTimer -= dt;
    }
}

void Portal::Draw(const EngineContext& engineContext)
{
}

void Portal::Free(const EngineContext& engineContext)
{
}

void Portal::LateFree(const EngineContext& engineContext)
{
}

void Portal::OnCollision(Object* other, const EngineContext& engineContext)
{
    // Agar portal band bo'lsa yoki ulanmagan bo'lsa, hech nima qilmaydi
    if (cooldownTimer > 0.0f || linkedPortal == nullptr) return;

    if (other->GetTag() == "[Object]Player1" || other->GetTag() == "[Object]Player2")
    {
        Player* teleportingPlayer = static_cast<Player*>(other);
        if (teleportingPlayer->tpState != TeleportState::NONE) return;

        engineContext.soundManager->Play("[Sound]PortalSound", 0.7f, 0.0f); //sound
        // animation
        teleportingPlayer->tpState = TeleportState::ENTERING;
        teleportingPlayer->tpTimer = teleportingPlayer->tpDuration;

        teleportingPlayer->tpStartPos = teleportingPlayer->GetTransform2D().GetPosition();
        teleportingPlayer->tpCenter = this->GetTransform2D().GetPosition();
        teleportingPlayer->tpDest = linkedPortal->GetTransform2D().GetPosition();

        teleportingPlayer->inPortal = this;
        teleportingPlayer->outPortal = linkedPortal;
        teleportingPlayer->tpState = TeleportState::ENTERING;
        teleportingPlayer->tpTimer = teleportingPlayer->tpDuration;

        teleportingPlayer->tpStartPos = teleportingPlayer->GetTransform2D().GetPosition();
        teleportingPlayer->tpCenter = this->GetTransform2D().GetPosition();
        teleportingPlayer->tpDest = linkedPortal->GetTransform2D().GetPosition();

        this->cooldownTimer = 3.0f;
        linkedPortal->cooldownTimer = 3.0f;
    }
}