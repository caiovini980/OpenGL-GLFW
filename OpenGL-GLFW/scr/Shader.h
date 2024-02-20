#pragma once

#include <string>
#include <unordered_map>

#include <GLM/glm.hpp>

struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader
{
public:
    Shader(std::string&& filePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set Uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMatrix4f(const std::string& name, glm::mat4x4 matrix);

private:
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    int GetUniformLocation(const std::string& name);
    
    ShaderProgramSources ParseShader(const std::string& filePath);

    std::unordered_map<std::string, int> uniformLocationCache;
    unsigned int rendererId;
    std::string filePath;
    // cache for uniforms
};
