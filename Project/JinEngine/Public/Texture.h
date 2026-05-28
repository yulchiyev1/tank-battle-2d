#pragma once
#include <string>

using FilePath = std::string;

enum class TextureMinFilter
{
    Nearest,
    Linear,
    NearestMipmapNearest,
    LinearMipmapNearest,
    NearestMipmapLinear,
    LinearMipmapLinear
};
enum class TextureMagFilter
{
    Nearest,
    Linear,
};

enum class TextureWrap
{
    ClampToEdge,
    Repeat,
    MirroredRepeat,
    ClampToBorder
};

struct TextureSettings
{
    TextureMinFilter minFilter = TextureMinFilter::Linear;
    TextureMagFilter magFilter = TextureMagFilter::Linear;
    TextureWrap wrapS = TextureWrap::ClampToEdge;
    TextureWrap wrapT = TextureWrap::ClampToEdge;
    bool generateMipmap = true;
};

enum class ImageAccess
{
    ReadOnly,
    WriteOnly,
    ReadWrite
};

enum class ImageFormat
{
    R8, RG8, RGBA8,
    R16F, RG16F, RGBA16F,
    R32F, RG32F, RGBA32F,
    R8UI, RG8UI, RGBA8UI,
    R32UI, RG32UI, RGBA32UI
};

class Texture
{
    friend class Material;
    friend class RenderManager;
public:
    Texture(unsigned int id_, int width_, int height_, int channels_) :id(id_), width(width_), height(height_), channels(channels_) {}
    Texture(const FilePath& path, const TextureSettings& settings = {});
    Texture(const unsigned char* data, int width_, int height_, int channels_, const TextureSettings& settings = {});
    ~Texture();
    [[nodiscard]] int GetWidth() const { return width; }
    [[nodiscard]] int GetHeight() const { return height; }
    [[nodiscard]] unsigned int GetID() const { return id; }

    void BindToUnit(unsigned int unit) const;
    void BindAsImage(unsigned int unit, ImageAccess access, ImageFormat format, int level = 0) const;
    void UnBind(unsigned int unit) const;

private:
    void GenerateTexture(const unsigned char* data, const TextureSettings& settings);
    void ForceUpdateTexture(unsigned int id_, int width_, int height_, int channels_);
    void ForceUpdateTexture(const unsigned char* data, int width_, int height_, int channels_, const TextureSettings& settings = {});
    unsigned int id;
    int width, height, channels;
};
