#pragma once
#include "Engine.h"

class Portal : public GameObject
{
public:
	void Init(const EngineContext& engineContext) override;
	void LateInit(const EngineContext& engineContext) override;
	void Update(float dt, const EngineContext& engineContext) override;
	void Draw(const EngineContext& engineContext) override;
	void Free(const EngineContext& engineContext) override;
	void LateFree(const EngineContext& engineContext) override;
	void OnCollision(Object* other, const EngineContext& engineContext) override;

	Portal* linkedPortal = nullptr; 
	float cooldownTimer = 0.0f;  

private:
	std::shared_ptr<SpriteSheet> portalSpritesheet;
};
