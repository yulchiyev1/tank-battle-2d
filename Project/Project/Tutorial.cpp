#include "Tutorial.h"
#include <random>
#include <cstdlib> // rand() uchun
#include "LoadingState.h"
#include "Item.h"
#include "Player.h"
#include "GameOver.h" 
#include "Portal.h"
#include "MapLoader.h"

void Tutorial::Load(const EngineContext& engineContext)
{
    JIN_LOG("[Tutorial] load called");
    RenderManager* rm = engineContext.renderManager;

    //texture & material load
    rm->RegisterTexture("[Texture]Track", "Textures/Tanks/Tire_Track_01.png"); 
    rm->RegisterMaterial("[Material]Track", "[EngineShader]default_texture", { {"u_Texture", "[Texture]Track"} });

    rm->RegisterTexture("[Texture]Bush", "Textures/Bush1.png");
    rm->RegisterMaterial("[Material]Bush", "[EngineShader]default_texture", { {"u_Texture", "[Texture]Bush"} });

    rm->RegisterTexture("[Texture]Grass", "Textures/Background/grass.png");
    rm->RegisterMaterial("[Material]Grass", "[EngineShader]default_texture", { {"u_Texture", "[Texture]Grass"} });

    rm->RegisterTexture("[Texture]TimerBg", "Textures/Background/countdown_bg.png");
    rm->RegisterMaterial("[Material]TimerBg", "[EngineShader]default_texture", { {"u_Texture", "[Texture]TimerBg"} });

    rm->RegisterTexture("[Texture]AmmoBg", "Textures/Background/ammo_bg.png");
    rm->RegisterMaterial("[Material]AmmoBg", "[EngineShader]default_texture", { {"u_Texture", "[Texture]AmmoBg"} });

    rm->RegisterTexture("[Texture]HpBg", "Textures/HP_Bar/bg.png"); 
    rm->RegisterMaterial("[Material]HpBg", "[EngineShader]default_texture", { {"u_Texture", "[Texture]HpBg"} });

    rm->RegisterTexture("[Texture]HpBlue", "Textures/HP_Bar/blue.png");
    rm->RegisterMaterial("[Material]HpBlue", "[EngineShader]default_texture", { {"u_Texture", "[Texture]HpBlue"} });

    rm->RegisterTexture("[Texture]HpRed", "Textures/HP_Bar/red.png");
    rm->RegisterMaterial("[Material]HpRed", "[EngineShader]default_texture", { {"u_Texture", "[Texture]HpRed"} });

    rm->RegisterTexture("[Texture]HpGreen", "Textures/HP_Bar/green.png");
    rm->RegisterMaterial("[Material]HpGreen", "[EngineShader]default_texture", { {"u_Texture", "[Texture]HpGreen"} });

    rm->RegisterTexture("[Texture]HitAnim", "Textures/Projectile/hit_anim.png");
    rm->RegisterSpriteSheet("[SpriteSheet]HitAnim", "[Texture]HitAnim", 64, 64);

    rm->RegisterTexture("[Texture]Portal", "Textures/portal.png");
    rm->RegisterSpriteSheet("[SpriteSheet]Portal", "[Texture]Portal", 32, 32);

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

    rm->RegisterTexture("[Texture]Background", "Textures/Background/final_bg.png");
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

    // grass (경기장 outside)
    float grassSize = 100.0f;
    float halfGrass = grassSize / 2.0f;
    float scrW = engineContext.windowManager->GetWidth();
    float scrH = engineContext.windowManager->GetHeight();
    float halfW = scrW / 2.0f;
    float halfH = scrH / 2.0f;
    int rows = 6;
    auto PlantGrass = [&](float px, float py) {
        GameObject* grass = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]GrassTile"));
        grass->SetMesh(engineContext, "[EngineMesh]default");
        grass->SetMaterial(engineContext, "[Material]Grass");
        grass->GetTransform2D().SetScale({ grassSize, grassSize });
        grass->GetTransform2D().SetPosition({ px, py });
        grass->SetRenderLayer("[Layer]Background");
        };

    // 1. TEPA va PASTNI yopish (Burchaklar bilan birga keng qilib)
    float startX = -halfW - (rows * grassSize);
    float endX = halfW + (rows * grassSize);

    // x o'qi bo'ylab yurib chiqamiz (ochiq joy qolmasligi uchun + grassSize qo'shilgan)
    for (float x = startX; x <= endX + grassSize; x += grassSize)
    {
        for (int i = 0; i < rows; ++i)
        {
            // Devorga aniq yopishib turishi uchun halfGrass (50px) dan boshlaymiz
            float offset = halfGrass + (i * grassSize);
            PlantGrass(x, halfH + offset);  // TEPA qatorlar
            PlantGrass(x, -halfH - offset); // PASTKI qatorlar
        }
    }

    // 2. CHAP va O'NG tomonlarni yopish (Tepa va past yopilgani uchun faqat o'rtasini yopamiz)
    for (float y = -halfH; y <= halfH + grassSize; y += grassSize)
    {
        for (int i = 0; i < rows; ++i)
        {
            float offset = halfGrass + (i * grassSize);
            PlantGrass(halfW + offset, y);   // O'NG qatorlar
            PlantGrass(-halfW - offset, y);  // CHAP qatorlar
        }
    }

    // Timer bg 
    TimerBg = new GameObject();
    TimerBg->SetMesh(engineContext, "[EngineMesh]default");
    TimerBg->SetMaterial(engineContext, "[Material]TimerBg");
    TimerBg->GetTransform2D().SetScale(glm::vec2(193.f, 112.f));
    TimerBg->SetRenderLayer("[Layer]UI");
    TimerBg->SetIgnoreCamera(true);
    TimerBg->GetTransform2D().SetPosition(glm::vec2(0.f, -330.f));
    engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(std::unique_ptr<Object>(TimerBg));

    // MOUSE cursor
    cursor = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Cursor"));
    cursor->SetMaterial(engineContext, "[Material]Cursor");
    cursor->SetMesh(engineContext, "[EngineMesh]default");
    cursor->GetTransform2D().SetScale({ 30,30 });
    cursor->SetIgnoreCamera(true, cameraManager.GetActiveCamera());
    cursor->SetRenderLayer("[Layer]Cursor");

    // screen width and height
    screenW = engineContext.windowManager->GetWidth();
    screenH = engineContext.windowManager->GetHeight();

    // background
    background = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Background"));
    background->SetMesh(engineContext, "[EngineMesh]default");
    background->SetMaterial(engineContext, "[Material]Background");

    // bg image width and h
    float bgImgW = 1920.0f; 
    float bgImgH = 1080.0f; 

    // X va Y uchun proporsiyani hisoblaymiz
    float scaleX = screenW / bgImgW;
    float scaleY = screenH / bgImgH;

    // Ekranni to'liq yopish uchun kattasini tanlaymiz
    float finalScale = std::max(scaleX, scaleY);

    // Asl nisbatni buzmasdan ekranga tortamiz
    background->GetTransform2D().SetScale({ bgImgW * finalScale, bgImgH * finalScale });
    background->GetTransform2D().SetPosition({ 0.0f, 0.0f });
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

    // ENG MUHIM QADAM: Xaritani oldin yuklab olamiz
    bool isMapLoaded = MapLoader::Load("map.txt", engineContext);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> randX(-450.0f, 450.0f);
    std::uniform_real_distribution<float> randY(-250.0f, 250.0f);

    // Agar xarita yuklanmagan bo'lsa, eski random devorlarni chiqaramiz
    if (!isMapLoaded)
    {
        int wallsSpawned = 0;
        wallPositions.clear();

        int maxAttempts = 1000;
        int attempts = 0;

        while (wallsSpawned < 8 && attempts < maxAttempts)
        {
            attempts++;
            glm::vec2 testPos(randX(gen), randY(gen));

            if (IsSafePosition(testPos, 60.0f))
            {
                WallBlock(testPos.x, testPos.y, wall_len, engineContext, false);
                wallPositions.push_back(testPos);
                wallsSpawned++;
            }
        }
    }

    // PORTAL 만들기 and 서로 연결하기
    glm::vec2 posA, posB;
    bool foundA = false;
    bool foundB = false;

    // 1-REJIM: Agar .txt xarita yuklangan va ochiq joylar bo'lsa
    if (isMapLoaded && !MapLoader::freeSpaces.empty())
    {
        std::uniform_int_distribution<int> dist(0, MapLoader::freeSpaces.size() - 1);

        // Portal A uchun
        int idxA = dist(gen);
        posA = MapLoader::freeSpaces[idxA];
        MapLoader::freeSpaces.erase(MapLoader::freeSpaces.begin() + idxA);
        foundA = true;

        // Portal B uchun
        for (int i = 0; i < 50; ++i)
        {
            if (MapLoader::freeSpaces.empty()) break;

            std::uniform_int_distribution<int> distB(0, MapLoader::freeSpaces.size() - 1);
            int idxB = distB(gen);
            glm::vec2 testPos = MapLoader::freeSpaces[idxB];

            if (glm::distance(posA, testPos) > 500.0f)
            {
                posB = testPos;
                MapLoader::freeSpaces.erase(MapLoader::freeSpaces.begin() + idxB);
                foundB = true;
                break;
            }
        }

        if (!foundB && !MapLoader::freeSpaces.empty())
        {
            posB = MapLoader::freeSpaces[0];
            MapLoader::freeSpaces.erase(MapLoader::freeSpaces.begin());
            foundB = true;
        }
    }
    // 2-REJIM: Xarita yo'q bo'lsa, eski usulda topamiz
    else
    {
        for (int i = 0; i < 50; ++i)
        {
            glm::vec2 testPos(randX(gen), randY(gen));
            if (IsSafePosition(testPos, 100.0f))
            {
                posA = testPos;
                foundA = true;
                break;
            }
        }

        if (foundA)
        {
            for (int i = 0; i < 50; ++i)
            {
                glm::vec2 testPos(randX(gen), randY(gen));
                if (IsSafePosition(testPos, 100.0f) && glm::distance(posA, testPos) > 500.0f)
                {
                    posB = testPos;
                    foundB = true;
                    break;
                }
            }
        }
    }

    // after found safe spots, addObject
    if (foundA && foundB)
    {
        auto portalA = std::make_unique<Portal>();
        auto portalB = std::make_unique<Portal>();

        portalA->GetTransform2D().SetPosition(posA);
        portalB->GetTransform2D().SetPosition(posB);

        Portal* ptrA = static_cast<Portal*>(objectManager.AddObject(std::move(portalA), "[Object]Portal"));
        Portal* ptrB = static_cast<Portal*>(objectManager.AddObject(std::move(portalB), "[Object]Portal"));

        ptrA->linkedPortal = ptrB;
        ptrB->linkedPortal = ptrA;
    }

    // Test Item (Birinchi item doim markazda (0,0) paydo bo'ladi!)
    Item* testItem = new Item();
    testItem->Init(engineContext);
    testItem->GetTransform2D().SetPosition(glm::vec2(0.0f, 0.0f));
    objectManager.AddObject(std::unique_ptr<Object>(testItem), "[Object]Item");

    // timer added
    if (timerTextObj == nullptr) {
        timerTextObj = static_cast<TextObject*>(engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(
            std::make_unique<TextObject>(engineContext.renderManager->GetFontByTag("[Font]defaultkr"), "02:00"),
            "[Object]TimerText"
        ));
    }
    timerTextObj->SetRenderLayer("[Layer]HPUI");
    timerTextObj->GetTransform2D().SetScale(glm::vec2(0.7f, 0.7f));
}

