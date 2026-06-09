#include "Tutorial.h"
#include <random>
#include "LoadingState.h"
#include "Item.h"

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
    std::uniform_real_distribution<float> randX(-500.0f, 500.0f);
    std::uniform_real_distribution<float> randY(-350.0f, 350.0f);
    for (int i = 0; i < 8; ++i)
    {
        float spawnX = randX(gen);
        float spawnY = randY(gen);
        WallBlock(spawnX, spawnY, wall_len, engineContext, false); // 'false' = Oddiy devor, kollayderi bor
    }

    // TEST UCHUN 1 TA QUTI (X: 0, Y: 0) MARKAZGA TUSHIRAMIZ
    Item* testItem = new Item();
    testItem->Init(engineContext);
    testItem->GetTransform2D().SetPosition(glm::vec2(0.0f, 0.0f));
    objectManager.AddObject(std::unique_ptr<Object>(testItem), "[Object]Item");
}

void Tutorial::LateInit(const EngineContext& engineContext)
{
}

void Tutorial::Update(float dt, const EngineContext& engineContext)
{
    objectManager.UpdateAll(dt, engineContext);
}

void Tutorial::LateUpdate(float dt, const EngineContext& engineContext)
{
}

void Tutorial::Draw(const EngineContext& engineContext)
{
    objectManager.DrawAll(engineContext);
}

void Tutorial::PostProcessing(const EngineContext& engineContext)
{

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