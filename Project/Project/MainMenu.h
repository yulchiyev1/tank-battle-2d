#pragma once
#include "GameState.h"
#include "TextObject.h"

class MainMenu : public GameState
{
public:
    virtual void Load(const EngineContext& engineContext) override;
    virtual void Init(const EngineContext& engineContext) override;
    virtual void LateInit(const EngineContext& engineContext) override;
    virtual void Update(float dt, const EngineContext& engineContext) override;
    virtual void LateUpdate(float dt, const EngineContext& engineContext) override;
    virtual void Draw(const EngineContext& engineContext) override;
    virtual void PostProcessing(const EngineContext& engineContext) override;
    virtual void Free(const EngineContext& engineContext) override;
    virtual void Unload(const EngineContext& engineContext) override;
private:
    //cursor
    GameObject* cursor = nullptr;

    //swords anim
    GameObject* swords = nullptr;
    float hoverTime = 0.0f;
    float swordsBaseY = 0.0f;

    // start button
    GameObject* btnStart = nullptr;
    glm::vec2 btnStartBaseScale;

    //info button
    GameObject* btnInfo = nullptr;
    glm::vec2 btnInfoBaseScale;

    // maps btn
    GameObject* btnMaps = nullptr;
    glm::vec2 btnMapsBaseScale;

    // exit btn
    GameObject* btnExit = nullptr;
    glm::vec2 btnExitBaseScale;

    //close bn
    GameObject* btnClose = nullptr;
    glm::vec2 btnCloseBaseScale;

    // sound btn
    GameObject* btnSound = nullptr;
    glm::vec2 btnSoundBaseScale;

    // main menu tanks
    GameObject* menuTankBlue = nullptr;
    GameObject* menuTankRed = nullptr;
    float tankTargetX_Blue = 0.0f;
    float tankTargetX_Red = 0.0f;

    //Info popup
    GameObject* InfoPopUp = nullptr;
    float infoStatus = 0.0f;

};