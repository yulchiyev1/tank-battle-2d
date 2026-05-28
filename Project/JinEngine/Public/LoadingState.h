#pragma once
#include <functional>
#include "Engine.h"
#include "AsyncResourceLoader.h"

class LoadingState : public GameState
{
public:
    explicit LoadingState(std::function<std::unique_ptr<GameState>()> nextFactory);

    void Load(const EngineContext& engineContext) override;

    void Init(const EngineContext& engineContext) override;

    void LateInit(const EngineContext& engineContext) override;

    void Update(float dt, const EngineContext& engineContext) override;

    void LateUpdate(float dt, const EngineContext& engineContext) override;

    void Draw(const EngineContext& engineContext) override;

    void Free(const EngineContext& engineContext) override;

    void Unload(const EngineContext& engineContext) override;

    void QueueTexture(const EngineContext& engineContext, const std::string& tag, const std::string& filePath, const TextureSettings& settings = {});

    void QueueShader(const EngineContext& engineContext, const std::string& tag, const std::vector<std::pair<ShaderStage, FilePath>>& files);

    void QueueFont(const EngineContext& engineContext, const std::string& tag, const std::string& ttfPath, uint32_t pixelSize);

    void QueueSound( const std::string& tag, const std::string& path, bool loop);

    [[nodiscard]] float GetProgress01() const { return loader.GetProgress(); }

    void UploadToGPU(const LoadResult& r, const EngineContext& engineContext);

protected:
    AsyncResourceLoader loader;
    std::function<std::unique_ptr<GameState>()> nextFactory;

    std::string uiFontTag = "[Loading]UIFont";
    bool uiReady = false;
    TextObject* progressText = nullptr;
};
