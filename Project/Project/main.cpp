#include <iostream>

#include "Debug.h"
#include "Engine.h"
#include "Tutorial.h"
#include "MainMenu.h"
#ifdef _DEBUG
#endif
int main(int argc, char* argv[])
{
    JinEngine jinEngine;
    DebugLogger::SetLogLevel(LogLevel::All);
    int width = 1280;
    int height = 720;

    try
    {
        if (argc == 3)
        {
            width = std::stoi(argv[1]);
            height = std::stoi(argv[2]);
        }
        else if (argc != 1)
        {
            JIN_ERR("Usage: ./MyGame [width height]");
            return -1;
        }
    }
    catch (const std::exception& e)
    {
        JIN_ERR("Invalid arguments. Width and height must be integers.");
        return -1;
    }

    if (!jinEngine.Init(width, height))
    {
        JIN_ERR("Engine initialization failed.");
        return -1;
    }
    jinEngine.RenderDebugDraws(false);
    jinEngine.GetEngineContext().windowManager->RestrictResizing(true);

    jinEngine.GetEngineContext().renderManager->RegisterShader("[Shader]Animation", { {ShaderStage::Vertex,"Shaders/Animation.vert"},{ShaderStage::Fragment,"Shaders/Animation.frag"} });
    jinEngine.GetEngineContext().renderManager->RegisterMaterial("[Material]Animation", "[Shader]Animation", { });


    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]Back", 0);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]Background", 1);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]Items", 2);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]TankBody", 3);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]Turret", 4);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]Projectile", 5);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]UI", 11);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]POPUP", 12);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]HPUI", 13);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]Cursor", 14);

    
    jinEngine.GetEngineContext().renderManager->RegisterFont("[Font]default", "Fonts/NotoSans-VariableFont_wdth,wght.ttf", 50);
    jinEngine.GetEngineContext().renderManager->RegisterFont("[Font]defaultkr", "Fonts/NanumPenScript-Regular.ttf", 50);

    jinEngine.GetEngineContext().renderManager->RegisterTexture("[Texture]Cursor", "Textures/cursor.png");
    jinEngine.GetEngineContext().renderManager->RegisterMaterial("[Material]Cursor", "[EngineShader]default_texture", { {"u_Texture","[Texture]Cursor"} });

    jinEngine.GetEngineContext().windowManager->SetCursorVisible(false);
    jinEngine.GetEngineContext().windowManager->SetBackgroundColor({ 0.2,0.2,0.4,1 });
    

    jinEngine.GetEngineContext().stateManager->ChangeState(std::make_unique<MainMenu>());

    jinEngine.Run();

    return 0;
}