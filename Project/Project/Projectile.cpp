#include "Projectile.h"
#include "EngineContext.h"
#include "GameState.h"
#include "Player.h"


void Projectile::Init(const EngineContext& engineContext)
{
	SetMesh(engineContext, "[EngineMesh]default");
	SetMaterial(engineContext, "[Material]Bullet");

	transform2D.SetScale(glm::vec2(15.f));

	//bullet collision 
	SetCollider(std::make_unique<AABBCollider>(this, glm::vec2{ 15.f, 15.f }));
	GetCollider()->SetUseTransformScale(false);
	SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Bullet", { "[Object]Wall", "[Object]Player1", "[Object]Player2" });

	
	SetRenderLayer("[Layer]Projectile");

	speed = 350.0f;
	lifeTimer = 4.0f;
}

void Projectile::LateInit(const EngineContext& engineContext)
{

}

void Projectile::Update(float dt, const EngineContext& engineContext)
{
	//timer for bullet
	lifeTimer -= dt;
	if (lifeTimer <= 0.0f)
	{
		Kill();
	}

	//bullet moving
	glm::vec2 currentPos = transform2D.GetPosition();
	currentPos.x += velocityX * dt; 
	currentPos.y += velocityY * dt;
	transform2D.SetPosition(currentPos);
   
}

void Projectile::Draw(const EngineContext& engineContext)
{

}

void Projectile::Free(const EngineContext& engineContext)
{

}

void Projectile::LateFree(const EngineContext& engineContext)
{

}

void Projectile::OnCollision(Object* other, const EngineContext& engineContext)
{
	JIN_LOG("Collision : " + other->GetTag());

	if (other->GetTag() == "[Object]Player1" || other->GetTag() == "[Object]Player2")
	{
		Player* hitPlayer = dynamic_cast<Player*>(other);

		if (hitPlayer != nullptr)
		{
			hitPlayer->TakeDamage(8);
		}
		Kill();
	}
	else if (other->GetTag() == "[Object]Wall")
	{
		Kill(); 
	}
}

void Projectile::SetDirection(float startAngle)
{
    transform2D.SetRotation(startAngle);
	float mathAngle = startAngle + glm::radians(90.0f);

	velocityX = glm::cos(mathAngle) * speed;
	velocityY = glm::sin(mathAngle) * speed;
}