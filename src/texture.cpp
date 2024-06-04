#include "texture.h"
#include "shader.h"
#include <iostream>
#include <fstream>
#include <glad/glad.h>

Texture::Texture(const char* image_path, GLenum texture_type, GLenum slot, GLenum format, GLenum pixel_type) {
    type = texture_type;

    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(type, ID);

    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    loadTexture(image_path, format, pixel_type);

    glBindTexture(type, 0);
}

void Texture::loadTexture(const char* image_path, GLenum format, GLenum pixel_type) {
    int width, height;
    unsigned char* image = loadPPM(image_path, width, height);
    if (image) {
        glTexImage2D(type, 0, GL_RGB, width, height, 0, format, pixel_type, image);
        glGenerateMipmap(type);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    free(image);
}

unsigned char* Texture::loadPPM(const char* filename, int& width, int& height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return nullptr;
    }

    std::string line;
    std::getline(file, line);
    bool isBinary = (line == "P6");

    if (!isBinary && line != "P3") {
        std::cerr << "Unsupported PPM file format: " << line << std::endl;
        return nullptr;
    }

    while (file.peek() == '#') {
        std::getline(file, line);
    }

    file >> width >> height;
    int maxval;
    file >> maxval;
    file.ignore(256, '\n');

    if (maxval != 255) {
        std::cerr << "Unsupported max color value: " << maxval << std::endl;
        return nullptr;
    }

    int size = width * height * 3;
    unsigned char* data = new unsigned char[size];

    if (isBinary) {
        file.read(reinterpret_cast<char*>(data), size);
        if (file.fail()) {
            std::cerr << "Error reading pixel data from binary PPM file." << std::endl;
            delete[] data;
            return nullptr;
        }
    } else {
        int r, g, b;
        for (int i = 0; i < size; i += 3) {
            file >> r >> g >> b;
            data[i] = static_cast<unsigned char>(r);
            data[i + 1] = static_cast<unsigned char>(g);
            data[i + 2] = static_cast<unsigned char>(b);
        }
    }

    file.close();
    return data;
}

Texture::~Texture() {
    glDeleteTextures(1, &ID);
    std::cout << "Texture ID " << ID << " deleted." << std::endl;
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.use();
    glUniform1i(texUni, unit);
}

void Texture::Bind() {
    glBindTexture(type, ID);
}

void Texture::Unbind() {
    glBindTexture(type, 0);
}

void Texture::Delete() {
    glDeleteTextures(1, &ID);
}
