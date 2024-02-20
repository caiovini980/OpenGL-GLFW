#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();
    
    void Bind() const;
    void Unbind() const;
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void AddLayout();

private:
    unsigned int rendererId;
};
