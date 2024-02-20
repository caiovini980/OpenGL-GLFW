#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(std::string&& filePath)
    : rendererId(0), filePath(std::move(filePath))
{
    const ShaderProgramSources sources = ParseShader(this->filePath);
    // std::cout << "VERTEX: \n" << sources.VertexSource << "\nFRAGMENT: \n" << sources.FragmentSource << "\n"; 
    rendererId = CreateShader(sources.VertexSource, sources.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(rendererId))
}

void Shader::Bind() const
{
    GLCall(glUseProgram(rendererId))
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0))
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value))
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3))
}

void Shader::SetUniformMatrix4f(const std::string& name, glm::mat4x4 matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]))
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
    {
        return uniformLocationCache[name];
    }
    
    GLCall(int location = glGetUniformLocation(rendererId, name.c_str()))

    if (location == -1)
    {
        std::cout << "WARNING: Uniform '" << name << "' doesn't exist!\n";
    }
    
    uniformLocationCache[name] = location;

    return location;
}

ShaderProgramSources Shader::ParseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT,
    };
    
    std::string line;
    std::stringstream stringStream[2];
    ShaderType type = ShaderType::NONE;
    
    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                // set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // set mode to fragment
                type = ShaderType::FRAGMENT;
            }

            continue;
        }

        stringStream[static_cast<int>(type)] << line << " \n";
    }

    return { stringStream[0].str(), stringStream[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    GLCall(glShaderSource(id, 1, &src, nullptr))
    GLCall(glCompileShader(id))

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result))
    
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length))
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        GLCall(glGetShaderInfoLog(id, length, &length, message))

        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << "Message: " << message << "\n";

        GLCall(glDeleteShader(id))
        
        return 0;
    }
    
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram()) 
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // attach compiled shaders to program
    GLCall(glAttachShader(program, vs))
    GLCall(glAttachShader(program, fs))

    // link and validate the program
    GLCall(glLinkProgram(program))
    GLCall(glValidateProgram(program))

    // clear used shaders from program
    GLCall(glDeleteShader(vs))
    GLCall(glDeleteShader(fs))

    return program;
}

