#include "Tutorial.h"
#include <random>
#include "LoadingState.h"
#include "Item.h"
#include "Player.h"
#include "GameOver.h" 

void Tutorial::Load(const EngineContext& engineContext)
{
    JIN_LOG("[Tutorial] load called");
    RenderManager* rm = engineContext.renderManager;

    //texture & material load
    rm->RegisterTexture("[Texture]ItemBox", "Textures/Items/item.png");
    rm->RegisterMaterial("[Material]ItemBox", "[EngineShader]default_texture", { {"u_Texture", "[Texture]ItemBox"} });

    rm->RegisterTexture("[Texture]Item2X", "Textures/Items/2x.png");
    rm->RegisterMaterial("[Material]Item2X", "[EngineShader]default_texture", { {"u_Texture", "[Texture]Item2X"} });

    rm->RegisterTexture("[Texture]ItemHealth", "Textures/Items/hp_item1.png");
    rm->RegisterMaterial("[Material]ItemHealth", "[EngineShader]default_texture", { {"u_Texture", "[Texture]ItemHealth"} });

    rm->RegisterTexture("[Texture]ItemAmmo", "Textures/Items/bullets_item.png");
    rm->RegisterMaterial("[Material]ItemAmmo", "[EngineShader]default_texture", { {"u_Texture", "[Texture]ItemAmmo"} });

    rm->RegisterTexture("[Texture]ItemSize", "Textures/Items/bullet_item1.png");
    rm->RegisterMaterial("[Material]ItemSize", "[EngineShader]default_texture", { {"u_Texture", "[Texture]ItemSize"} });

    rm->RegisterTexture("[Texture]TurretBlue", "Textures/Tanks/blue/turret_blue.png");
    rm->RegisterSpriteSheet("[SpriteSheet]TurretBlue", "[Texture]TurretBlue", 128, 128);

    rm->RegisterTexture("[Texture]TurretRed", "Textures/Tanks/red/turret_red.png");
    rm->RegisterSpriteSheet("[SpriteSheet]TurretRed", "[Texture]TurretRed", 128, 128);

    rm->RegisterTexture("[Texture]TurretR", "Textures/Tanks/red/turretR.png");
    rm->RegisterMaterial("[Material]TurretR", "[EngineShader]default_texture", { {"u_Texture", "[Texture]TurretR"} });

    rm->RegisterTexture("[Texture]TurretB", "Textures/Tanks/blue/turretB.png");
    rm->RegisterMaterial("[Material]TurretB", "[EngineShader]default_texture", { {"u_Texture", "[Texture]TurretB"} });


    rm->RegisterTexture("[Texture]BlueTank", "Textures/Tanks/blue/body_halftrack.png");
    rm->RegisterSpriteSheet("[SpriteSheet]BlueTank", "[Texture]BlueTank", 128, 128);

    rm->RegisterTexture("[Texture]RedTank", "Textures/Tanks/red/body_halftrack_r.png");
    rm->RegisterSpriteSheet("[SpriteSheet]RedTank", "[Texture]RedTank", 128, 128);

    rm->RegisterTexture("[Texture]Background", "Textures/Background/_09_background.png");
    rm->RegisterMaterial("[Material]Background", "[EngineShader]default_texture", { {"u_Texture", "[Texture]Background"} });

    rm->RegisterTexture("[Texture]Tank1", "Textures/Tanks/blue/body_tracks.png");
    rm->RegisterMaterial("[Material]Tank1", "[EngineShader]default_texture", { {"u_Texture", "[Texture]Tank1"} });
    rm->RegisterTexture("[Texture]Tank2", "Textures/Tanks/red/body_tracks_r.png");
    rm->RegisterMaterial("[Material]Tank2", "[EngineShader]default_texture", { {"u_Texture", "[Texture]Tank2"} });

    rm->RegisterTexture("[Texture]StoneWall", "Textures/Wall/stone_wall.jpg");
    rm->RegisterMaterial("[Material]StoneWall", "[EngineShader]default_texture", { {"u_Texture", "[Texture]StoneWall"} });

    rm->RegisterTexture("[Texture]Bullet", "Textures/Projectile/bullet2.png");
    rm->RegisterMaterial("[Material]Bullet", "[EngineShader]default_texture", { {"u_Texture", "[Texture]Bullet"} });
}

