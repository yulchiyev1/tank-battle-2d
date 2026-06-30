#include "MainMenu.h"
#include "Tutorial.h"

void MainMenu::Load(const EngineContext& engineContext)
{
    // close
    engineContext.renderManager->RegisterTexture("[Texture]BtnClose", "Textures/mainMenu/close.png");
    engineContext.renderManager->RegisterMaterial("[Material]BtnClose", "[EngineShader]default_texture", { {"u_Texture", "[Texture]BtnClose"} });
    //pop up info png
    engineContext.renderManager->RegisterTexture("[Texture]InfoPopUp", "Textures/mainMenu/infoPU.png");
    engineContext.renderManager->RegisterMaterial("[Material]InfoPopUp", "[EngineShader]default_texture", { {"u_Texture", "[Texture]InfoPopUp"} });

    //tanks on Main Menu
    engineContext.renderManager->RegisterTexture("[Texture]MenuTankBlue", "Textures/mainMenu/blue_blue.png");
    engineContext.renderManager->RegisterMaterial("[Material]MenuTankBlue", "[EngineShader]default_texture", { {"u_Texture", "[Texture]MenuTankBlue"} });
    engineContext.renderManager->RegisterTexture("[Texture]MenuTankRed", "Textures/mainMenu/red_red.png");
    engineContext.renderManager->RegisterMaterial("[Material]MenuTankRed", "[EngineShader]default_texture", { {"u_Texture", "[Texture]MenuTankRed"} });

    //sound off
    engineContext.renderManager->RegisterTexture("[Texture]SoundOff", "Textures/mainMenu/Sound_Off.png");
    engineContext.renderManager->RegisterMaterial("[Material]BtnSoundOff", "[EngineShader]default_texture", { {"u_Texture", "[Texture]BtnSoundOff"} });
    //sound
    engineContext.renderManager->RegisterTexture("[Texture]BtnSound", "Textures/mainMenu/Sound.png");
    engineContext.renderManager->RegisterMaterial("[Material]BtnSound", "[EngineShader]default_texture", { {"u_Texture", "[Texture]BtnSound"} });
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
    
    engineContext.soundManager->LoadSound("[Sound]MainMenuSound", "Sounds/main_menu.mp3", true);
}

