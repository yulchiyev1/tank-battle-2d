#pragma once

#include <string>
#include <vector>
#include "glm.hpp"

class LoadingState;

enum class ShaderStage
{
    Vertex,
    Fragment,
    Geometry,
    TessControl,
    TessEval,
    Compute
};

class RenderManager;
class Material;

using GLuint = unsigned int;
using GLint = int;
using GLenum = unsigned int;
using FilePath = std::string;

class Shader {
    friend Material;
    friend RenderManager;
    friend LoadingState;
public:
    Shader();

    ~Shader();

    void SendUniform(const std::string& name, int value) const;

    void SendUniform(const std::string& name, float value) const;

    void SendUniform(const std::string& name, const glm::vec2& value) const;

    void SendUniform(const std::string& name, const glm::vec3& value) const;

    void SendUniform(const std::string& name, const glm::vec4& value) const;

    void SendUniform(const std::string& name, const glm::mat4& value) const;

    [[nodiscard]] GLuint GetProgramID() const { return programID; }

    void Use() const;

    void Unuse() const;
private:


    [[nodiscard]] bool SupportsInstancing() const;

    bool Link();

    bool AttachFromFile(ShaderStage stage, const FilePath& filepath);

    bool AttachFromSource(ShaderStage stage, const std::string& source);

    [[nodiscard]] std::string LoadShaderSource(const FilePath& filepath, GLint& success);

    [[nodiscard]] GLuint CompileShader(ShaderStage stage, const std::string& source, GLint& success);

    void CheckSupportsInstancing();

    GLuint programID;
    std::vector<GLuint> attachedShaders;
    std::vector<ShaderStage> attachedStages;

    bool isSupportInstancing;
};
