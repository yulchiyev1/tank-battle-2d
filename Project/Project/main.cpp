#include <iostream>

#include "Debug.h"
#include "Engine.h"
#include "Tutorial.h"
#include "MainMenu.h"
#ifdef _DEBUG
//#include<vld.h>//TODO: remove this and directories before release (VC++ Directories -> Include Directories & Library Directories)
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
    jinEngine.RenderDebugDraws(true);
    //jinEngine.GetEngineContext().windowManager->RestrictResizing(true);

    jinEngine.GetEngineContext().renderManager->RegisterShader("[Shader]Animation", { {ShaderStage::Vertex,"Shaders/Animation.vert"},{ShaderStage::Fragment,"Shaders/Animation.frag"} });
    jinEngine.GetEngineContext().renderManager->RegisterMaterial("[Material]Animation", "[Shader]Animation", { });


   
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]Background", 0);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]Items", 1);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]TankBody", 2);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]Turret", 3);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]Projectile", 4);
    jinEngine.GetEngineContext().renderManager->RegisterRenderLayer("[Layer]UI", 11);
    
    jinEngine.GetEngineContext().renderManager->RegisterFont("[Font]default", "Fonts/NotoSans-VariableFont_wdth,wght.ttf", 50);
    jinEngine.GetEngineContext().renderManager->RegisterFont("[Font]defaultkr", "Fonts/NanumPenScript-Regular.ttf", 50);


    jinEngine.GetEngineContext().windowManager->SetCursorVisible(true);
    jinEngine.GetEngineContext().windowManager->SetBackgroundColor({ 0.2,0.2,0.4,1 });
    

    jinEngine.GetEngineContext().stateManager->ChangeState(std::make_unique<MainMenu>());

    jinEngine.Run();

    return 0;
}