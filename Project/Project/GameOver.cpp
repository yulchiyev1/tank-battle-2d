#include "GameOver.h"
#include "MainMenu.h"
#include "Tutorial.h"

void GameOver::Load(const EngineContext& engineContext)
{
    // podium bg
    engineContext.renderManager->RegisterTexture("[Texture]PodiumBG", "Textures/gameOver/podium.png");
    engineContext.renderManager->RegisterMaterial("[Material]PodiumBG", "[EngineShader]default_texture", { {"u_texture", "[Texture]PodiumBG"} });

    // tanks
    engineContext.renderManager->RegisterTexture("[Texture]MenuTankBlue", "Textures/mainMenu/blue_blue.png");
    engineContext.renderManager->RegisterMaterial("[Material]MenuTankBlue", "[EngineShader]default_texture", { {"u_Texture", "[Texture]MenuTankBlue"} });
    engineContext.renderManager->RegisterTexture("[Texture]MenuTankRed", "Textures/mainMenu/red_red.png");
    engineContext.renderManager->RegisterMaterial("[Material]MenuTankRed", "[EngineShader]default_texture", { {"u_Texture", "[Texture]MenuTankRed"} });

    // exit
    engineContext.renderManager->RegisterTexture("[Texture]BtnHome", "Textures/gameOver/home.png");
    engineContext.renderManager->RegisterMaterial("[Material]BtnHome", "[EngineShader]default_texture", { {"u_Texture", "[Texture]BtnHome"} });

    // replay 
    engineContext.renderManager->RegisterTexture("[Texture]BtnReplay", "Textures/gameOver/replay.png");
    engineContext.renderManager->RegisterMaterial("[Material]BtnReplay", "[EngineShader]default_texture", { {"u_Texture", "[Texture]BtnReplay"} });

    // bg
    engineContext.renderManager->RegisterTexture("[Texture]OverBG", "Textures/gameOver/gameOver_bg.png");
    engineContext.renderManager->RegisterMaterial("[Material]OverBG", "[EngineShader]default_texture", { {"u_Texture", "[Texture]OverBG"} });
}

void GameOver::Init(const EngineContext& engineContext)
{
    // mouse cursor
    engineContext.windowManager->SetCursorVisible(false);
    cursor = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Cursor"));
    cursor->SetMesh(engineContext, "[EngineMesh]default");
    cursor->SetMaterial(engineContext, "[Material]Cursor");
    cursor->GetTransform2D().SetScale({ 30.0f, 30.0f });
    cursor->SetRenderLayer("[Layer]Cursor");

    // screen W&H
    float scrW = engineContext.windowManager->GetWidth();
    float scrH = engineContext.windowManager->GetHeight();

    // podium background
    GameObject* podium = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Podium"));
    podium->SetMesh(engineContext, "[EngineMesh]default");
    podium->SetMaterial(engineContext, "[Material]PodiumBG");
    podium->GetTransform2D().SetScale({ 700.0f, 400.0f });
    podium->GetTransform2D().SetPosition({ 0.0f, -130.0f });
    podium->SetRenderLayer("[Layer]UI");

    // background 
    GameObject* background = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]OverBackground"));
    background->SetMesh(engineContext, "[EngineMesh]default");
    background->SetMaterial(engineContext, "[Material]OverBG");
    float imgW = 1536.0f;
    float imgH = 1024.0f;
    float scaleX = scrW / imgW;
    float scaleY = scrH / imgH;
    float bgScale = std::max(scaleX, scaleY);
    background->GetTransform2D().SetScale({ imgW * bgScale, imgH * bgScale });
    background->GetTransform2D().SetPosition({ 0.0f, 0.0f });
    background->SetRenderLayer("[Layer]Background");

    winnerTank = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Winner"));
    winnerTank->SetMesh(engineContext, "[EngineMesh]default");
    winnerTank->SetRenderLayer("[Layer]HPUI");

    loserTank = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Loser"));
    loserTank->SetMesh(engineContext, "[EngineMesh]default");
    loserTank->SetRenderLayer("[Layer]HPUI");

    // w/l & draw init
    if (winnerID == 1) // blue wins
    {
        winnerTank->SetMaterial(engineContext, "[Material]MenuTankBlue");
        winnerTank->GetTransform2D().SetPosition({ -1000.0f, 55.0f }); // right
        winnerTank->GetTransform2D().SetScale({ 400.0f, 350.0f });

        loserTank->SetMaterial(engineContext, "[Material]MenuTankRed");
        loserTank->GetTransform2D().SetPosition({ scrW / 2.0f - 150.0f, -scrH / 2.0f + 150.0f });
        loserTank->GetTransform2D().SetScale({ 60.0f, 60.0f });
        loserTank->GetTransform2D().SetRotation(glm::radians(45.0f));
    }
    else if (winnerID == 2) // red wins
    {
        winnerTank->SetMaterial(engineContext, "[Material]MenuTankRed");
        winnerTank->GetTransform2D().SetPosition({ 1000.0f, 55.0f }); // left
        winnerTank->GetTransform2D().SetScale({ 400.0f, 350.0f });

        loserTank->SetMaterial(engineContext, "[Material]MenuTankBlue");
        loserTank->GetTransform2D().SetPosition({ scrW / 2.0f - 150.0f, -scrH / 2.0f + 150.0f });
        loserTank->GetTransform2D().SetScale({ 60.0f, 60.0f });
        loserTank->GetTransform2D().SetRotation(glm::radians(45.0f));
    }
    else // draw
    {
        winnerTank->SetMaterial(engineContext, "[Material]MenuTankBlue");
        winnerTank->GetTransform2D().SetPosition({ 1000.0f, 15.0f }); // blue from right
        winnerTank->GetTransform2D().SetScale({ 350.0f, 250.0f }); // smaller for draw

        loserTank->SetMaterial(engineContext, "[Material]MenuTankRed");
        loserTank->GetTransform2D().SetPosition({ -1000.0f, 15.0f }); // red from left
        loserTank->GetTransform2D().SetScale({ 300.0f, 250.0f }); // smaller for draw
        loserTank->GetTransform2D().SetRotation(0.0f); // reset rotation
    }

    // REPLAY button
    float baseScale = scrW / 1000.0f;
    btnReplay = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]btnReplay"));
    btnReplay->SetMesh(engineContext, "[EngineMesh]default");
    btnReplay->SetMaterial(engineContext, "[Material]BtnReplay");
    btnReplayBaseScale = glm::vec2(140.0f * baseScale, 94.0f * baseScale);
    btnReplay->GetTransform2D().SetScale(btnReplayBaseScale);
    btnReplay->GetTransform2D().SetPosition(glm::vec2(0.0f, -275.0f));
    btnReplay->SetRenderLayer("[Layer]UI");

    // Home button
    BtnHome = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]BtnHome"));
    BtnHome->SetMesh(engineContext, "[EngineMesh]default");
    BtnHome->SetMaterial(engineContext, "[Material]BtnHome");
    BtnHomeBaseScale = glm::vec2(80.0f * baseScale, 60.0f * baseScale);
    BtnHome->GetTransform2D().SetScale(BtnHomeBaseScale);
    BtnHome->GetTransform2D().SetPosition(glm::vec2(600.0f, -320.0f));
    BtnHome->SetRenderLayer("[Layer]UI");
}

