#include <texture.hpp>
#include <stb/stb_image.h>
#include <iostream>

GLuint LoadTexture(const char *path, int width, int height)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    if (path)
    {
        stbi_set_flip_vertically_on_load(true);
        int imgWidth, imgHeight, nrChannels;
        unsigned char *data = stbi_load(path, &imgWidth, &imgHeight, &nrChannels, 0);
        if (data)
        {
            GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "Could not load texture\n";
            glDeleteTextures(1, &texture);
            return 0;
        }
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}
