#include "MainMenu.h"
#include "Tutorial.h" 

void MainMenu::Load(const EngineContext& engineContext)
{
    JIN_LOG("[MainMenu] load called");
    // Agar menyu uchun orqa fon rasmlari bo'lsa, shu yerda Register qilasiz
}

void MainMenu::Init(const EngineContext& engineContext)
{
    JIN_LOG("[MainMenu] init called");

    // game name
    titleText = static_cast<TextObject*>(objectManager.AddObject(std::make_unique<TextObject>(
        engineContext.renderManager->GetFontByTag("[Font]defaultkr"),
        "TANK BATTLE"
    ), "[Object]TitleText"));

    titleText->SetRenderLayer("[Layer]UI");
    titleText->GetTransform2D().SetScale(glm::vec2(1.5f, 1.5f)); 
    titleText->GetTransform2D().SetPosition(glm::vec2(-150.0f, 100.0f)); 

    // start text
    pressStartText = static_cast<TextObject*>(objectManager.AddObject(std::make_unique<TextObject>(
        engineContext.renderManager->GetFontByTag("[Font]defaultkr"),
        "PRESS SPACE TO START"
    ), "[Object]StartText"));

    pressStartText->SetRenderLayer("[Layer]UI");
    pressStartText->GetTransform2D().SetScale(glm::vec2(0.8f, 0.8f));
    pressStartText->GetTransform2D().SetPosition(glm::vec2(-200.0f, -50.0f)); // Sarlavhaning tagida
}

void MainMenu::Update(float dt, const EngineContext& engineContext)
{
    objectManager.UpdateAll(dt, engineContext);

    // space > 경기장
    if (engineContext.inputManager->IsKeyDown(KEY_SPACE)) // yoki InputManager::Key::Space
    {
        engineContext.stateManager->ChangeState(std::make_unique<Tutorial>());
    }
}

void MainMenu::Draw(const EngineContext& engineContext)
{
    objectManager.DrawAll(engineContext);
}

void MainMenu::LateInit(const EngineContext& engineContext) {}
void MainMenu::LateUpdate(float dt, const EngineContext& engineContext) {}
void MainMenu::PostProcessing(const EngineContext& engineContext) {}

void MainMenu::Free(const EngineContext& engineContext) {
    JIN_LOG("[MainMenu] free called");
}
void MainMenu::Unload(const EngineContext& engineContext) {
    JIN_LOG("[MainMenu] unload called");
}