void Tutorial::Init(const EngineContext& engineContext)
{
    JIN_LOG("[Tutorial] init called");
    screenW = engineContext.windowManager->GetWidth();
    screenH = engineContext.windowManager->GetHeight();

    // Background
    background = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Background"));
    background->SetMesh(engineContext, "[EngineMesh]default");
    background->SetMaterial(engineContext, "[Material]Background");
    int background_width = screenW;
    background->GetTransform2D().SetScale({ background_width, background_width });
    background->SetRenderLayer("[Layer]Background");

    // Controls of Tanks
    // Tank1 (WASD)
    player1 = static_cast<Player*>(objectManager.AddObject(std::make_unique<Player>(), "[Object]Player1"));
    player1->SetControls(KEY_W, KEY_S, KEY_A, KEY_D);
    // Tank2 
    player2 = static_cast<Player*>(objectManager.AddObject(std::make_unique<Player>(), "[Object]Player2"));
    player2->SetControls(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);

    // border wallblocks without collider
    float wall_len = screenW * 0.02f;
    float right_x = screenW / 2.f - wall_len / 2.f;
    float left_x = -right_x;
    float top_y = screenH / 2.f - wall_len / 2.f;
    float bottom_y = -top_y;

    for (float x = left_x; x <= right_x; x += wall_len) // T&B
    {
        WallBlock(x, top_y, wall_len, engineContext, true);    // 'true' = no collider
        WallBlock(x, bottom_y, wall_len, engineContext, true);
    }
    for (float y = bottom_y; y <= top_y; y += wall_len) // L&R
    {
        WallBlock(left_x, y, wall_len, engineContext, true);
        WallBlock(right_x, y, wall_len, engineContext, true);
    }

    // 4 border colliders  
    auto addInvisibleCollider = [&](float x, float y, float width, float height, std::string tag) {
        GameObject* invisibleWall = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), tag));
        invisibleWall->GetTransform2D().SetPosition(glm::vec2(x, y));
        invisibleWall->SetCollider(std::make_unique<AABBCollider>(invisibleWall, glm::vec2{ width, height }));
        invisibleWall->GetCollider()->SetUseTransformScale(false);
        invisibleWall->SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), tag, { "[Object]Player1", "[Object]Player2", "[Object]Bullet" });
        };
    // T&B
    addInvisibleCollider(0.0f, top_y, screenW, wall_len, "[Object]Border_H");
    addInvisibleCollider(0.0f, bottom_y, screenW, wall_len, "[Object]Border_H");
    // L&R
    addInvisibleCollider(left_x, 0.0f, wall_len, screenH, "[Object]Border_V");
    addInvisibleCollider(right_x, 0.0f, wall_len, screenH, "[Object]Border_V");

    // 경기장 안 random wall blocks
    std::random_device rd;
    std::mt19937 gen(rd());

    // Chegaralarga (Border) tegib qolmasligi uchun maydonni biroz qisqartirdik
    std::uniform_real_distribution<float> randX(-450.0f, 450.0f);
    std::uniform_real_distribution<float> randY(-250.0f, 250.0f);

    int wallsSpawned = 0;
    wallPositions.clear();

    while (wallsSpawned < 8)
    {
        glm::vec2 testPos(randX(gen), randY(gen));

        // at least 100p distance
        if (IsSafePosition(testPos, 100.0f))
        {
            WallBlock(testPos.x, testPos.y, wall_len, engineContext, false);
            wallPositions.push_back(testPos); 
            wallsSpawned++;
        }
    }

    // one box to center for testing (X: 0, Y: 0) 
    Item* testItem = new Item();
    testItem->Init(engineContext);
    testItem->GetTransform2D().SetPosition(glm::vec2(0.0f, 0.0f));
    objectManager.AddObject(std::unique_ptr<Object>(testItem), "[Object]Item");

    // timer of the game
    if (timerTextObj == nullptr) {
        timerTextObj = static_cast<TextObject*>(engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(
            std::make_unique<TextObject>(engineContext.renderManager->GetFontByTag("[Font]defaultkr"), "02:00"),
            "[Object]TimerText"
        ));
    }
    timerTextObj->SetRenderLayer("[Layer]UI"); // Har doim ustda turishi uchun
    timerTextObj->GetTransform2D().SetScale(glm::vec2(0.6f, 0.6f));
}

void Tutorial::LateInit(const EngineContext& engineContext)
{
}

