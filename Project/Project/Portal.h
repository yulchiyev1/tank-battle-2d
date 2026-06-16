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

	Portal* linkedPortal = nullptr; // Ikkinchi portalni eslab qolish uchun
	float cooldownTimer = 0.0f;     // Pauza vaqti

private:
	std::shared_ptr<SpriteSheet> portalSpritesheet;
};
