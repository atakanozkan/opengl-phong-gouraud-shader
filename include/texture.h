#ifndef TEXTURE_H
#define TEXTURE_H

#include "shader.h"
#include <glad/glad.h>
#include <string>

class Texture {
public:
    Texture(const char* image_path, GLenum texture_type, GLenum slot, GLenum format, GLenum pixel_type);
    ~Texture();

    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    void Bind();
    void Unbind();
    void Delete();
    GLuint getID() const { return ID; }

private:
    GLuint ID;
    GLenum type;
    void loadTexture(const char* image_path, GLenum format, GLenum pixel_type);
    unsigned char* loadPPM(const char* filename, int& width, int& height);
};

#endif

