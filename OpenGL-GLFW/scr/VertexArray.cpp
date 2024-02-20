#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &rendererId))
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &rendererId))
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(rendererId))
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0))
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();

    unsigned int offset = 0;
    const auto& elements = layout.GetElements();
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset))
        GLCall(glEnableVertexAttribArray(i))

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::AddLayout()
{
}
