#pragma once

#include "Renderer.h"

class Texture
{
public:
    Texture(std::string&& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind();

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    
    
private:
    unsigned int rendererId;
    unsigned char* localBuffer;
    int width;
    int height;
    int bitsPerPixel;
    std::string filePath;
    
};
