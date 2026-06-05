#pragma once
#include "Engine.h"

class Turret;

class Player : public GameObject
{
public:
	void Init(const EngineContext& engineContext) override;
	void LateInit(const EngineContext& engineContext) override;
	void Update(float dt, const EngineContext& engineContext) override;
	void Draw(const EngineContext& engineContext) override;
	void Free(const EngineContext& engineContext) override;
	void LateFree(const EngineContext& engineContext) override;

	int upKey, downKey, leftKey, rightKey;
	void SetControls(int up, int down, int left, int right);

	virtual void OnCollision(Object* other, const EngineContext& engineContext) override;

	//bullet player collision 
	void TakeDamage(int damageAmount);
	//item function
	void AddHealth(int amount);
	void AddAmmo(int amount);
	void IncreaseBulletSize();
private:
	glm::vec2 oldPos;
	
	std::shared_ptr<SpriteSheet> moveSpritesheetB;
	std::shared_ptr<SpriteSheet> moveSpritesheetR;

	TextObject* hpText; 
	Turret* myTurret = nullptr;
	int hp = 100;
};