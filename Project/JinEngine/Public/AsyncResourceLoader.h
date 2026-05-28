#pragma once
#include <string>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <variant>
#include "Shader.h"
#include "Texture.h"


struct TextureJob
{
    std::string tag;
    std::string filePath;
    TextureSettings settings{};
};

struct ShaderFile
{
    ShaderStage stage;
    FilePath    path;
};

struct ShaderJob
{
    std::string tag;
    std::vector<ShaderFile> sources;
};

struct FontJob
{
    std::string tag;
    std::string ttfPath;
    uint32_t    pixelSize = 32;
};

struct SoundJob
{
    std::string tag;
    std::string filePath;
    bool        loop = false;
};

struct SpriteSheetJob
{
    std::string tag;
    std::string textureTag;
    int frameW = 1;
    int frameH = 1;
};

using LoadRequest = std::variant<TextureJob, ShaderJob, FontJob, SoundJob, SpriteSheetJob>;

struct TextureResult
{
    std::string tag;
    TextureSettings settings{};
    int width = 0;
    int height = 0;
    int channels = 0;
    std::vector<unsigned char> pixels;
    bool ok = false;
};

struct ShaderSource
{
    ShaderStage stage;
    std::string source;
};

struct ShaderResult
{
    std::string tag;
    std::vector<ShaderSource> sources;
    bool ok = false;
};

struct FontResult
{
    std::string tag;
    std::string ttfPath;
    uint32_t    pixelSize = 32;
    bool ok = true;
};

struct SoundResult
{
    std::string tag;
    std::string filePath;
    bool loop = false;
    bool ok = true;
};


using LoadResult = std::variant<TextureResult, ShaderResult, FontResult, SoundResult>;

class AsyncResourceLoader
{
public:
    AsyncResourceLoader() = default;
    ~AsyncResourceLoader();

    void Start();
    void Stop();

    void Enqueue(const LoadRequest& req);

    [[nodiscard]] float GetProgress() const;

    [[nodiscard]] bool HasFinished() const;

    [[nodiscard]] std::vector<LoadResult> FetchCompleted();

    [[nodiscard]] int GetTotalCount() const { return totalCount.load(); }
    [[nodiscard]] int GetLoadedCount() const { return loadedCount.load(); }

private:
    void Worker();

private:
    std::thread workerThread;
    std::atomic<bool> running{ false };

    std::queue<LoadRequest> requestQueue;
    mutable std::mutex queueMutex;
    std::condition_variable cv;

    std::vector<LoadResult> completed;
    mutable std::mutex completedMutex;

    std::atomic<int> totalCount{ 0 };
    std::atomic<int> loadedCount{ 0 };
};