void MainMenu::Init(const EngineContext& engineContext)
{
    //main manu bg sound
    engineContext.soundManager->Play("[Sound]MainMenuSound", 0.4f, 0.0f);

    // mouse curspr
    engineContext.windowManager->SetCursorVisible(false);
    cursor = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Cursor"));
    cursor->SetMesh(engineContext, "[EngineMesh]default");
    cursor->SetMaterial(engineContext, "[Material]Cursor");
    cursor->GetTransform2D().SetScale({ 30.0f, 30.0f });
    cursor->SetRenderLayer("[Layer]Cursor");

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

    // 2. SWORDS 
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
    btnStartBaseScale = glm::vec2(140.0f * baseScale, 94.0f * baseScale);
    btnStart->GetTransform2D().SetScale(btnStartBaseScale);
    btnStart->GetTransform2D().SetPosition(glm::vec2(0.0f, -250.0f));
    btnStart->SetRenderLayer("[Layer]UI");

    // 4. Info button
    btnInfo = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]BtnInfo"));
    btnInfo->SetMesh(engineContext, "[EngineMesh]default");
    btnInfo->SetMaterial(engineContext, "[Material]BtnInfo");
    btnInfoBaseScale = glm::vec2(140.0f * baseScale, 94.0f * baseScale);
    btnInfo->GetTransform2D().SetScale(btnInfoBaseScale);
    btnInfo->GetTransform2D().SetPosition(glm::vec2(-250.0f, -250.0f));
    btnInfo->SetRenderLayer("[Layer]UI");

    // 5. Maps button
    btnMaps = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]BtnMaps"));
    btnMaps->SetMesh(engineContext, "[EngineMesh]default");
    btnMaps->SetMaterial(engineContext, "[Material]BtnMaps");
    btnMapsBaseScale = glm::vec2(140.0f * baseScale, 94.0f * baseScale);
    btnMaps->GetTransform2D().SetScale(btnMapsBaseScale);
    btnMaps->GetTransform2D().SetPosition(glm::vec2(250.0f, -250.0f));
    btnMaps->SetRenderLayer("[Layer]UI");


    // 6. Exit button
    btnExit = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]BtnExit"));
    btnExit->SetMesh(engineContext, "[EngineMesh]default");
    btnExit->SetMaterial(engineContext, "[Material]BtnExit");
    btnExitBaseScale = glm::vec2(50.0f * baseScale, 50.0f * baseScale);
    btnExit->GetTransform2D().SetScale(btnExitBaseScale);
    btnExit->GetTransform2D().SetPosition(glm::vec2(600.0f, -320.0f));
    btnExit->SetRenderLayer("[Layer]UI");

    //7. Sound btn
    btnSound = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]BtnSound"));
    btnSound->SetMesh(engineContext, "[EngineMesh]default");
    btnSound->SetMaterial(engineContext, "[Material]BtnSound");
    btnSoundBaseScale = glm::vec2(50.0f * baseScale, 50.0f * baseScale);
    btnSound->GetTransform2D().SetScale(btnSoundBaseScale);
    btnSound->GetTransform2D().SetPosition(glm::vec2(-600.0f, -320.0f));
    btnSound->SetRenderLayer("[Layer]UI");

    // 8.Tanks
    float tankSize = 400.0f * baseScale;
    menuTankBlue = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]MenuTankBlue"));
    menuTankBlue->SetMesh(engineContext, "[EngineMesh]default");
    menuTankBlue->SetMaterial(engineContext, "[Material]MenuTankBlue");
    menuTankBlue->GetTransform2D().SetScale({ tankSize, 350.f * baseScale });
    float tankY = -10.0f;
    menuTankBlue->GetTransform2D().SetPosition({ -scrW, tankY });
    menuTankBlue->SetRenderLayer("[Layer]UI");

    menuTankRed = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]MenuTankRed"));
    menuTankRed->SetMesh(engineContext, "[EngineMesh]default");
    menuTankRed->SetMaterial(engineContext, "[Material]MenuTankRed");
    menuTankRed->GetTransform2D().SetScale({ tankSize, tankSize });
    menuTankRed->GetTransform2D().SetPosition({ scrW, tankY });
    menuTankRed->SetRenderLayer("[Layer]UI");
    tankTargetX_Blue = -scrW * 0.35f;
    tankTargetX_Red = scrW * 0.35f;


    // 9. Pop-UP Info
    InfoPopUp = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]InfoPopUp"));
    InfoPopUp->SetMesh(engineContext, "[EngineMesh]default");
    InfoPopUp->SetMaterial(engineContext, "[Material]InfoPopUp");
    InfoPopUp->GetTransform2D().SetScale({ 1152, 756 });
    InfoPopUp->GetTransform2D().SetPosition({ 0, 800 });
    InfoPopUp->SetRenderLayer("[Layer]POPUP");

    //10.Close button
    btnClose = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]BtnClose"));
    btnClose->SetMesh(engineContext, "[EngineMesh]default");
    btnClose->SetMaterial(engineContext, "[Material]BtnClose");
    btnCloseBaseScale = glm::vec2(50.0f * baseScale, 50.0f * baseScale);
    btnClose->GetTransform2D().SetScale(btnCloseBaseScale);
    btnClose->GetTransform2D().SetPosition(glm::vec2(450.0f, 290.0f));
    btnClose->SetRenderLayer("[Layer]Back");
}

