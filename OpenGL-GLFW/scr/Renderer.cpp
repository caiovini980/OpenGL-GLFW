#include "Renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL ERROR]: (" << error << ")\nAt function: " << function << "\nAt file: " << file << "\nAt line: " << line << "\n";
        return false;
    }

    return true;
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
    shader.Bind();
    indexBuffer.Bind();
    vertexArray.Bind();
    
    GLCall(glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr))
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT))
}
