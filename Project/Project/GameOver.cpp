#include "GameOver.h"
#include "MainMenu.h"

void GameOver::Init(const EngineContext& engineContext)
{
    resultText = static_cast<TextObject*>(objectManager.AddObject(std::make_unique<TextObject>(
        engineContext.renderManager->GetFontByTag("[Font]defaultkr"),
        winnerName + "\nPRESS SPACE TO MENU"
    ), "[Object]ResultText"));

    resultText->SetRenderLayer("[Layer]UI");
    resultText->GetTransform2D().SetPosition(glm::vec2(-200.0f, 0.0f));
}

void GameOver::Update(float dt, const EngineContext& engineContext)
{
    objectManager.UpdateAll(dt, engineContext);

    if (engineContext.inputManager->IsKeyDown(KEY_SPACE))
    {
        // Menyuga qaytish
        engineContext.stateManager->ChangeState(std::make_unique<MainMenu>());
    }
}

void GameOver::Draw(const EngineContext& engineContext)
{
    objectManager.DrawAll(engineContext);
}
