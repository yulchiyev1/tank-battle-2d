#pragma once
#include "Engine.h"

enum class ItemType
{
	HEALTH,
	AMMO,
	SIZE,
	FLASH
};

class Item : public GameObject
{
public:
	void Init(const EngineContext& engineContext) override;
	void LateInit(const EngineContext& engineContext) override;
	void Update(float dt, const EngineContext& engineContext) override;
	void Draw(const EngineContext& engineContext) override;
	void Free(const EngineContext& engineContext) override;
	void LateFree(const EngineContext& engineContext) override;
	void OnCollision(Object* other, const EngineContext& engineContext) override;


	bool IsUnlocked() const { return isUnlocked; }

private:
	ItemType currentType = ItemType::HEALTH;
	bool isUnlocked = false;

	//breathin effect
	glm::vec2 baseScale;         // origin size of box
	float currentScaleOffset = 0.0f; 
	float breathSpeed = 0.8f;   
	bool isGrowing = true;
	// somke effect
	float smokeTimer = 0.0f;
	bool isSmoking = false;
};