void Tutorial::Update(float dt, const EngineContext& engineContext)
{
    objectManager.UpdateAll(dt, engineContext);
   
    //camera
    Camera2D* mainCam = cameraManager.GetActiveCamera();
    if (mainCam != nullptr && player1 != nullptr && player2 != nullptr)
    {
        glm::vec2 p1Pos = player1->GetTransform2D().GetPosition();
        glm::vec2 p2Pos = player2->GetTransform2D().GetPosition();

        glm::vec2 targetPos = (p1Pos + p2Pos) * 0.5f;
        float distance = glm::distance(p1Pos, p2Pos);

        float minDist = 450.0f;
        float maxDist = 2050.0f;
        float clampedDist = std::clamp(distance, minDist, maxDist);
        float t = (clampedDist - minDist) / (maxDist - minDist);
        float targetZoom = glm::mix(1.2f, 0.65f, t);

        float panSpeed = 6.0f;  //kamera siljish tezligi
        float zoomSpeed = 1.0f; //ekran kattlashish/kichiklashish tezligi

        float panFactor = 1.0f - std::exp(-panSpeed * dt);
        float zoomFactor = 1.0f - std::exp(-zoomSpeed * dt);

        glm::vec2 currentPos = mainCam->GetPosition();
        float currentZoom = mainCam->GetZoom();

        glm::vec2 smoothPos = glm::mix(currentPos, targetPos, panFactor);
        float smoothZoom = glm::mix(currentZoom, targetZoom, zoomFactor);

      
        if (glm::distance(smoothPos, targetPos) < 0.5f) {
            smoothPos = targetPos;
        }
        if (std::abs(smoothZoom - targetZoom) < 0.001f) {
            smoothZoom = targetZoom;
        }

        mainCam->SetPosition(smoothPos);
        mainCam->SetZoom(smoothZoom);
    }



    // randomly adding items 
    itemSpawnTimer -= dt;
    if (itemSpawnTimer <= 0.0f)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> xDist(-500.0f, 500.0f);
        std::uniform_real_distribution<float> yDist(-280.0f, 280.0f);

        glm::vec2 spawnPos;
        bool foundSafePos = false;
        
        for (int i = 0; i < 10; ++i) //trying 10 times for safe place 
        {
            spawnPos = glm::vec2(xDist(gen), yDist(gen));

            if (IsSafePosition(spawnPos, 90.0f))
            {
                foundSafePos = true;
                break; // spot if found
            }
        }

        if (foundSafePos)
        {
            Item* newItem = new Item();
            newItem->GetTransform2D().SetPosition(spawnPos);
            newItem->Init(engineContext);
            engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(std::unique_ptr<Object>(newItem), "[Object]Item");
        }

        itemSpawnTimer = 10.0f; 
    }


    // GAME TIMER (Ending game and texobject)
    roundTimer -= dt;
    if (roundTimer < 0.0f) roundTimer = 0.0f;

    std::string winnerMessage = "";
    bool gameEnded = false;

    if (player1->hp <= 0)
    {
        gameEnded = true;
        winnerMessage = "PLAYER 2 WON";
    }
    else if (player2->hp <= 0)
    {
        gameEnded = true;
        winnerMessage = "PLAYER 1 WON";
    }
    else if (roundTimer <= 0.0f)
    {
        gameEnded = true;

        if (player1->hp > player2->hp)
            winnerMessage = "TIME UP! PLAYER 1 WON";
        else if (player2->hp > player1->hp)
            winnerMessage = "TIME UP! PLAYER 2 WON";
        else
            winnerMessage = "TIME UP! IT'S A DRAW!";
    }

    // 스크린에서 보여주기
    if (timerTextObj != nullptr && mainCam != nullptr)
    {
        int minutes = static_cast<int>(roundTimer) / 60;
        int seconds = static_cast<int>(roundTimer) % 60;

        char timeStr[16];
        snprintf(timeStr, sizeof(timeStr), "%02d:%02d", minutes, seconds);
        timerTextObj->SetText(timeStr);

        // Kameraga bog'lash
        glm::vec2 camPos = mainCam->GetPosition();

        // Taymerni kameraning markazidan ozgina chaproq va tepadagi burchakka suramiz
        glm::vec2 timerPos = camPos + glm::vec2(-30.0f, 300.0f);

        timerTextObj->GetTransform2D().SetPosition(timerPos);
    }

    // GameOver state로 바꾸기
    if (gameEnded)
    {
        engineContext.stateManager->ChangeState(std::make_unique<GameOver>(winnerMessage));
    }
}


void Tutorial::LateUpdate(float dt, const EngineContext& engineContext)
{
}

void Tutorial::Draw(const EngineContext& engineContext)
{
    objectManager.DrawAll(engineContext);
}

void Tutorial::Free(const EngineContext& engineContext)
{
    JIN_LOG("[Tutorial] free called");
}

void Tutorial::Unload(const EngineContext& engineContext)
{
    JIN_LOG("[Tutorial] unload called");
}

// Wall Block 
void Tutorial::WallBlock(float x, float y, float size, const EngineContext& engineContext, bool isBorder)
{
    GameObject* block = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), isBorder ? "[Object]BorderVisual" : "[Object]Wall"));
    block->SetMesh(engineContext, "[EngineMesh]default");
    block->SetMaterial(engineContext, "[Material]StoneWall");
    block->GetTransform2D().SetScale({ size, size });
    block->SetRenderLayer("[Layer]Items");
    block->GetTransform2D().SetPosition(glm::vec2(x, y));

    if (!isBorder)
    {
        block->SetCollider(std::make_unique<AABBCollider>(block, glm::vec2{ size, size }));
        block->GetCollider()->SetUseTransformScale(false);
        block->SetCollision(engineContext.stateManager->GetCurrentState()->GetObjectManager(), "[Object]Wall", { "[Object]Player1", "[Object]Player2", "[Object]Bullet" });
    }
}

bool Tutorial::IsSafePosition(glm::vec2 pos, float radius)
{
    for (const auto& wPos : wallPositions)
    {
        if (glm::distance(pos, wPos) < radius) return false; // not free spot
    }

    if (player1 && glm::distance(pos, player1->GetTransform2D().GetPosition()) < radius) return false;
    if (player2 && glm::distance(pos, player2->GetTransform2D().GetPosition()) < radius) return false;

    return true; // Ok place 
}