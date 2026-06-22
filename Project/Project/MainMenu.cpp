#include "MainMenu.h"
#include "Tutorial.h"

void MainMenu::Load(const EngineContext& engineContext)
{
    //info
    engineContext.renderManager->RegisterTexture("[Texture]BtnInfo", "Textures/mainMenu/info.png");
    engineContext.renderManager->RegisterMaterial("[Material]BtnInfo", "[EngineShader]default_texture", { {"u_Texture", "[Texture]BtnInfo"} });
    // maps
    engineContext.renderManager->RegisterTexture("[Texture]BtnMaps", "Textures/mainMenu/maps.png");
    engineContext.renderManager->RegisterMaterial("[Material]BtnMaps", "[EngineShader]default_texture", { {"u_Texture", "[Texture]BtnMaps"} });
    // exit
    engineContext.renderManager->RegisterTexture("[Texture]BtnExit", "Textures/mainMenu/exit.png");
    engineContext.renderManager->RegisterMaterial("[Material]BtnExit", "[EngineShader]default_texture", { {"u_Texture", "[Texture]BtnExit"} });
    //start 
    engineContext.renderManager->RegisterTexture("[Texture]BtnStart", "Textures/mainMenu/start.png");
    engineContext.renderManager->RegisterMaterial("[Material]BtnStart", "[EngineShader]default_texture", { {"u_Texture", "[Texture]BtnStart"} });
    //swords
    engineContext.renderManager->RegisterTexture("[Texture]Swords", "Textures/mainMenu/swords.png");
    engineContext.renderManager->RegisterMaterial("[Material]Swords", "[EngineShader]default_texture", { {"u_Texture", "[Texture]Swords"} });
    //bg
    engineContext.renderManager->RegisterTexture("[Texture]MenuBG", "Textures/mainMenu/menu_bg.png");
    engineContext.renderManager->RegisterMaterial("[Material]MenuBG", "[EngineShader]default_texture", { {"u_Texture", "[Texture]MenuBG"} });
}

void MainMenu::Init(const EngineContext& engineContext)
{
    // screen W&H
    float scrW = engineContext.windowManager->GetWidth();
    float scrH = engineContext.windowManager->GetHeight();

    // 1. BACKGROUND
    GameObject* background = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]MenuBackground"));
    background->SetMesh(engineContext, "[EngineMesh]default");
    background->SetMaterial(engineContext, "[Material]MenuBG");
    float imgW = 1536.0f;
    float imgH = 1024.0f;
    float scaleX = scrW / imgW;
    float scaleY = scrH / imgH;
    float bgScale = std::max(scaleX, scaleY);
    background->GetTransform2D().SetScale({ imgW * bgScale, imgH * bgScale });
    background->GetTransform2D().SetPosition({ 0.0f, 0.0f });
    background->SetRenderLayer("[Layer]Background");

    // 2. SWORDS (XATO TO'G'RILANDI: "GameObject*" olib tashlandi)
    float baseScale = scrW / 1000.0f;
    swords = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Swords"));
    swords->SetMesh(engineContext, "[EngineMesh]default");
    swords->SetMaterial(engineContext, "[Material]Swords");

    float swordSize = 140.0f * baseScale;
    swords->GetTransform2D().SetScale(glm::vec2(swordSize, swordSize));

    swordsBaseY = 20.0f;
    swords->GetTransform2D().SetPosition(glm::vec2(0.0f, swordsBaseY));
    swords->SetRenderLayer("[Layer]UI");

    // 3. START button
    btnStart = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]BtnStart"));
    btnStart->SetMesh(engineContext, "[EngineMesh]default");
    btnStart->SetMaterial(engineContext, "[Material]BtnStart");
    btnStartBaseScale = glm::vec2(133.0f * baseScale, 90.0f * baseScale);
    btnStart->GetTransform2D().SetScale(btnStartBaseScale);
    btnStart->GetTransform2D().SetPosition(glm::vec2(0.0f, -250.0f));
    btnStart->SetRenderLayer("[Layer]UI");

    // 4. Info button
    btnInfo = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]BtnInfo"));
    btnInfo->SetMesh(engineContext, "[EngineMesh]default");
    btnInfo->SetMaterial(engineContext, "[Material]BtnInfo");
    btnInfoBaseScale = glm::vec2(133.0f * baseScale, 90.0f * baseScale);
    btnInfo->GetTransform2D().SetScale(btnStartBaseScale);
    btnInfo->GetTransform2D().SetPosition(glm::vec2(-250.0f, -250.0f));
    btnInfo->SetRenderLayer("[Layer]UI");

    // 5. Maps button
    btnMaps = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]BtnMaps"));
    btnMaps->SetMesh(engineContext, "[EngineMesh]default");
    btnMaps->SetMaterial(engineContext, "[Material]BtnMaps");
    btnMapsBaseScale = glm::vec2(133.0f * baseScale, 90.0f * baseScale);
    btnMaps->GetTransform2D().SetScale(btnStartBaseScale);
    btnMaps->GetTransform2D().SetPosition(glm::vec2(250.0f, -250.0f));
    btnMaps->SetRenderLayer("[Layer]UI");

    // 6. Exit button
    btnExit = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]BtnExit"));
    btnExit->SetMesh(engineContext, "[EngineMesh]default");
    btnExit->SetMaterial(engineContext, "[Material]BtnExit");
    btnExitBaseScale = glm::vec2(60.0f * baseScale, 60.0f * baseScale);
    btnExit->GetTransform2D().SetScale(btnStartBaseScale);
    btnExit->GetTransform2D().SetPosition(glm::vec2(550.0f, -350.0f));
    btnExit->SetRenderLayer("[Layer]UI");
}

void MainMenu::Update(float dt, const EngineContext& engineContext)
{
    objectManager.UpdateAll(dt, engineContext);

    // 1. sword animation
    if (swords != nullptr)
    {
        hoverTime += dt;
        float hoverSpeed = 3.0f;
        float hoverAmplitude = 15.0f;
        float newY = swordsBaseY + std::sin(hoverTime * hoverSpeed) * hoverAmplitude;
        glm::vec2 currentPos = swords->GetTransform2D().GetPosition();
        swords->GetTransform2D().SetPosition(glm::vec2(currentPos.x, newY));
    }

    // 2. start button click 
    ///
    //
    
    
}

void MainMenu::Draw(const EngineContext& engineContext)
{
    objectManager.DrawAll(engineContext);
}

void MainMenu::LateInit(const EngineContext& engineContext) {}
void MainMenu::LateUpdate(float dt, const EngineContext& engineContext) {}
void MainMenu::PostProcessing(const EngineContext& engineContext) {}

void MainMenu::Free(const EngineContext& engineContext)
{
    JIN_LOG("[MainMenu] free called");
}

void MainMenu::Unload(const EngineContext& engineContext)
{
    JIN_LOG("[MainMenu] unload called");
}