void MainMenu::Update(float dt, const EngineContext& engineContext)
{
    objectManager.UpdateAll(dt, engineContext);

    // 0. mouse cursor
    if (cursor != nullptr)
    {
        float scrW = engineContext.windowManager->GetWidth();
        float scrH = engineContext.windowManager->GetHeight();
        float rawMouseX = static_cast<float>(engineContext.inputManager->GetMouseX());
        float rawMouseY = static_cast<float>(engineContext.inputManager->GetMouseY());
        float mouseX = rawMouseX - (scrW / 2.0f);
        float mouseY = (scrH / 2.0f) - rawMouseY;
        cursor->GetTransform2D().SetPosition({ mouseX + 15.0f, mouseY - 15.0f });
    }

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

    // 2. start button 
    if (btnStart != nullptr)
    {
        float scrW = engineContext.windowManager->GetWidth();
        float scrH = engineContext.windowManager->GetHeight();
        float rawMouseX = static_cast<float>(engineContext.inputManager->GetMouseX());
        float rawMouseY = static_cast<float>(engineContext.inputManager->GetMouseY());
        float mouseX = rawMouseX - (scrW / 2.0f);
        float mouseY = (scrH / 2.0f) - rawMouseY;
        glm::vec2 btnPos = btnStart->GetTransform2D().GetPosition();
        float halfW = btnStartBaseScale.x / 2.0f;
        float halfH = btnStartBaseScale.y / 2.0f;
        bool isHovering = (mouseX >= btnPos.x - halfW && mouseX <= btnPos.x + halfW &&
            mouseY >= btnPos.y - halfH && mouseY <= btnPos.y + halfH);

        if (isHovering)
        {
            btnStart->GetTransform2D().SetScale(btnStartBaseScale * 1.25f);
            if (engineContext.inputManager->IsMouseButtonPressed(0))
            {
                JIN_LOG("[MainMenu] START tugmasi bosildi!");
                engineContext.stateManager->ChangeState(std::make_unique<Tutorial>());
            }
        }
        else
        {
            btnStart->GetTransform2D().SetScale(btnStartBaseScale);
        }
    }

    // 3.tanks
    if (menuTankBlue != nullptr && menuTankRed != nullptr)
    {
        float moveSpeed = 400.0f * dt;
        glm::vec2 bluePos = menuTankBlue->GetTransform2D().GetPosition();
        glm::vec2 redPos = menuTankRed->GetTransform2D().GetPosition();
        if (bluePos.x < tankTargetX_Blue)
        {
            bluePos.x += moveSpeed;
            // Agar targetdan o'tib ketsa, o'z joyiga qotirib qo'yamiz
            if (bluePos.x > tankTargetX_Blue) bluePos.x = tankTargetX_Blue;
            menuTankBlue->GetTransform2D().SetPosition(bluePos);
        }
        if (redPos.x > tankTargetX_Red)
        {
            redPos.x -= moveSpeed;
            if (redPos.x < tankTargetX_Red) redPos.x = tankTargetX_Red;
            menuTankRed->GetTransform2D().SetPosition(redPos);
        }
    }

    //  4. exit button
    if (btnExit != nullptr)
    {
        float scrW = engineContext.windowManager->GetWidth();
        float scrH = engineContext.windowManager->GetHeight();
        float rawMouseX = static_cast<float>(engineContext.inputManager->GetMouseX());
        float rawMouseY = static_cast<float>(engineContext.inputManager->GetMouseY());
        float mouseX = rawMouseX - (scrW / 2.0f);
        float mouseY = (scrH / 2.0f) - rawMouseY;
        glm::vec2 btnPos = btnExit->GetTransform2D().GetPosition();
        float halfW = btnExitBaseScale.x / 2.0f;
        float halfH = btnExitBaseScale.y / 2.0f;
        bool isHovering = (mouseX >= btnPos.x - halfW && mouseX <= btnPos.x + halfW &&
            mouseY >= btnPos.y - halfH && mouseY <= btnPos.y + halfH);

        if (isHovering)
        {
            btnExit->GetTransform2D().SetScale(btnExitBaseScale * 1.25f);
            if (engineContext.inputManager->IsMouseButtonPressed(0))
            {
                engineContext.engine->RequestQuit();
            }
        }
        else
        {
            btnExit->GetTransform2D().SetScale(btnExitBaseScale);
        }
    }

    // 5. info button 
    if (btnInfo != nullptr)
    {
        float scrW = engineContext.windowManager->GetWidth();
        float scrH = engineContext.windowManager->GetHeight();
        float rawMouseX = static_cast<float>(engineContext.inputManager->GetMouseX());
        float rawMouseY = static_cast<float>(engineContext.inputManager->GetMouseY());
        float mouseX = rawMouseX - (scrW / 2.0f);
        float mouseY = (scrH / 2.0f) - rawMouseY;
        glm::vec2 btnPosInfo = btnInfo->GetTransform2D().GetPosition();
        float halfW = btnInfoBaseScale.x / 2.0f;
        float halfH = btnInfoBaseScale.y / 2.0f;
        bool isHovering = (mouseX >= btnPosInfo.x - halfW && mouseX <= btnPosInfo.x + halfW &&
            mouseY >= btnPosInfo.y - halfH && mouseY <= btnPosInfo.y + halfH);

        if (isHovering)
        {
            btnInfo->GetTransform2D().SetScale(btnInfoBaseScale * 1.25f);
            if (engineContext.inputManager->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                infoStatus = 1.0f;
            }
        }
        else
        {
            btnInfo->GetTransform2D().SetScale(btnInfoBaseScale);
        }
        if (infoStatus == 1.0f) {
            if (InfoPopUp != nullptr)
            {
                float moveSpeed = 1800.0f * dt;
                glm::vec2 infoPos = InfoPopUp->GetTransform2D().GetPosition();

                if (infoPos.y > 0)
                {
                    infoPos.y -= moveSpeed;

                    if (infoPos.y < 0) infoPos.y = 0;
                    InfoPopUp->GetTransform2D().SetPosition(infoPos);
                }

                if (infoPos.y <= 0) {
                    infoStatus = 0.0f;
                    btnClose->SetRenderLayer("[Layer]HPUI");
                    JIN_LOG("CLOSe oldinda");
                }
            }
        }
    }
    //  6. close button
    if (btnClose != nullptr)
    {
        float scrW = engineContext.windowManager->GetWidth();
        float scrH = engineContext.windowManager->GetHeight();
        float rawMouseX = static_cast<float>(engineContext.inputManager->GetMouseX());
        float rawMouseY = static_cast<float>(engineContext.inputManager->GetMouseY());
        float mouseX = rawMouseX - (scrW / 2.0f);
        float mouseY = (scrH / 2.0f) - rawMouseY;
        glm::vec2 btnPos = btnClose->GetTransform2D().GetPosition();
        float halfW = btnCloseBaseScale.x / 2.0f;
        float halfH = btnCloseBaseScale.y / 2.0f;
        bool isHovering = (mouseX >= btnPos.x - halfW && mouseX <= btnPos.x + halfW &&
            mouseY >= btnPos.y - halfH && mouseY <= btnPos.y + halfH);

        if (isHovering)
        {
            btnClose->GetTransform2D().SetScale(btnCloseBaseScale * 1.25f);
            if (engineContext.inputManager->IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                //info pop up 위로 가도록
                if (infoStatus == 0.0f) {
                    if (InfoPopUp != nullptr)
                    {
                        glm::vec2 infoPos = InfoPopUp->GetTransform2D().GetPosition();

                        if (infoPos.y < 800)
                        {
                            infoPos.y += 810;

                            if (infoPos.y >= 800)
                            {
                                infoPos.y = 800;
                                btnClose->SetRenderLayer("[Layer]Back");
                                JIN_LOG("Close button orqaga otti");
                            }
                            InfoPopUp->GetTransform2D().SetPosition(infoPos);
                        }
                    }
                }
            }
        }
        else
        {
            btnClose->GetTransform2D().SetScale(btnCloseBaseScale);
        }
    }
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