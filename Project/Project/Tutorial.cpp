#include "Tutorial.h"

#include <random>

#include "LoadingState.h"

void Tutorial::Load(const EngineContext& engineContext)
{
    JIN_LOG("[Tutorial] load called");

    RenderManager* rm = engineContext.renderManager;

    //texture & material load

    rm->RegisterTexture("[Texture]Background", "Textures/Background/a_rgb.jpg");
    rm->RegisterMaterial("[Material]Background", "[EngineShader]default_texture", { {"u_Texture", "[Texture]Background"} });

    rm->RegisterTexture("[Texture]Tank1", "Textures/Tanks/Tank1.png");
    rm->RegisterMaterial("[Material]Tank1", "[EngineShader]default_texture", { {"u_Texture", "[Texture]Tank1"} });
    rm->RegisterTexture("[Texture]Tank2", "Textures/Tanks/Tank2.png");
    rm->RegisterMaterial("[Material]Tank2", "[EngineShader]default_texture", { {"u_Texture", "[Texture]Tank2"} });
   
}

void Tutorial::Init(const EngineContext& engineContext)
{
    JIN_LOG("[Tutorial] init called");

    // Background
    background = static_cast<GameObject*>(objectManager.AddObject(std::make_unique<GameObject>(), "[Object]Background"));
    background->SetMesh(engineContext, "[EngineMesh]default");
    background->SetMaterial(engineContext, "[Material]Background");
    int width = engineContext.windowManager->GetWidth();
    background->GetTransform2D().SetScale({ width, width });
    background->SetRenderLayer("[Layer]Background");



    // Controls of Tanks
    // Tank1 (WASD)
    player1 = static_cast<Player*>(objectManager.AddObject(std::make_unique<Player>(), "[Object]Player1"));
    player1->SetControls(KEY_W, KEY_S, KEY_A, KEY_D);

    // Tank2 
    player2 = static_cast<Player*>(objectManager.AddObject(std::make_unique<Player>(), "[Object]Player2"));
    player2->SetControls(KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
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