void Tutorial::LateInit(const EngineContext& engineContext)
{
}

void Tutorial::Update(float dt, const EngineContext& engineContext)
{
    objectManager.UpdateAll(dt, engineContext);

    // tracks of tank
    static std::vector<std::pair<GameObject*, float>> activeTracks;
    static glm::vec2 lastTrackPos1 = player1->GetTransform2D().GetPosition();
    static glm::vec2 lastTrackPos2 = player2->GetTransform2D().GetPosition();
    float trackDistance = 50.0f;
    float trackLifeTime = 1.f; // Izlar 3 soniya saqlanadi

    // 1-TANK 
    glm::vec2 p1Pos = player1->GetTransform2D().GetPosition();
    if (glm::distance(p1Pos, lastTrackPos1) > trackDistance)
    {
        GameObject* track = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Track"));
        track->SetMesh(engineContext, "[EngineMesh]default");
        track->SetMaterial(engineContext, "[Material]Track");
        track->GetTransform2D().SetPosition(p1Pos);
        track->GetTransform2D().SetRotation(player1->GetTransform2D().GetRotation());
        track->GetTransform2D().SetScale({ 80.0f, 80.0f });
        track->SetRenderLayer("[Layer]Items");

        activeTracks.push_back({ track, trackLifeTime });
        lastTrackPos1 = p1Pos;
    }

    // 2-TANK 
    glm::vec2 p2Pos = player2->GetTransform2D().GetPosition();
    if (glm::distance(p2Pos, lastTrackPos2) > trackDistance)
    {
        GameObject* track = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Track"));
        track->SetMesh(engineContext, "[EngineMesh]default");
        track->SetMaterial(engineContext, "[Material]Track");
        track->GetTransform2D().SetPosition(p2Pos);
        track->GetTransform2D().SetRotation(player2->GetTransform2D().GetRotation());
        track->GetTransform2D().SetScale({ 80.0f, 80.0f });
        track->SetRenderLayer("[Layer]Items");

        activeTracks.push_back({ track, trackLifeTime });
        lastTrackPos2 = p2Pos;
    }

    // IZLARNING TAYMERI VA XIRALASHISH MANTIG'I
    for (auto it = activeTracks.begin(); it != activeTracks.end(); )
    {
        it->second -= dt;
        if (it->second > 0.0f)
        {
            float alpha = std::min(it->second, 1.0f);
            it->first->SetColor({ 1.0f, 1.0f, 1.0f, alpha });
            ++it;
        }
        else
        {
            it->first->Kill();
            it = activeTracks.erase(it);
        }
    }

    //bush (경기장 outside)
    static float bushTimer = 0.0f;
    static int bushCount = 0;
    bushTimer += dt;
    if (bushTimer >= 1.0f && bushCount < 100) 
    {
        bushTimer = 0.0f;
        float hw = engineContext.windowManager->GetWidth() / 2.0f;
        float hh = engineContext.windowManager->GetHeight() / 2.0f;
        for (int i = 0; i < 3; ++i)
        {
            float x = 0, y = 0;
            int side = rand() % 4;                
            float offset = 0.0f + (rand() % 50); 
            if (side == 0) { x = (rand() % (int)(hw * 2)) - hw; y = hh + offset; }  // Tepa
            else if (side == 1) { x = (rand() % (int)(hw * 2)) - hw; y = -hh - offset; } // Past
            else if (side == 2) { x = -hw - offset; y = (rand() % (int)(hh * 2)) - hh; } // Chap
            else { x = hw + offset;  y = (rand() % (int)(hh * 2)) - hh; } // O'ng
            GameObject* bush = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Bush"));
            bush->SetMesh(engineContext, "[EngineMesh]default");
            bush->SetMaterial(engineContext, "[Material]Bush");
            bush->GetTransform2D().SetScale({ 30.0f, 20.0f });
            bush->GetTransform2D().SetPosition({ x, y });
            bush->SetRenderLayer("[Layer]Items");
            bushCount++;
        }
    }
    //cursor
    cursor->GetTransform2D().SetPosition(glm::vec2(engineContext.inputManager->GetMousePos().x - engineContext.windowManager->GetWidth() / 2.f, engineContext.windowManager->GetHeight() / 2.f - engineContext.inputManager->GetMousePos().y) + glm::vec2(11, -11));
    if (engineContext.inputManager->IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || engineContext.inputManager->IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) || engineContext.inputManager->IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        cursor->SetColor({ 0.7f, 0.7f, 0.7f, 1.0f });

    }
    if (engineContext.inputManager->IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || engineContext.inputManager->IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE) || engineContext.inputManager->IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
    {
        cursor->SetColor({ 1.0f, 1.0f,1.0f,1.0f });
    }

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

        float panSpeed = 6.0f;
        float zoomSpeed = 1.0f;

        float panFactor = 1.0f - std::exp(-panSpeed * dt);
        float zoomFactor = 1.0f - std::exp(-zoomSpeed * dt);

        glm::vec2 currentPos = mainCam->GetPosition();
        float currentZoom = mainCam->GetZoom();

        float smoothZoom = glm::mix(currentZoom, targetZoom, zoomFactor);
        if (std::abs(smoothZoom - targetZoom) < 0.001f) {
            smoothZoom = targetZoom;
        }

        float scrW = engineContext.windowManager->GetWidth();
        float scrH = engineContext.windowManager->GetHeight();

        float padding = 30.0f;

        float mapBoundX = (scrW / 2.0f) + padding;
        float mapBoundY = (scrH / 2.0f) + padding;

        float camViewHalfW = (scrW / 2.0f) / smoothZoom;
        float camViewHalfH = (scrH / 2.0f) / smoothZoom;

        float minCamX = -mapBoundX + camViewHalfW;
        float maxCamX = mapBoundX - camViewHalfW;
        float minCamY = -mapBoundY + camViewHalfH;
        float maxCamY = mapBoundY - camViewHalfH;

        if (minCamX > maxCamX) minCamX = maxCamX = 0.0f;
        if (minCamY > maxCamY) minCamY = maxCamY = 0.0f;

        targetPos.x = std::clamp(targetPos.x, minCamX, maxCamX);
        targetPos.y = std::clamp(targetPos.y, minCamY, maxCamY);

        glm::vec2 smoothPos = glm::mix(currentPos, targetPos, panFactor);
        if (glm::distance(smoothPos, targetPos) < 0.5f) {
            smoothPos = targetPos;
        }

        mainCam->SetPosition(smoothPos);
        mainCam->SetZoom(smoothZoom);
    }

    // randomly adding items 
    itemSpawnTimer -= dt;
    if (itemSpawnTimer <= 0.0f)
    {
        glm::vec2 spawnPos;
        bool foundSafePos = false;

        // IKKI REJIMLI TEKSHIRUV
        if (!MapLoader::freeSpaces.empty())
        {
            // Xarita o'qilgan bo'lsa, zamonaviy generator orqali haqiqiy random qilamiz
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, MapLoader::freeSpaces.size() - 1);

            int idx = dist(gen);
            spawnPos = MapLoader::freeSpaces[idx];

            // Item ustma-ust tushmasligi uchun ro'yxatdan o'chiramiz
            MapLoader::freeSpaces.erase(MapLoader::freeSpaces.begin() + idx);
            foundSafePos = true;
        }
        else
        {
            // Eski random rejim (agar xarita bo'lmasa)
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<float> xDist(-500.0f, 500.0f);
            std::uniform_real_distribution<float> yDist(-280.0f, 280.0f);

            for (int i = 0; i < 10; ++i)
            {
                spawnPos = glm::vec2(xDist(gen), yDist(gen));

                if (IsSafePosition(spawnPos, 90.0f))
                {
                    foundSafePos = true;
                    break;
                }
            }
        }

        if (foundSafePos)
        {
            Item* newItem = new Item();
            newItem->GetTransform2D().SetPosition(spawnPos);
            engineContext.stateManager->GetCurrentState()->GetObjectManager().AddObject(std::unique_ptr<Object>(newItem), "[Object]Item");
        }

        itemSpawnTimer = 10.0f;
    }


    // GAME TIMER (Ending game and texobject)
    roundTimer -= dt;
    if (roundTimer < 0.0f) roundTimer = 0.0f;

    std::string winnerMessage = "";
    bool gameEnded = false;

    int winnerID = 0;
    if (player1->hp <= 0)
    {
        gameEnded = true;
        winnerID = 2;
    }
    else if (player2->hp <= 0)
    {
        gameEnded = true;
        winnerID = 1;
    }
    else if (roundTimer <= 0.0f)
    {
        gameEnded = true;
        if (player1->hp > player2->hp) winnerID = 1;
        else if (player2->hp > player1->hp) winnerID = 2;
        else winnerID = 0; 
    }

    // timer 스크린에서 보여주기
    if (timerTextObj != nullptr)
    {
        int minutes = static_cast<int>(roundTimer) / 60;
        int seconds = static_cast<int>(roundTimer) % 60;
        char timeStr[16];
        snprintf(timeStr, sizeof(timeStr), "%02d:%02d", minutes, seconds);
        timerTextObj->SetText(timeStr);
        timerTextObj->SetIgnoreCamera(true);
        glm::vec2 timerPos = glm::vec2(-26.f, -306.0f);
        timerTextObj->GetTransform2D().SetPosition(timerPos);
    }

    // GameOver state로 바꾸기
    if (gameEnded)
    {
        engineContext.stateManager->ChangeState(std::make_unique<GameOver>(winnerID));
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