#include "Projectile.h"
#include "EngineContext.h"
#include "GameState.h"

void Projectile::Init(const EngineContext& engineContext)
{
	transform2D.SetScale(glm::vec2(20.f));
	SetMesh(engineContext, "[Engine]default");
	SetMaterial(engineContext, "[Material]Projectile");
	SetTag("[Object]Projectile");
	SetRenderLayer("[Layer]GameObject");

}

void Projectile::LateInit(const EngineContext& engineContext)
{

}

void Projectile::Update(float dt, const EngineContext& engineContext)
{
  
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

void OnCollision(Object* other, const EngineContext& engineContext)
{

}