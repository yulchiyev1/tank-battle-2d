#include "Tutorial.h"

#include <random>

#include "LoadingState.h"

void Tutorial::Load(const EngineContext& engineContext)
{
    JIN_LOG("[Tutorial] load called");

   
}

void Tutorial::Init(const EngineContext& engineContext)
{
    JIN_LOG("[Tutorial] init called");
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