void GameOver::Update(float dt, const EngineContext& engineContext)
{
    objectManager.UpdateAll(dt, engineContext);

    // 1. mouse cursor 
    float scrW = engineContext.windowManager->GetWidth();
    float scrH = engineContext.windowManager->GetHeight();
    float rawMouseX = static_cast<float>(engineContext.inputManager->GetMouseX());
    float rawMouseY = static_cast<float>(engineContext.inputManager->GetMouseY());
    float mouseX = rawMouseX - (scrW / 2.0f);
    float mouseY = (scrH / 2.0f) - rawMouseY;
    if (cursor != nullptr){cursor->GetTransform2D().SetPosition({ mouseX + 15.0f, mouseY - 15.0f });}

    bool isClicking = engineContext.inputManager->IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    // 2. tanks anim
    glm::vec2 winPos = winnerTank->GetTransform2D().GetPosition();
    float centerOffset = (winnerID == 0) ? 150.0f : 0.0f; // offset for draw

    // winner (or blue in draw) anim
    if (winPos.x > centerOffset) // from right
    {
        winPos.x -= 1000.0f * dt;
        if (winPos.x < centerOffset) winPos.x = centerOffset; // stop at center
        winnerTank->GetTransform2D().SetPosition(winPos);
    }
    else if (winPos.x < -centerOffset) // from left
    {
        winPos.x += 1000.0f * dt;
        if (winPos.x > -centerOffset) winPos.x = -centerOffset; // stop at center
        winnerTank->GetTransform2D().SetPosition(winPos);
    }

    // loser (or red in draw) anim
    // loser anim (if not draw)
    if (winnerID != 0)
    {
        static float time = 0.0f;
        time += dt;
        loserTank->GetTransform2D().SetRotation(time * 6.0f);
        float s = 70.0f + std::sin(time * 3.0f) * 10.0f; //pulse scale
        loserTank->GetTransform2D().SetScale({ s, s });
        float x = -800.0f + std::fmod(time * 150.0f, 3600.0f); 
        float y = 250.0f + std::sin(time * 5.0f) * 20.0f;      
        loserTank->GetTransform2D().SetPosition({ x, y });
    }
    else
    {
        // draw: move red to center
        glm::vec2 losePos = loserTank->GetTransform2D().GetPosition();
        if (losePos.x < -centerOffset) // from left
        {
            losePos.x += 1000.0f * dt;
            if (losePos.x > -centerOffset) losePos.x = -centerOffset; // stop next to blue
            loserTank->GetTransform2D().SetPosition(losePos);
        }
    }

    // 3. buttons hover & click
    // replay btn
    glm::vec2 rplPos = btnReplay->GetTransform2D().GetPosition();
    if (mouseX > rplPos.x - 70.0f && mouseX < rplPos.x + 70.0f && mouseY > rplPos.y - 45.0f && mouseY < rplPos.y + 45.0f)
    {
        btnReplay->GetTransform2D().SetScale(btnReplayBaseScale * 1.2f); // zoom in
        if (isClicking) engineContext.stateManager->ChangeState(std::make_unique<Tutorial>());
    }
    else
    {
        btnReplay->GetTransform2D().SetScale(btnReplayBaseScale);
    }

    // exit btn
    glm::vec2 extPos = BtnHome->GetTransform2D().GetPosition();
    if (mouseX > extPos.x - 25.0f && mouseX < extPos.x + 25.0f && mouseY > extPos.y - 25.0f && mouseY < extPos.y + 25.0f)
    {
        BtnHome->GetTransform2D().SetScale(BtnHomeBaseScale * 1.2f);
        if (isClicking) engineContext.stateManager->ChangeState(std::make_unique<MainMenu>());
    }
    else
    {
        BtnHome->GetTransform2D().SetScale(BtnHomeBaseScale);
    }

    // space key > replay
    if (engineContext.inputManager->IsKeyPressed(KEY_SPACE))
    {
        engineContext.stateManager->ChangeState(std::make_unique<Tutorial>());
    }
}

void GameOver::Draw(const EngineContext& engineContext)
{
    objectManager.DrawAll(engineContext);
}