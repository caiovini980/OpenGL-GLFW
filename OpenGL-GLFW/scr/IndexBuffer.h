#pragma once

class IndexBuffer
{
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    unsigned int GetCount() const { return count; }
    
private:
    unsigned int rendererId;
    unsigned int count;
};
