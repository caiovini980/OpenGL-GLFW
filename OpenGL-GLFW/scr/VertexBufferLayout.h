#pragma once
#include <vector>

#include <GL/glew.h>

#include "Renderer.h"

struct VertexBufferElement
{
    unsigned int count;
    unsigned int type;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_FLOAT:
                return 4;
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
        }

        ASSERT(false)
        return 0;
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout()
        : stride(0) {}
    ~VertexBufferLayout() = default;

    template<typename T>
    void Push(unsigned int count)
    {
        ASSERT(false);
    }

    template<>
    void Push<float>(unsigned int count)
    {
        VertexBufferElement newElement;
        newElement.count = count;
        newElement.type = GL_FLOAT;
        newElement.normalized = GL_FALSE;
        
        elements.emplace_back(newElement);
        stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count)
    {
        VertexBufferElement newElement;
        newElement.count = count;
        newElement.type = GL_UNSIGNED_INT;
        newElement.normalized = GL_FALSE;
        
        elements.emplace_back(newElement);
        stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int count)
    {
        VertexBufferElement newElement;
        newElement.count = count;
        newElement.type = GL_UNSIGNED_BYTE;
        newElement.normalized = GL_TRUE;
        
        elements.emplace_back(newElement);
        stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    unsigned int GetStride() const { return stride; }
    const std::vector<VertexBufferElement>& GetElements() const { return elements; }
    
private:
    std::vector<VertexBufferElement> elements;
    unsigned int stride;
};
