#include "Texture.h"

#include "STB_IMAGE/stb_image.h"

Texture::Texture(std::string&& path)
    : rendererId(0), localBuffer(nullptr), width(0), height(0), bitsPerPixel(0), filePath(std::move(path))
{
    stbi_set_flip_vertically_on_load(1);
    localBuffer = stbi_load(filePath.c_str(), &width, &height, &bitsPerPixel, 4);
    
    GLCall(glGenTextures(1, &rendererId))
    GLCall(glBindTexture(GL_TEXTURE_2D, rendererId))

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR))
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR))
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE))
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE))

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer))
    GLCall(glBindTexture(GL_TEXTURE_2D, 0))

    if (localBuffer)
    {
        stbi_image_free(localBuffer);
    }
}

Texture::~Texture()
{
    GLCall(glDeleteTextures(1, &rendererId))
}

void Texture::Bind(unsigned slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot))
    GLCall(glBindTexture(GL_TEXTURE_2D, rendererId))
}

void Texture::Unbind()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0))
}
